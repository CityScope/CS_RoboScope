import math
import sys
import time
# insert at 1, 0 is the script path (or '' in REPL)
sys.path.insert(1, '../PhysicalControls')
from main import PhysicalController
from collections import Counter 
from colour import Color
from utils import Utils

class GridHandling:
    def __init__(self):
        """
        Initializer. Sets up PhysicalController, Utils, Interacts, and connected translator
        """
        self.PC = None
        self.scale = 1
        self.Utils = Utils()
        self.com_port = '/dev/cu.usbmodem6893170'

    def tableStart(self, features, properties):
        """
        On interface app connect: resets table properties (rows, columns, 
        land usage, etc) and creates PC
        Parameters: 
        - features: list of features from interface app (dictionaries)
        - properties: header from interface app
        """
        self.Utils.setInitSettings(properties["header"],len(features))
        self.Utils.setColorMapping(properties["types"])
        self.PC = PhysicalController(
                    pixel_assignment=self.Utils.createPixelAssignment(),
                    grid_size=(self.Utils.TABLE_ROWS, self.Utils.TABLE_COLS),
                    com_port =self.com_port, BAUDRATE=2000000)
    
    def setSelected(self, scale, pixels):
        '''
        Handle updates from interface app on scale or translate change
        Parameters: scale factor (int), pixels (list of features)
        - table_to_grid: (col, row) --> pixel data
        - ids_to_table: pixel id --> [(col, row), ...]
        '''
        #update settings, pixels, scale, and range of heights
        self.scale= scale
        self.Utils.updateSettings(int(self.Utils.TABLE_COLS/scale), 
                                   int(self.Utils.TABLE_ROWS/scale), 
                                   len(pixels), self.scale)
        # iterate through cells
        self.table_to_grid = {}
        self.ids_to_table = {}
        for y in range(0,self.Utils.VIEW_ROWS):
            for x in range(0, self.Utils.VIEW_COLS):
                #cell corresponding to (x,y)
                cell_index = y*self.Utils.VIEW_COLS+x 
                p = pixels[cell_index]
                #table pixels corresponding to cell
                table_coords = [(x*scale+i, y*scale+j) for i in range(scale) for j in range(scale)] #all
                #update with height and color
                [height, color] = self.colorAndHeight(p)
                for tc in table_coords: 
                    self.PC.update_pixel(tc, height, color)
                    self.table_to_grid[tc] = p
                self.ids_to_table[p["id"]] = table_coords
        #send data
        self.PC.send_table_data()
                                            
    def serialSend(self, in_px): 
        '''
        Handles brush events from the interface app
        Parameters: one feature (dictionary)
        '''
        table_coords = self.ids_to_table[in_px['id']]
        nodes = set()
        [height, color] = self.colorAndHeight(in_px)
        
        for tc in table_coords:
            self.PC.update_pixel(tc, height, color)
            self.table_to_grid[tc] = in_px
            nodes.add(self.Utils.pixel_assignment[(tc[1], tc[0])][0])
            
        for n in nodes:
            self.PC.send_node_data(n)
        
    def serialReceive(self): 
        '''
        Handles data received from table
        ''' 
        if self.PC:
            pixels = self.PC.read_pixel_data()
            if pixels: 
                output = []
                node = self.PC.node_location[int(pixels[0])]
                for i in range(1, len(pixels)): 
                    p = self.RXPixelInfo(pixels[i])
                    if p is None:
                        return None
                    (row, col) = node[i-1]
                    if sum(p['inter']) != 0:
                        print(p['inter'])
                        if p['inter'][0] == 1:
                            print("short press")
                        if p['inter'][1] == 1:
                            print("long press")
                        if p['inter'][len(p['inter'])-1]==1:
                            print("double tap")
                            for i in range(len(self.table_to_grid[(col, row)])):
                                if self.table_to_grid[(col, row)][i].get("interactive")!= None:
                                    [name, color] = self.Utils.getNextType(self.table_to_grid[(col, row)][i]["name"])
                                    data = {'color': color, 'height': 10, 'id': self.table_to_grid[(col, row)][i]['id'], 'interactive': 'Web', 'name': name}
                                    self.table_to_grid[(col, row)][i] = data
                                    self.PC.update_pixel((col,row), 10, color)
                                    output.append(data)      
                self.PC.send_node_data(int(pixels[0]))         
                return output    
        return None
    
    def RXPixelInfo(self, pix):
        '''
        Check to make sure data was received properly from table
        Formats pixel into dictionary
        ''' 
        try: 
            p = [int(j) for j in pix]
            p = {
                'information': p[0],
                'pos': p[1],
                'inter': [int(x) for x in bin(p[2])[2:]],
                'state': p[3]
            }
            if len(p['inter'])!=8:
                print(len(p['inter']))
                for i in range(8-len(p['inter'])):
                    p['inter'].insert(0,0)
            return p
        except: 
            return None 
        
    def colorAndHeight(self, p):
        '''
        Returns proper height and color for table pixel
        Input: one pixel (dictionary)
        ''' 
        new_c = p["color"]
        if p.get("interactive")==None:
            color = Color(rgb=tuple([c/255 for c in p["color"]]))
            color.luminance=0.2
            new_c = [int(c*255) for c in color.rgb]
        return [self.Utils.getHeight(p["name"], p["height"]), new_c]