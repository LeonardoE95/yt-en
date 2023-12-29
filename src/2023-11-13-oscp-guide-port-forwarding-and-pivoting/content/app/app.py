#!/usr/bin/env python3

from flask import Flask

app = Flask(__name__)
port = 1337

@app.route('/')
def root():
    return 'Welcome to the app'

# ----------------------

def main():
    app.run(debug=False, host="127.0.0.1", port=1337)

if __name__ == '__main__':
    main()

