import time
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.dates as mdates
import datetime
#JUly 9 at 910 turned to 68

# July 14 at 2107 turned to 70

def parseData(file):
    f = open(file)
    lines = f.readlines()

    temperatureData = np.zeros(len(lines))
    humidityData = np.zeros(len(lines))
    timeData = np.zeros(len(lines))
    count = 0
    for line in lines:
        temp,humid,timing = line.split(",")
        timing = timing.split("\n")[0]
        temperatureData[count] = float(temp)*(9/5) + 32
        humidityData[count] = float(humid)
        timeData[count] = float(timing)
        count += 1
    return temperatureData,humidityData,timeData

def convDataTime(arr):
    resArr = np.array([])
    for a in arr:
        res = datetime.datetime.fromtimestamp(a).strftime('%Y-%m-%d %H:%M:%S')
        resArr = np.append(resArr,res)
    return resArr


bedroomTemperatureData,bedroomHumidityData,bedroomTimeData = parseData("/home/alex/projects/AirConditionerEffectiveness/serverSide/bedroomData.csv")

basementTemperatureData,basementHumidityData,basementTimeData = parseData("/home/alex/projects/AirConditionerEffectiveness/serverSide/basementData.csv")

outsideTemperatureData,outsideHumidityData,outsideTimeData = parseData("/home/alex/projects/AirConditionerEffectiveness/serverSide/outsideWeatherData.csv")

midTemperatureData,midHumidityData,midTimeData = parseData("/home/alex/projects/AirConditionerEffectiveness/serverSide/midData.csv")

#bedroomDateTimeData = np.load("bedroomDateTimeData.npy")#convDataTime(bedroomTimeData)
#np.save("bedroomDateTimeData.npy",bedroomDateTimeData)
#print(bedroomDateTimeData)

#x = [datetime.datetime.strptime(d,'%Y-%m-%d %H:%M:%S').date() for d in bedroomDateTimeData]

#plt.plot(x,bedroomTemperatureData,label="NEW")
#plt.gca().xaxis.set_major_formatter(mdates.DateFormatter('%Y-%m-%d'))
#plt.gca().xaxis.set_major_locator(mdates.DayLocator())
#plt.gcf().autofmt_xdate()
timeSwitch = 1625865042
timeSwitch2 = 1626311278

plt.axvline(x=timeSwitch,label="set temperature change 69F to 68F",color="purple")
plt.axvline(x=timeSwitch2,label="set temperature change 68F to 70F",color="purple")
plt.plot(bedroomTimeData,bedroomTemperatureData,label="bedroom temperature in F")
plt.plot(basementTimeData,basementTemperatureData,label="basement temperature in F")
plt.plot(outsideTimeData,outsideTemperatureData,label="outside temperature in F")
plt.plot(midTimeData,midTemperatureData,label="mid level temperature in F")
plt.xlabel("time")
plt.ylabel("temperature F")
plt.title("temperature in F over time")
plt.legend()
plt.show()

print(np.average(bedroomTemperatureData))
print(np.average(basementTemperatureData))
print(np.average(outsideTemperatureData))
print(np.average(midTemperatureData))
