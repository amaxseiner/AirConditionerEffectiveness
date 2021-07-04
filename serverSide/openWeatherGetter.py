# Python program to find current
# weather details of any city
# using openweathermap api

# import required modules
import requests, json,time

f = open("secret.txt","r")
# Enter your API key here
api_key = str(f.readline().split("\n")[0])

f.close()

# base_url variable to store url
base_url = "http://api.openweathermap.org/data/2.5/weather?"

# complete_url variable to store
# complete url address
complete_url = base_url + "&zip=22030,us&appid=" + api_key

# get method of requests module
# return response object

# json method of response object
# convert json format data into
# python format data

# Now x contains list of nested dictionaries
# Check the value of "cod" key is equal to
# "404", means city is found otherwise,
# city is not found

while(1):
	response = requests.get(complete_url)

	x = response.json()
	if x["cod"] != "404":

		# store the value of "main"
		# key in variable y
		y = x["main"]

		# store the value corresponding
		# to the "temp" key of y
		current_temperature = y["temp"] - 273.15 # in celcius


		# store the value corresponding
		# to the "humidity" key of y
		current_humidity = y["humidity"] # %
		f = open("outsideWeatherData.csv","a")
		f.write(str(current_temperature) + "," + str(current_humidity)+","+str(time.time())+"\n")
		f.close()
		time.sleep(100)

	else:
		print(" City Not Found ")
