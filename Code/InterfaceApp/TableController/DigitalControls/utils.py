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
        self.FLOOR_HEIGHT = 5
        self.rgb888Dict = {}
        self.rgb565Dict = {}
    
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
        
    def setColorMapping(self, types): 
        for i in types.keys():
            rgbColor = self.hextoRGB(types[i]["color"])
            color565 = self.RGB565(rgbColor)
            self.rgb888Dict[tuple(rgbColor)] = [i,color565]
            self.rgb565Dict[tuple(color565)] = [i, rgbColor]
    
    def getHeight(self, height):
        new_pixel_dim = math.sqrt(self.CELL_SIZE)*self.SCALE
        use_height = round((height*self.FLOOR_HEIGHT*2.5)/new_pixel_dim, 3)
        if height is None: 
            use_height = 0
        return math.ceil(use_height)
    
    def createPixelAssignment(self):
        pixelMapping = {}
        for row in range(self.TABLE_ROWS):
            for col in range(self.TABLE_COLS):
                node = int(col/self.NODE_COLS)+int(row/self.NODE_ROWS)*self.NUM_NODES_ROW
                local_id = (col%self.NODE_COLS)+((row%self.NODE_ROWS)*self.NODE_COLS)
                pixelMapping[(row, col)] = (node, local_id)
        self.pixel_assignment = pixelMapping
        return pixelMapping
    
    def getGridCoords(self, id): 
        return (id%self.GRID_COLS, int(id/self.GRID_COLS))   
        
    ##get rgb888 color of inputted hex color
    def hextoRGB(self, hex): 
        h = hex.lstrip('#')
        return [int(h[i:i+2], 16) for i in (0, 2, 4)]

    ##get rgb565 color of inputted rgb888 color
    def RGB565(self, color):
        val = ("%0.4X" % ( ((color[0] & 0xf8)<<8) + ((color[1] & 0xfc)<<3)+(color[2]>>3)))
        output = [int(val[0:2], 16), int(val[2:4], 16)]
        return output
    
    ## get name of type that matches inputted rgb888 color
    def getNameofColor(self, color): 
        return self.rgb888Dict[tuple(color)][0]
        
    ## get rgb888 color that matches inputted rgb565 color
    def getRGB888(self, color): 
        absVal = 510
        index = 0
        for i in self.rgb565Dict.keys(): 
            tempabs = abs(i[0]-color[0]) + abs(i[1]-color[1])
            if tempabs < absVal:
                absVal = tempabs
                index = i        
        return self.rgb565Dict[index]
    
    ## get rgb565 color that matches inputted rgb888 color
    def getRGB565(color): 
        return self.rgb888Dict[tuple(color)][1]