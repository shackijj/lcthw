#/bin/bash

./devpkg -I "http://nginx.org/download/nginx-1.9.4.tar.gz" \
-c --without-http_gzip_module -c --without-http_rewrite_module
