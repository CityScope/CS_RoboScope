import math

class Utils:
    def __init__(self):
        self.GRID_COLS = 8
        self.GRID_ROWS = 12
        self.NUM_PIXELS = 96
        
        self.VIEW_COLS = 8
        self.VIEW_ROWS = 12
        self.VIEW_PIXELS = 96
        self.SCALE = 1
        
        self.TABLE_COLS = 8
        self.TABLE_ROWS = 12
        self.NODE_COLS = 2
        self.NODE_ROWS = 4
        self.NUM_NODES_ROW = 4

        self.CELL_SIZE = 40
        self.PX_SIZE = 40
        self.types = {}
        
        self.MAX_HEIGHT = 100
        self.MIN_HEIGHT = 0
    
    def setInitSettings(self, header, num_pixels):
        self.GRID_COLS = header["ncols"]
        self.GRID_ROWS = header["nrows"]
        self.CELL_SIZE = header["cellSize"]
        self.NUM_PIXELS = num_pixels
    
    def updateSettings(self, cols, rows, len_pixels, scale):
        self.VIEW_COLS = cols
        self.VIEW_ROWS = rows
        self.VIEW_PIXELS = len_pixels
        self.SCALE = scale
    
    def createPixelAssignment(self):
        pixelMapping = {}
        for row in range(self.TABLE_ROWS):
            for col in range(self.TABLE_COLS):
                node = int(col/self.NODE_COLS)+int(row/self.NODE_ROWS)*self.NUM_NODES_ROW
                local_id = (col%self.NODE_COLS)+((row%self.NODE_ROWS)*self.NODE_COLS)
                pixelMapping[(row, col)] = (node, local_id)
        self.pixel_assignment = pixelMapping
        return pixelMapping 
            
    def setColorMapping(self, types): 
        for i in types.keys():
            self.types[i] = {
                "color": self.hextoRGB(types[i]["color"]), 
                "meters": types[i].get("meters", 0)
            }
    
    def getHeight(self, type, height):
        if height is None: 
            return 0
        return height*self.types[type]["meters"]
        
    def hextoRGB(self, hex): 
        h = hex.lstrip('#')
        return [int(h[i:i+2], 16) for i in (0, 2, 4)]
    
    def getNextType(self, name):
        index = list(self.types.keys()).index(name)
        if index == len(list(self.types.keys()))-1:
            index = -1
        new_name =  list(self.types.keys())[index+1]
        return [new_name, self.types[new_name].color]        