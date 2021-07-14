import eventlet
import socketio
import time
from threading import Thread
from threading import Lock
from flask import Flask, render_template
from engineio.payload import Payload
from grid_handling import GridHandling #computation methods

# initialize server
Payload.max_decode_packets = 500
sio = socketio.Server(async_mode='threading', cors_allowed_origins='*')

# initialize application wrapper for server
app = Flask(__name__)
app.wsgi_app = socketio.WSGIApp(sio, app.wsgi_app)
thread = None #thread for reading from table

# initialize GridHandling
gh = GridHandling()
time.sleep(2)

def background_read():
    """
    Continuous background reading from table and sends processed data to interface app
    """
    while True:
        data = gh.serialReceive()
        if data is not None:
            print(data)
            sio.emit('roboscopeInput', data)
        time.sleep(1)

@sio.on('connect')
def connect(sid, environ):
    """
    On connect event handler (connects to port 8080 as does the web socket client in the Interface App)
    """
    print('connected... ', sid)
    dim = gh.Utils.returnTableDimension()
    sio.emit("welcome", dim);
    global thread
    if thread is None:
        sio.start_background_task(target=background_read)

@sio.on('onInit')
def on_start(sid, features, properties):
    """
    Sends table dimensions to interface app every time interface app connects
    """
    print("onInit");
    dim = gh.Utils.returnTableDimension()
    sio.emit("tableDim", dim)
    gh.tableStart(features, properties) #start the physical controller

@sio.on('pixelUpdate')
def pixel_handling(sid, data):
    """
    Handles data received 'brushing' edit grid events occur (height, color, ID of each pixel)
    """
    # print('received data')
    gh.serialSend(data)

# on scale or translation change, receive all features involved in change
@sio.on('gridUpdate')
def grid_handling(sid, scale, data):
    """
    Handles data received when grid view is scaled or translated (all new features)
    """
    gh.setSelected(scale, data)

@sio.on('editUpdate')
def grid_handling(sid, editBool, type):
    """
    Handles changes in edit menu on interface app
    """
    gh.Interacts.setInteraction(editBool, type, gh.Utils)

@sio.on('disconnect')
def disconnect(sid):
    """
    Handles disconnects from server
    """
    print('disconnect ', sid)

if __name__ == '__main__':
    app.run(threaded=True, host='127.0.0.1', port=8080)
