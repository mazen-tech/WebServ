import os
import sys

status = "Status: 500 server error"

def insert_env(line):
    part_one = line.split('{{')[0]
    middle = line.split('{{')[1].split('}}')[0]
    part_two = line.split('}}')[1]
    middle = os.getenv(middle, "")
    return part_one + middle + part_two

def render(page):
    print("Status: 200 success")
    print("Content-Type: text/html\n\n")
    with open(f"{os.getcwd()}/cgi/html/{page}") as source:
        for line in source:
            if ('{{' in line and '}}' in line):
                line = insert_env(line)
            print(line)

pages = os.listdir(f'{os.getcwd()}/cgi/html')
print(pages)
page = sys.argv[1]
print(page)
if page in pages:
    print(f"page {page} in pages")
    status = "Status: 200 success"
    render(page)
else:
    print("page not in pages")

# query_string = os.getenv("QUERY_STRING", "")
# content_length = int(os.getenv("CONTENT_LENGTH", 0))
# request_method = os.getenv("REQUEST_METHOD", "")
# post_data = sys.stdin.read(content_length)

# print("\nENV\n")
# env_vars = os.environ
# for key, value in env_vars.items():
#     print(f'{key}: {value}')
# print("\nend ENV\n")


# print("Status: 200 success")
# print("Content-Type: text/html\n")
# print("<html>")
# print("<head><title>CGI Request Example</title></head>")
# print("<body>")
# print("<p>")
# print(f"quey string is {query_string}")
# print(f"content length is {content_length}")
# print(f"request method is {request_method}")
# print(f"post data is {post_data}")
# print("</p>")
# print("</body></html>")