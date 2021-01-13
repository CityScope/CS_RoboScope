import socketio
import eventlet
import time
from threading import Thread
from flask import Flask, render_template
from engineio.payload import Payload
from grid_handling import GridHandling #computation methods

# initialize server
Payload.max_decode_packets = 500
sio = socketio.Server(async_mode='threading', cors_allowed_origins='*')

# initialize application wrapper for server
app = Flask(__name__)
app.wsgi_app = socketio.WSGIApp(sio, app.wsgi_app)
thread = None

# initialize GridHandling
gh = GridHandling()
time.sleep(2)

# continuous background reading task
def background_read():
    while True:
        node = gh.serialRead()
        if node:
            return_data = gh.serialReceive(node)
            sio.emit('roboscopeInput', return_data)
        time.sleep(1)

# on connect event handler (connects to port 8080 as does the web socket client in the Interface App)       
@sio.on('connect')
def connect(sid, environ):
    print('connected... ', sid)
    sio.emit("welcome", "connected to socket server...");
    global thread
    if thread is None:
        sio.start_background_task(target=background_read)
        
@sio.on('onInit')
def on_start(sid, features, properties):
    gh.tableStart(features, properties)

# receive data when 'brushing' edit grid events occur (height, color, ID of each pixel)
@sio.on('pixelUpdate')
def pixel_handling(sid, data):
    gh.serialSend(data)

# on scale or translation change, receive all features involved in change
@sio.on('gridUpdate')
def grid_handling(sid, scale, data):
    gh.setSelected(scale, data)
    
# on disconnect event handler
@sio.on('disconnect')
def disconnect(sid):
    print('disconnect ', sid)

if __name__ == '__main__':
    app.run(threaded=True, host='127.0.0.1', port=8080)
