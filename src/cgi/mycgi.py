import os
import sys
from datetime import datetime, timedelta

data = sys.stdin.read()  # Odczyt całej zawartości ze stdin
print("Wprowadzone dane:")
print(data)
exit()

def parse_args(qs):
    if '?' not in qs:
        return
    args_string = qs.split('?')[1]
    args = args_string.split('&')
    for a in args:
        a = a.split('=')
        os.environ[a[0]] = a[1]

qs = os.getenv("QUERY_STRING", "")
parse_args(qs)
page = sys.argv[1].replace('.py', '.html')

def find_party():
    host_bday = datetime.strptime(os.getenv("your_bday"), "%Y-%m-%d").date()
    user_bday = datetime.strptime(os.getenv("friend_bday"), "%Y-%m-%d").date()
    if host_bday > user_bday:
        distance = (host_bday - user_bday).days
        if distance > 183:
            best_date = host_bday + timedelta(days=int((365 - distance)/2))
        else:
            best_date = host_bday - timedelta(days=int(distance/2))
    else:
        distance = (user_bday - host_bday).days
        if distance > 183:
            best_date = user_bday + timedelta(days=int((365 - distance)/2))
        else:
            best_date = user_bday - timedelta(days=int(distance/2))
    if best_date < datetime.today().date():
        best_date += timedelta(days=365)
    return str(best_date)

if page == 'bday_share.html':
    os.environ['shared_bd'] = find_party()
html_pages = os.listdir(f'{os.getcwd()}/src/cgi/html')
css_pages = os.listdir(f'{os.getcwd()}/src/cgi/style')

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


if 'html' in page and page in html_pages:
    render(page, 'html')
elif 'css' in page and page in css_pages:
    render(page, 'style')
