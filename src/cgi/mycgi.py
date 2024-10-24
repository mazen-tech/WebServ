import os
import sys

query_string = os.getenv("QUERY_STRING", "")
content_length = int(os.getenv("CONTENT_LENGTH", 0))
request_method = os.getenv("REQUEST_METHOD", "")
print(f"quey string is {query_string}")
print(f"content length is {content_length}")
print(f"request method is {request_method}")

