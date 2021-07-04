import socket
import time,sys
import requests, json

UDP_IP = "192.168.1.171"
UDP_PORT = 5005
MESSAGE = b"Hello, World!"


print("UDP target IP: %s" % UDP_IP)
print("UDP target port: %s" % UDP_PORT)
print("message: %s" % MESSAGE)
f = open("secret.txt","r")
# Enter your API key here
api_key = str(f.readline().split("\n")[0])

f.close()

base_url = "http://api.openweathermap.org/data/2.5/weather?"

complete_url = base_url + "&zip=22030,us&appid=" + api_key


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

        response = requests.get(complete_url)

        f = open("basementData.csv","a")
        f.write(str(temp) + "," + str(humidity)+","+str(time.time())+"\n")
        f.close()
        if(counter>100):
            print(".")
            counter=0

        counter += 1
