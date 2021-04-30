import math
import json

class Utils:
    def __init__(self):
        '''
        Initializer. Table properties from CONFIG file and sets up interface app properties
        ''' 
        f = open('config.json')
        config = json.load(f)
        #TABLE
        self.TABLE_COLS = config["TABLE_COLS"]
        self.TABLE_ROWS = config["TABLE_ROWS"]
        self.NODE_COLS = config["NODE_COLS"]
        self.NODE_ROWS = config["NODE_ROWS"]
        self.NUM_NODES_ROW = config["TABLE_COLS"]/config["NODE_COLS"]
        self.PX_SIZE = config["PX_SIZE"]
        self.TRAVEL_DIST = config["TRAVEL_DIST"]

        #WEB APP
        self.GRID_COLS = 8
        self.GRID_ROWS = 12
        self.NUM_PIXELS = 96

        self.VIEW_COLS = 8
        self.VIEW_ROWS = 12
        self.VIEW_PIXELS = 96
        self.SCALE = 1
        self.CELL_SIZE = 40
        self.types = {}    
    
    def setInitSettings(self, header, num_pixels):
        '''
        Updates grid properties from Interface App
        ''' 
        self.GRID_COLS = header["ncols"]
        self.GRID_ROWS = header["nrows"]
        self.CELL_SIZE = header["cellSize"]
        self.NUM_PIXELS = num_pixels
    
    def updateSettings(self, cols, rows, len_pixels, scale):
        '''
        Updates zoom view properties from Interface App
        ''' 
        self.VIEW_COLS = cols
        self.VIEW_ROWS = rows
        self.VIEW_PIXELS = len_pixels
        self.SCALE = scale
    
    def createPixelAssignment(self):
        '''
        Creates Table pixel Assignment  
        - pixel_assignment: (row, col) --> (node, local_id [0-7])
        ''' 
        pixelMapping = {}
        for row in range(self.TABLE_ROWS):
            for col in range(self.TABLE_COLS):
                node = int(col/self.NODE_COLS)+int(row/self.NODE_ROWS)*self.NUM_NODES_ROW
                local_id = (col%self.NODE_COLS)+((row%self.NODE_ROWS)*self.NODE_COLS)
                pixelMapping[(row, col)] = (node, local_id)
        self.pixel_assignment = pixelMapping
        return pixelMapping 
            
    def setColorMapping(self, types): 
        '''
        Creates map from land usage types to its color and height  
        ''' 
        for i in types.keys():
            self.types[i] = {
                "name": i,
                "color": self.hextoRGB(types[i]["color"]), 
                "meters": types[i].get("meters", 0)
            }
    
    def getHeight(self, type, floors):
        '''
        Calculates proper height of table pixel based on number of floors of the
        cell and height of land usage
        - (# floors * land type height)                X
          _____________________________  =  ________________________
                length of cell              table pixel size * scale
        - returns float in range [0,1]
        ''' 
        if floors is None:
            return 0
        tableHeight = (floors*self.types[type]["meters"]*self.PX_SIZE*self.SCALE)/math.sqrt(self.CELL_SIZE)
        if tableHeight > self.TRAVEL_DIST:
            return 1
        return tableHeight/self.TRAVEL_DIST
                
    def hextoRGB(self, hex): 
        '''
        Converts hex color to RGB
        '''
        h = hex.lstrip('#')
        return [int(h[i:i+2], 16) for i in (0, 2, 4)]
    
    def getNextType(self, name):
        index = list(self.types.keys()).index(name)
        if index == len(list(self.types.keys()))-1:
            index = -1
        new_name =  list(self.types.keys())[index+1]
        return [new_name, self.types[new_name].color]   
    
    def returnTableDimension(self):
        '''
        Returns table dimensions
        '''
        return [self.TABLE_COLS, self.TABLE_ROWS]   