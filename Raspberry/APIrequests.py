# importing the requests library
import requests
import json
import logging
import datetime

logging.basicConfig(level=logging.DEBUG,
                    format='[%(levelname)s] (%(threadName)-10s) %(message)s',
                    )

class Request:
    # defining the api-endpoint
    API_ENDPOINT = "http://192.168.0.11:3000/listnotes"

    # your API key here
    API_KEY = "XXXXXXXXXXXXXXXXX"

    acc =  {'aX': [],
            'aY': [],
            'aZ': []}

    mag = {'mX': [],
           'mY': [],
           'mZ': []}

    timestamp = '1000111111'

    # 'api_paste_format': 'python',
    # data to be sent to api
    data = {'title': 'msg from node number correct',
            'acc': acc,
            'mag': mag,
            'timestamp': timestamp}


    def sendPost(self,data):
        listValues = data['values']
        actualTime = str(round(datetime.datetime.now().timestamp(),2))
        dataPackage = {'title': 'meranickoRanicko', 'timestamp': actualTime, 'meranie': []}
        listValues = [x.replace("\r\n","") for x in listValues]
        for i in range(len(listValues)):
            aX, aY, aZ, mX, mY, mZ, actualTime = listValues[i].split(",")
            acc = {'aX': aX,
                    'aY': aY,
                    'aZ': aZ}

            mag = {'mX': int(mX),
                    'mY': int(mY),
                    'mZ': int(mZ)}

            # 'api_paste_format': 'python',
            # data to be sent to api
            dataDictionary = {'acc': acc, 'mag': mag, 'timestamp': actualTime}
            dataPackage['meranie'].append(dataDictionary)

        # sending post request and saving response as response object
        # r = requests.post(url=Request.API_ENDPOINT, json=Request.data)
        # logging.debug("Sending Request with: %i elements" % len(dataDictionary))

        r = requests.post(url=Request.API_ENDPOINT, json=dataPackage)

        # extracting response text
        pastebin_url = r.text
        # logging.debug("The pastebin URL is:%s" % pastebin_url)
