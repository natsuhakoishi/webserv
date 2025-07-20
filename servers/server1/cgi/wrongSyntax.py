#!/usr/bin/env python3

import o

name = os.environ.get("NAME", "Who")
said = os.environ.get("SAY", "Hi")
content = f"""<html lang="en">
<head>
    <title>WhoSayHi</title>
</head>
<body>
    <main>
        <h1>{name} say {said}!</h1>
    </main>
</body>
</html>"""

print("HTTP/1.1 200 ok")
print("Content-Type: text/html")
print("Content-Length:", len(content))
print()
print(content)