#!/bin/bash

if [ -z "$NAME" ]; then
    name="Who"
else
    name=$NAME
fi

if [ -z "$SAY" ]; then
    said="Hi"
else
    said=$SAY
fi

content="<html lang="en"><head><title>WhoSayHi</title></head><body><main><h1>$name say $said!</h1></main></body></html>"

echo "HTTP/1.1 200 ok"
echo "Content-Type: text/html"
echo -n "Content-Length: "
echo -n $content | wc -c
echo
echo $content
