#!/usr/bin/env python3
import sys
import os
import cgi




# length = int(os.environ.get("Content-Length", 0))
# read_total = 0
# data = b""

# while read_total < length:
#     chunk = sys.stdin.buffer.read(min(4096, length - read_total))
#     if not chunk:
#         break
#     data += chunk
#     read_total += len(chunk)

form = cgi.FieldStorage()

if 'Y' not in os.environ.get("GET_METHOD") or 'Y' not in os.environ.get("POST_METHOD"):
    print("403")
    sys.exit(1)

# print("HTTP/1.1 200 ok")


if "upload_file" in form:
    fileitem = form["upload_file"]
    
    if fileitem.filename:
        filename = os.path.basename(fileitem.filename)
        upload_dir = os.environ.get("UPLOAD_PATH")
        if not upload_dir:
            upload_dir = "/upload"
        root_path = os.environ.get("ROOT_PATH", ".")
        full_upload_dir = f"{root_path}{upload_dir}/"
        upload_path = f"{full_upload_dir}{filename}"

        os.makedirs(full_upload_dir, exist_ok=True)

        with open(upload_path, "wb") as f:
            f.write(fileitem.file.read())

        content = "ok"
    else:
        content = "ok"
else:
    content = "500"

# print(f"Content-Length: {len(content)}")
# print("Content-Type: text/html\n")
print(content)