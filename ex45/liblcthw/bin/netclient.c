#undef NDEBUG
#include <stdlib.h>
#include <sys/select.h>
#include <stdio.h>
#include <lcthw/ringbuffer.h>
#include <lcthw/dbg.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>


/*
TODO

1. Check connection closed/opened
2. Show usage function
3. Make it read whole web page http://c.learncodethehardway.org/book/ex45.html
   Now it fails with 

[ERROR] (src/lcthw/ringbuffer.c:82 function: RingBuffer_gets errno: None) Need more than 0 for gets, you gave: -1
[ERROR] (bin/netclient.c:93 function: write_some errno: None) Failed to get from the buffer.
[DEBUG] bin/netclient.c:197: failed to write to stdout.

*/
struct tagbstring NL = bsStatic("\n");
struct tagbstring CLRF = bsStatic("\r\n");

int nonblock(int fd) {
    // Get the file access mode
    int flags = fcntl(fd, F_GETFL, 0);
    check(flags >= 0, "Invalid flags on nonblock.");

    // Add NONBLOCK flag
    int rc = fcntl(fd, F_SETFL, flags | O_NONBLOCK);
    check(rc == 0, "Can't set nonblocking.");

    return 0;
error:
    return -1;
}

int client_connect(char *host, char *port)
{
    int rc = 0;
    // struct describer somewhere in soket, types or netdb.
    // we need it for creating correct socket struct.
    struct addrinfo *addr = NULL;
    // DNS lookup for uri. 
    rc = getaddrinfo(host, port, NULL, &addr);
    check(rc == 0, "Failed to lookup %s:%s", host, port);
    // creates  an endpoint for communication
    // over ipv4 AF_INET, 
    // SOCK_STREAM  Provides sequenced, reliable, two-way, connection-based
    // byte streams
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    check(sock >= 0, "Cannot create a socket.");
    // connect soket with network address
    rc = connect(sock, addr->ai_addr, addr->ai_addrlen);
    check(rc == 0, "Connect failed.");
    // make socket nonblocking
    rc = nonblock(sock);
    check(rc == 0, "Can't set nonblocking.");
    //!
    freeaddrinfo(addr);
    return sock;

error:
    freeaddrinfo(addr);
    return -1;
}

int read_some(RingBuffer *buffer, int fd, int is_socket)
{
    int rc = 0;

    if(RingBuffer_available_data(buffer) == 0 ) {
        buffer->start = buffer->end = 0;
    }

    if(is_socket) {
        // read from socket to buffer.
        rc = recv(fd, RingBuffer_starts_at(buffer), RingBuffer_available_space(buffer), 0);
    } else {
        // read from file.
        rc = read(fd, RingBuffer_starts_at(buffer), RingBuffer_available_space(buffer));
    }
    
    check(rc >= 0, "Failed to read from fd: %d", fd);

    RingBuffer_commit_write(buffer, rc);

    return rc;

error:
    return -1;
}

int write_some(RingBuffer *buffer, int fd, int is_socket, int remove_flag)
{
    int rc = 0;
    bstring data = RingBuffer_get_all(buffer);

    check(data != NULL, "Failed to get from the buffer.");
    if (remove_flag == 1) {
        check(bfindreplace(data, &NL, &CLRF, 0) == BSTR_OK, "Failed to replace NL.");
    } else {
        debug("I don't do replace");
    }

    if(is_socket) {
        // Send data from buffer to connection recipient
        // Socket should be in connected state
        rc = send(fd, bdata(data), blength(data), 0);
    } else {
        // Write data to file.
        rc = write(fd, bdata(data), blength(data));
    }

    check(rc == blength(data), "Failed to write everything to fd: %d", fd);
    bdestroy(data);

    return rc;

error:
    return -1;
}

int main(int argc, char *argv[])
{
    // declare file sets
    fd_set allreads;
    fd_set readmask;

    int socket = 0;
    int rc = 0;
    int remove_flag = 1;

    check(argc >= 3, "Usage: netclient [-r] host port");

    char *host = argv[1];
    char *port = argv[2];

    while((rc = getopt(argc, argv, "r:")) != -1) {
       switch (rc) {
            case 'r':
                remove_flag = -1;
                log_info("Remove flag is -1");
                host = argv[2];
                port = argv[3];                
                break;
            default:
                return -1;
                break;
       }
    }

    RingBuffer *in_rb = RingBuffer_create(1024 * 10);
    RingBuffer *sock_rb = RingBuffer_create(1024 * 10);
    // Returns socket
    socket = client_connect(host, port);
    check(socket >= 0, "connect to %s:%s failed.", host, port);
    // Clear set of descriptors
    FD_ZERO(&allreads);
    // Add socket descriptor to allreads set
    FD_SET(socket, &allreads);
    // Add stdin to allreads set
    FD_SET(0, &allreads);

    while(1) {
       // Endless loop - discrete mode. 
       // We need this for secure handling and 
       // gives possibility to change original set
       readmask = allreads;
        
       /*
       select() allow  a  program  to  monitor  multiple  file
       descriptors,  waiting  until one or more of the file 
       descriptors become "ready" for some class of 
       I/O operation (e.g., input possible). 
 
       first arg is the highest-numbered file descriptor 
       in any of the three  sets, plus 1
        */

        // All ready for reading/wriing?
        rc = select(socket + 1, &readmask, NULL, NULL, NULL);
        check(rc >= 0, "select failed.");
        // Ok. Here we go

        // Check: Is stdin part of the set readmask?
        if(FD_ISSET(0, &readmask)) {
            // Yes. Read from it like file. 
            rc = read_some(in_rb, 0, 0);
            check_debug(rc != -1, "failed to read from stdin.");
        }
        
        // Check: Is socket part of the set readmask?
        if(FD_ISSET(socket, &readmask)) {
            rc = read_some(sock_rb, socket, 0);
            check_debug(rc != -1, "failed to read from socket.");
        }

        // Get data from socket buffer and write it to stdout
        // while sock_rb not empty
        while(!RingBuffer_empty(sock_rb)) {
            rc = write_some(sock_rb, 1, 0, remove_flag);
            check_debug(rc != -1, "failed to write to stdout.");
        }
        // Like upper loop, but write to socket.
        while(!RingBuffer_empty(in_rb)) {
            rc = write_some(in_rb, socket, 1, remove_flag);
            check_debug(rc != -1, "Failed to write to socket.");
        }
    }

    return 0;

error:
    return -1;
}   
