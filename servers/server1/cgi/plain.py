import sys
import os
import cgi

length = int(os.environ.get("Content-Length", 0))
read_total = 0
data = b""

while read_total < length:
    chunk = sys.stdin.buffer.read(min(4096, length - read_total))
    if not chunk:
        break
    data += chunk
    read_total += len(chunk)

