import time

class Measurement:
    x = []
    y = []
    z = []

    def __init__(self):
        self.x = []
        self.y = []
        self.z = []

class DataModule:
    title = 0
    acc = Measurement()
    mag = Measurement()
    timestamp = 0

    def __init__(self):
        self.title = "title"
        self.acc = Measurement()
        self.mag = Measurement()
        self.timestamp = time.time()

