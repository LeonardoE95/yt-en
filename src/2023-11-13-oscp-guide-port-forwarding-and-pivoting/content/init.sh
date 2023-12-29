#!/usr/bin/env sh

nohup python3 /app/app.py >/dev/null 2>&1 &
/usr/sbin/sshd -D
