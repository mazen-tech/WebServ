import os
import sys

status = "Status: 500 server error"
qs = os.getenv("QUERY_STRING", "")

def insert_env(line):
    part_one = line.split('{{')[0]
    middle = line.split('{{')[1].split('}}')[0]
    part_two = line.split('}}')[1]
    middle = os.getenv(middle, "")
    return part_one + middle + part_two

def render(page, dir):
    with open(f"src/cgi/{dir}/{page}") as source:
        for line in source:
            if ('{{' in line and '}}' in line):
                line = insert_env(line)
            print(line)

def count_size(page, dir):
    all = 0
    body = ''
    with open(f"src/cgi/{dir}/{page}") as source:
        for line in source:
            if ('{{' in line and '}}' in line):
                line = insert_env(line)
            all += len(line.encode('utf-8'))
            body += line
    return all, body

html_pages = os.listdir(f'{os.getcwd()}/src/cgi/html')
css_pages = os.listdir(f'{os.getcwd()}/src/cgi/style')
page = sys.argv[1]

size = 0
if 'html' in page and page in html_pages:
    size, body = count_size(page, 'html')
elif 'css' in page and page in css_pages:
    size, body = count_size(page, 'style')
else:
    print("page not in pages")
    exit()

if 'css' in qs:
    size = len(body.encode('utf-8'))
    page = 'style.css'
    print(              "HTTP/1.1 200 OK\r\n"
                        "Content-Type: text/css\r\n"
                        "Content-Length: 847\r\n"
                        "Connection: close\r\n"
                        "\r\n")
else:
    size = len(body.encode('utf-8'))
    print(                  "HTTP/1.1 200 OK\r\n"
                            "Content-Type: text/html\r\n"
                            # f"Content-Length: {size}\r\n"
                            "Content-Length: 1000\r\n"
                            "Connection: close\r\n"
                            "\r\n")

if 'html' in page and page in html_pages:
    render(page, 'html')
elif 'css' in page and page in css_pages:
    render(page, 'style')


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
