#!/usr/bin/env python3
import sys
import os

print("HTTP/1.1 200 ok")

length = int(os.environ.get("CONTENT_LENGTH", 0))
read_total = 0
data = b""

while read_total < length:
    chunk = sys.stdin.buffer.read(min(4096, length - read_total))
    if not chunk:
        break
    data += chunk
    read_total += len(chunk)

content = f"<pre>Expected: {length}, Got: {read_total} bytes</pre>"
print(f"Content-Length: {len(content)}")
print("Content-Type: text/html\n")
print(content)