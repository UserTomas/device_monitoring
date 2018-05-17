import serial
import threading
import logging
import datetime
from APIrequests import Request
from dataFormat import DataModule
from plotly_visual import PlotlyVisual

logging.basicConfig(level=logging.DEBUG,
                    format='[%(levelname)s] (%(threadName)-10s) %(message)s',
                    )


# thread function
def worker(data):
    logging.debug('Starting')
    request = Request()
    request.sendPost(data)
    logging.debug('Exiting')


try:
    ser = serial.Serial('/dev/rfcomm0', 9600, timeout = 1) # ttyACM1 for Arduino board

    readOut = 0   #chars waiting from laser range finder

    print ("Starting up")
    connected = False
    commandToSend = 1 # get the distance in mm

    dataArray = []


    ser.isOpen()
    active = False
    print ("port is opened!!!")

except IOError: # if port is already opened, close it and open it again and print message
    ser.close()
    ser.open()
    print ("port was already open, was closed and opened again!")

# while True:
#     readOut = ser.readline().decode('ascii')
#     print ("Reading:",readOut)
#     if readOut == 'S\r\n':
#         print("start!!!")
#         active = True
#         continue
#     elif readOut == 'E\r\n':
#         print("Stop.")
#         active = False
#
#     if active:
#         readOut.replace("\r\n","")
#         actualTime = str(round(datetime.datetime.now().timestamp(),2))
#         readOut += ',' + actualTime
#         dataArray.append(readOut)
#     if len(dataArray) > 19:
#         dataDictionary = {"values": dataArray}
#         t = threading.Thread(target=worker, args=(dataDictionary,))
#         t.start()
#
#         dataArray = []
#
# print ("Restart")
# ser.flush() #flush the buffer

#plotly
plotly = PlotlyVisual()
sensor_data = DataModule()
while True:
    readOut = ser.readline().decode('ascii')
    print ("Reading:",readOut)
    if readOut == 'S\r\n':
        print("start!!!")
        active = True
        continue
    elif readOut == 'E\r\n':
        print("Stop.")
        active = False
        # plotly.send_to_plotly(sensor_data)
        plotly.send_compass_data(sensor_data)
        # plotly.calculate_compass(sensor_data)
        sensor_data = 0
    if active:

        readOut = readOut.replace("\r\n","")
        aX, aY, aZ, mX, mY, mZ = readOut.split(",")
        sensor_data.mag.x.append(mX)
        sensor_data.mag.y.append(mY)
        sensor_data.mag.z.append(mZ)


print ("Restart")
ser.flush() #flush the buffer

