from APIrequests import Request
import logging
import threading
import requests
import datetime
import time
from threading import Thread
from plotly_visual import PlotlyVisual


# plotly = PlotlyVisual()
# plotly.send_to_plotly()

logging.basicConfig(level=logging.DEBUG,
                    format='[%(levelname)s] (%(threadName)-10s) %(message)s',
                    )

data = ['128888,192,16768,-200,-431,-483\r\n','888,192,122,-200,-431,8999\r\n','66666,192,16768,-200,-431,-4444\r\n']
dataDictionary = {"values": data}


x = '128888,192,16768,-200,-431,-483\r\n'

x = x.replace("\r\n","")
actualTime = str(datetime.datetime.now().timestamp())
x += ','+ actualTime
# print(x)
# def worker(data):
#     print("start!!")
#     logging.debug('Starting')
#     time.sleep(2)
#     # logging.debug(data)
#     # request = Request()
#     # request.sendPost(data)
#
#     # defining the api-endpoint
#     API_ENDPOINT = "http://192.168.0.11:3000/listnotes"
#
#     listValues = data['values']
#
#     dataPackage = {'title': 'meranickoRanicko', 'meranie': []}
#     listValues = [x.replace("\r\n", "") for x in listValues]
#     for i in range(len(listValues)):
#         aX, aY, aZ, mX, mY, mZ = listValues[i].split(",")
#         acc = {'aX': aX,
#                'aY': aY,
#                'aZ': aZ}
#
#         mag = {'mX': int(mX),
#                'mY': int(mY),
#                'mZ': int(mZ)}
#
#         timestamp = '1000111111'
#
#     # 'api_paste_format': 'python',
#     # data to be sent to api
#     dataDictionary = {'acc': acc, 'mag': mag, 'timestamp': timestamp}
#     dataPackage['meranie'].append(dataDictionary)
#
#     # sending post request and saving response as response object
#     # r = requests.post(url=Request.API_ENDPOINT, json=Request.data)
#     print("Sending Request!!!")
#     # payload = json.dumps(dataDictionary)
#     # print(payload)
#     r = requests.post(url=Request.API_ENDPOINT, json=dataPackage)
#
#     # extracting response text
#     pastebin_url = r.text
#     print("The pastebin URL is:%s" % pastebin_url)
#
#     def sayHi(self):
#         print("Hello")
#     logging.debug('Exiting')
#
# #
# # def sleeper(i):
# #     logging.debug('Starting')
# #     print ("thread %d sleeps for 5 seconds" % i)
# #     time.sleep(5)
# #     print ("thread %d woke up" % i)
# #
# # for i in range(1):
# #     print("i",i)
# #     t = Thread(target=worker, args=(dataDictionary,))
# #     # t = Thread(target=sleeper, args=(i,))
# #     t.start()
# # print("pokracujem")
#
#
# # for i in range(10):
# #     print("i:", i)
# #     t = Thread(target=sleeper, args=(i,))
# #     t.start()