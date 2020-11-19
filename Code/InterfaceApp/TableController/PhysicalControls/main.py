from serial_lib import *
import numpy as np


class PixelData:
    def __init__(self, node=None, pixel=None, color=(0,0,0), height=0):
        self.node=node
        self.pixel=pixel
        self.color=color
        self.height=height

    def get_serial_data(self):
        color565 = self.RGB565(self.color)
        return [str(self.node),
                str(self.pixel),
                str(self.height),
                str(color565[0]),
                str(color565[1])]

    def RGB565(self, color):
        val = ("%0.4X" % ( ((color[0] & 0xf8)<<8) + ((color[1] & 0xfc)<<3)+(color[2]>>3)))
        output = [int(val[0:2], 16), int(val[2:4], 16)]
        return output

    def update_pixel(self, height, color):
        self.update_height(height)
        self.update_color(color)

    def update_height(self, height):
        self.height=height

    def update_color(self, color):
        self.color=color

    def current_status(self):
        return (self.color, self.height)

class PhysicalController:
    def __init__ (self, pixel_assignment, grid_size=(4, 2), com_port = 'COM5', BAUDRATE=115200):
        self.grid_size = grid_size
        self.total_pixel_count = grid_size[0] * grid_size[1]

        # Where each pixel data is stored
        self.total_grid = [[None for col in range(grid_size[1])] for row in range(grid_size[0])]

        # Stores which pixel location corresponds to each node
        # The key of node_location represents the node_id
        # The list at each index represents where in the grid that pixel is (row, col)
        self.node_location = {}

        self.assign_nodes(pixel_assignment)
        self.serial_com = Translator(com_port, BAUDRATE)
        # reference as y,x
        # self.display_table()
        # self.send_pixel_data((0,0))

    def display_table(self):
        for row in self.total_grid:
            row_data = []
            for pixel in row:
                if pixel == None:
                    continue
                row_data.append(pixel.current_status())
            print(row_data)

    def assign_nodes(self, pixel_assignment):
        for (row,col) in pixel_assignment:
            (node_id, pixel_id) = pixel_assignment[(row,col)]
            self.total_grid[row][col] = PixelData(node=node_id, pixel=pixel_id)
            try:
                self.node_location[node_id].append((row, col))
            except KeyError:
                self.node_location[node_id] = [(row, col)]

    def read_pixel_data(self):
        return self.serial_com.read_pixels()
    # Takes in a position as (x,y)
    def update_pixel(self, pos, height, color):
        (col, row) = pos
        self.total_grid[row][col].update_pixel(height, color)

    def send_pixel_data(self, pos):
        (col, row) = pos
        pixel_data = self.total_grid[row][col].get_serial_data()
        self.serial_com.write_pixel(pixel_data)

    def send_node_data(self, node_id):
        node_data = [str(node_id)]
        for (row, col) in self.node_location[node_id]:
            node_data.extend(self.total_grid[row][col].get_serial_data()[1:])
        self.serial_com.write_node(node_data)

    def send_table_data(self):
        table_data = []
        for node_id in self.node_location:
            temp = [str(node_id)]
            for (row, col) in self.node_location[node_id]:
                temp.extend(self.total_grid[row][col].get_serial_data()[1:])
            table_data.append(temp)
        self.serial_com.write_multiple(table_data)
        return table_data



if __name__ == '__main__':
    # physicalController.run();
    pixel_assignment = {(0,0): (0,0), (0,1): (0,1),
                        (1,0): (0,2), (1,1): (0,3),
                        (2,0): (0,4), (2,1): (0,5),
                        (3,0): (0,6), (3,1): (0,7)}

    physicalController = PhysicalController(pixel_assignment=pixel_assignment)

    while (True):
        print("Sending data:")
        tb = physicalController.send_table_data()
        print(tb)
        print("Receiving data:")
        print(physicalController.serial_com.read_pixels())
    pass
