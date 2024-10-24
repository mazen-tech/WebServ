import os
import sys

query_string = os.getenv("QUERY_STRING", "")
content_length = int(os.getenv("CONTENT_LENGTH", 0))
request_method = os.getenv("REQUEST_METHOD", "")
post_data = sys.stdin.read(content_length)
print("Status: 200 success")
print("Content-Type: text/html\n")
print("<html>")
print("<head><title>CGI Request Example</title></head>")
print("<body>")
print("<p>")
print(f"quey string is {query_string}")
print(f"content length is {content_length}")
print(f"request method is {request_method}")
print(f"post data is {post_data}")
print("</p>")
print("</body></html>")
