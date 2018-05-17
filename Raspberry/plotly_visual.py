import plotly
import plotly.plotly as py
import plotly.graph_objs as go
import numpy as np
from dataFormat import DataModule
import math

plotly.tools.set_credentials_file(username='tomas_urban', api_key='bSTpWjykOzSVkASTSRMB')



class PlotlyVisual:



    @staticmethod
    def send_to_plotly(data_format: DataModule):
        # x, y, z = np.random.multivariate_normal(np.array([0,0,0]), np.eye(3), 400).transpose()
        z = data_format.mag.x

        trace1 = go.Scatter3d(
            x=data_format.mag.x,
            y=data_format.mag.y,
            z=data_format.mag.z,
            # x=x,
            # y=y,
            # z=z,
            mode='markers',
            marker=dict(
                size=8,
                color=z,                # set color to an array/list of desired values
                colorscale='Viridis',   # choose a colorscale
                opacity=0.8
            )
        )

        data = [trace1]
        print(data);
        layout = go.Layout(
            margin=dict(
                l=0,
                r=0,
                b=0,
                t=0
            )
        )
        fig = go.Figure(data=data, layout=layout)
        py.plot(fig, filename='3d-scatter-colorscale')


    @staticmethod
    def send_compass_data(data_format: DataModule):

        traceX = go.Scatter(
            x=data_format.mag.x,
            y=np.asarray(range(0, len(data_format.mag.x))),
            mode='lines+markers',
            name='X'
        )

        traceY = go.Scatter(
            x=data_format.mag.y,
            y=np.asarray(range(0, len(data_format.mag.y))),
            mode='lines+markers',
            name='Y'
        )

        traceZ = go.Scatter(
            x=data_format.mag.z,
            y=np.asarray(range(0, len(data_format.mag.z))),
            mode='lines+markers',
            name='Z'
        )

        data = [traceX, traceY, traceZ]
        py.plot(data, filename='compassData')

    @staticmethod
    def send_heading(heading):
        trace1 = go.Scatter(
            x=heading,
            y=np.asarray(range(0, len(heading))),
            mode='lines+markers',
            name='degrees'
        )

        data = [trace1]
        # print(data)
        py.plot(data, filename='compassDegress')


    @staticmethod
    def calculate_compass(data_format: DataModule):
        x = data_format.mag.x
        y = data_format.mag.y
        headingDegrees = []

        for i in range(0, len(x)):
                heading = (math.atan2(int(y[i]),int(x[i]))*180)/math.pi
                if heading <0:
                    heading = heading + 360
                headingDegrees.append(heading)
        PlotlyVisual.send_heading(headingDegrees)
        # print(headingDegrees)
