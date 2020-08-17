import socketio
import eventlet
import time
from threading import Thread
from flask import Flask, render_template
from engineio.payload import Payload

from serial_lib import * #serial communication 
import grid_handling as gh #computation methods

# initialize server
Payload.max_decode_packets = 500
sio = socketio.Server(async_mode='threading', cors_allowed_origins='*')

# initialize application wrapper for server
app = Flask(__name__)
app.wsgi_app = socketio.WSGIApp(sio, app.wsgi_app)

thread = None

# initialize serial communication
t = Translator()
time.sleep(2)

# continuous background reading task
def background_read():
    while True:
        node = t.read_pixels()
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
    print(len(features))
    table_data = gh.tableStart(features, properties)
    t.write_multiple(table_data)
    time.sleep(2)
    [teensy_output, IA_output] = gh.waveTest(6)
    for i in range(len(teensy_output)):
        t.write_multiple(teensy_output[i])
        sio.emit('waveTest', IA_output[i])
        time.sleep(0.1)

# receive output from Interface App when 'brushing' edit grid events occur (height, color, ID of each pixel)
@sio.on('roboscopeOutput')
def update_handling(sid, data):
    pixel_data = gh.serialSend(data)
    t.write_node(pixel_data)

# on disconnect event handler
@sio.on('disconnect')
def disconnect(sid):
    print('disconnect ', sid)

if __name__ == '__main__':
    # run app on port 8080
    app.run(threaded=True, host='127.0.0.1', port=8080)
