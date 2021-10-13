import socket

s = socket.socket()

# nc 192.168.0.53 30003
host = '192.168.0.53'
port = 30003

s.connect((host, port))
while True:
    print (s.recv(1024))