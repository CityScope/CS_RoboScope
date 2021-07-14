import serial

class Translator():
    def __init__(self, serial_port='COM4', baud_rate=115200,
            read_timeout=5):
        '''
        Initialize connection to Translator Teensy Translator
        serial_port should match the port the Teensy is on
        '''

        try:
            self.conn = serial.Serial(serial_port, baud_rate)
        except serial.SerialException as err:
            print("error port")
            raise err
        else:
            print('Open Port')

        if self.conn.isOpen() == True:
            self.conn.timeout = read_timeout # Timeout for readline()
            self.conn.flushInput()
            self.conn.flushOutput()
            self.counter=0

    def write_pixel(self, pixel_data):
        """
        Sends all information of an updated pixel to the Translator
        The actual command sent is
            'W{node_id},
            {pixel_id},{height},{color 1},{color 2},{color 3}
            E'
        """
        self.counter += 1
        command="W"
        command+=str(pixel_data[0])
        command+=","
        command+=','.join(pixel_data[1:])
        command+="E"
        if self.conn.isOpen() == True:
            self.conn.write(command.encode())

    def write_node(self, node_data):
        """
        Sends all information of an updated pixel to the Translator
        The actual command sent is
            'W{node_id},
            {pixel_id},{height},{color 1},{color 2},{color 3} (x8)
            E'
        """
        self.counter+=1
        command="W"
        command+=str(node_data[0])
        command+=","
        command+=','.join(node_data[1:])
        command+="E"
        if self.conn.isOpen() == True:
            self.conn.write(command.encode())

    def write_multiple(self, pixel_data):
        """
        Sends all information of an updated pixel to the Translator
        The actual command sent is
            'T{number of nodes},
            {node_id},
            {interaction byte},{height},{color 1},{color 2}; (x8)
            {node_id},
            {interaction byte},{height},{color 1},{color 2}; (x8)
            ...
            E'
        """
        command="T"
        command+=str(len(pixel_data))
        command+=","
        for pixel in pixel_data:
            command+=str(pixel[0])
            command+=","
            command+=','.join(pixel[1:])
            command+=";"
        command+="E"
        if self.conn.isOpen() == True:
            self.conn.write(command.encode())


    def read_pixels(self):
        """
        Performs a read from the serial port, processes the data, and returns it as a list
        Once processed, output array looks like:
        [node_id, [pixel0: 4 bytes], [pixel1: 4 bytes], ...]
        """
        line_received = self.conn.readline().decode().strip()
        if line_received:
            vals = line_received.split(',')
            print(vals)
            output=[vals[0]]
            for i in range(1,len(vals[1:]), 4):
                output.append(vals[i:i+4])
            return output

        if self.conn.isOpen() == True:
            self.conn.flushInput()


    def close(self):
        """
        To ensure we are properly closing our connection to the
        Arduino device.
        """
        self.conn.close()
        print('Connection to Arduino closed')
