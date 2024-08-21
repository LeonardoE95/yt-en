#!/usr/bin/env python3

import json

import requests
from bs4 import BeautifulSoup

# -----------------------------

def extract_links_from_blog():
    # make HTTP request to obtain the HTML data
    url = "https://blog.leonardotamiano.xyz/tech/"
    r = requests.get(url)

    # parse the HTML data into a soup object
    soup = BeautifulSoup(r.text, "html.parser")

    # now we can query the document and extract the parts we're
    # interested in.
    articles = soup.find_all("article")

# <article class="archive-link">
#   <time datetime="2024-07-22">2024.07.22</time>
#   -
#   <header style="display: inline;"><a href="https:&#x2F;&#x2F;blog.leonardotamiano.xyz&#x2F;tech&#x2F;android-emulation-setup&#x2F;">Android Emulation Setup</a></header>
# </article>
    
    data = []
    for article in articles:
        pubblication_date = article.find("time").text
        link = article.find("a")['href']
        title = article.find("a").text

        data.append({
            "title": title,
            "link": link,
            "date": pubblication_date
        })

    print(json.dumps(data, indent=2))

# -----------------------------

def main():
    extract_links_from_blog()

if __name__ == "__main__":
    main()
