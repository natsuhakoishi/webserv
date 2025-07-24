import sys
import os

length = int(os.environ.get("CONTENT_LENGTH", 0))
read_total = 0
data = b""

while read_total < length:
    chunk = sys.stdin.buffer.read(min(4096, length - read_total))
    if not chunk:
        break
    data += chunk
    read_total += len(chunk)

print("ok")

# content = f"""<html lang="en">
# <head>
#     <title>ReadChunkTest</title>
# </head>
# <body>
#     <main>
#         {data}
#     </main>
# </body>
# </html>"""

# print("HTTP/1.1 200 ok")
# print("Content-Type: text/html")
# print("Content-Length:", len(content))
# print()
# print(content)