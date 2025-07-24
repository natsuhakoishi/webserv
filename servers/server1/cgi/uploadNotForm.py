#!/usr/bin/env python3

import sys
import os

test = os.environ.get("Content-Length", 0)
if int(test) == 0:
    test = os.environ.get("CONTENT_LENGTH", 0)
    if int(test) == 0:
        print("ok")
        sys.exit()

length = int(test)
body = sys.stdin.read(length)

filename = os.environ.get("FILEN", "default")
upload_dir = os.environ.get("UPLOAD_PATH", "/upload")
root_path = os.environ.get("ROOT_PATH", ".")
full_upload_dir = f"{root_path}{upload_dir}/"
upload_path = f"{full_upload_dir}{filename}"

os.makedirs(full_upload_dir, exist_ok=True)
with open(upload_path, "w") as f:
    f.write(body)

print("ok")