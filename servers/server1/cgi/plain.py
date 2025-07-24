#!/usr/bin/env python3

import sys
import os
import cgi

test = os.environ.get("Content-Length", 0)
if int(test) == 0:
    print("404")
    sys.exit(1)

length = int(test)
body = sys.stdin.read(length)

filename = os.environ.get("FILEN", "default");
upload_dir = os.environ.get("UPLOAD_PATH")
if not upload_dir:
    upload_dir = "/upload"
root_path = os.environ.get("ROOT_PATH", ".")
full_upload_dir = f"{root_path}{upload_dir}/"
upload_path = f"{full_upload_dir}{filename}"

os.makedirs(full_upload_dir, exist_ok=True)
with open(upload_path, "w") as f:
    f.write(body)

content = "ok"
print(content)
