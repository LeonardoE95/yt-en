#!/usr/bin/env python3

import requests

PROXIES = { "http": "http://localhost:8080", "https": "https://localhost:8080" }

# -----------------------------

def get_examples():
    global PROXIES

    # simple GET
    url = "http://localhost/get"
    requests.get(url, proxies=PROXIES)

    # GET with custom PATH PARAMETERS
    url = "http://localhost/params"
    custom_params = { "key1": "value1", "key2": "value2"}
    requests.get(url, params=custom_params, proxies=PROXIES)

    # GET with custom HEADERS
    url = "http://localhost/header"
    custom_headers = {
        "User-Agent": "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/126.0.0.0 Safari/537.36"
    }
    requests.get(url, headers=custom_headers, proxies=PROXIES)

    # GET with custom HEADERS and custom PATH PARAMETERS
    url = "http://localhost/general"
    custom_headers = { "User-Agent": "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/126.0.0.0 Safari/537.36" }    
    custom_params = { "key1": "value1", "key2": "value2"}
    requests.get(url, params=custom_params, headers=custom_headers, proxies=PROXIES)

def post_examples():
    global PROXIES

    # simple POST
    url = "http://localhost/post"
    requests.post(url, proxies=PROXIES)

    # POST with 'x-www-form-urlencoded' body
    url = "http://localhost/form"
    custom_body_params = {"key1": "value1", "key2": "value"}
    requests.post(url, data=custom_body_params, proxies=PROXIES)

    # POST with custom HEADERS
    url = "http://localhost/post-headers"
    custom_headers = {
        "User-Agent": "XYZ"
    }    
    requests.post(url, headers=custom_headers, proxies=PROXIES)

    # POST with JSON body
    url = "http://localhost/json"
    custom_json_body = { "key1": "value1", "key2": "value" }
    requests.post(url, json=custom_json_body, proxies=PROXIES)    

def response_examples():
    global PROXIES    

    url = "http://localhost/index.php"
    phpsessid = "pdse1s5d2vm47b07vk1579jr87"
    custom_headers = {
        "Cookie": f"PHPSESSID={phpsessid}; security=low"
    }
    r = requests.get(url, headers=custom_headers, proxies=PROXIES)

    status_code = r.status_code
    response_headers = r.headers
    server_header = r.headers["Server"]
    response_body = r.text

    check_condition = "Welcome to Damn Vulnerable Web Application!" in response_body

    print(check_condition)    

# -----------------------------

def main():
    get_examples()
    post_examples()
    response_examples()

if __name__ == "__main__":
    main()
