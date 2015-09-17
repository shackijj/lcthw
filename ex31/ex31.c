#include <unistd.h>

int main()
{
    int i = 0;
    int arr[10] = {};

    while(i < 100) {
        usleep(3000);
        arr[i] = i;
        i++;
    }
    return 0;
}
