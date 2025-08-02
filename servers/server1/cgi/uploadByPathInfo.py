import sys
import os

length = int(os.environ.get("CONTENT_LENGTH", 0))
if int(length) == 0:
    print("ok")
    sys.exit()
read_total = 0
data = b""

while read_total < length:
    chunk = sys.stdin.buffer.read(min(4096, length - read_total))
    if not chunk:
        break
    data += chunk
    read_total += len(chunk)

path_info = os.environ.get("PATH_INFO", "/cgi/upload/default")
upload_dir, filename = os.path.split(path_info)
if not upload_dir:
    upload_dir = "/upload"
root_path = os.environ.get("ROOT_PATH", ".")
full_upload_dir = f"{root_path}{upload_dir}/"
upload_path = f"{root_path}{path_info}"

os.makedirs(full_upload_dir, exist_ok=True)
with open(upload_path, "wb") as f:
    f.write(data)

print("ok")