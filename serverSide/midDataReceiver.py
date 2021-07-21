import socket
import time,sys
import requests, json

UDP_IP = "192.168.1.172"
UDP_PORT = 5005
MESSAGE = b"Hello, World!"

f = open("secret.txt","r")
# Enter your API key here
api_key = str(f.readline().split("\n")[0])

f.close()


print("UDP target IP: %s" % UDP_IP)
print("UDP target port: %s" % UDP_PORT)
print("message: %s" % MESSAGE)


sock = socket.socket(socket.AF_INET, # Internet
                     socket.SOCK_DGRAM) # UDP

sock.sendto(MESSAGE, (UDP_IP, UDP_PORT))

counter = 0

while True:
    data, addr = sock.recvfrom(1024) # buffer size is 1024 bytes
    synch,temp,humidity = str(data).split(" ")
    synch = synch.split("'")[1]
    if(str(synch) == "Data"):
        temp = float(temp)
        humidity = float(humidity.split("'")[0])

        f = open("midData.csv","a")
        f.write(str(temp) + "," + str(humidity)+","+str(time.time())+"\n")
        f.close()
        if(counter>100):
            print(".")
            counter=0

        counter += 1
