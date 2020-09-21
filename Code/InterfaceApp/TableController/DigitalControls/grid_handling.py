import utils 
import math
import sys
# insert at 1, 0 is the script path (or '' in REPL)
sys.path.insert(1, '../PhysicalControls')
from main import PhysicalController

class GridHandling:
    def __init__(self):
        self.PC = None
        self.scale = 1
        self.Utils = Utils()
        self.selectedPixels = []
        self.pixel_to_coords = {}
        self.coords_to_pixels = {}

    def tableStart(self, features, properties):
        #reset total_grid and nodes
        self.Utils.setInitSettings(properties["header"],len(features))
        self.Utils.setColorMapping(properties["types"])
        self.PC = PhysicalController(
                    pixel_assignment=self.Utils.createPixelAssignment(), 
                    grid_size=(self.Utils.TABLE_ROWS, self.Utils.TABLE_COLS))
    
    def setSelected(self, scale, pixels):
        self.scale= scale
        self.selectedPixels = pixels
        self.Utils.updateSettings(int(self.Utils.TABLE_COLS*scale), 
                                  int(self.Utils.TABLE_ROWS*scale), 
                                  len(pixels))
        self.Utils.setMaxHeight(pixels)
            
        if scale==int(scale):
            x1, y1 = self.Utils.getGridCoords(pixels[0]["id"])
            for i in pixels:
                x, y = self.Utils.getGridCoords(i["id"])
                scaled_coords = (int((x-x1)/scale), int((y-y1)/scale))
                self.pixel_to_coords[i["id"]] = scaled_coords
                try:
                    self.coords_to_pixels[scaled_coords].append(i["id"])
                except KeyError:
                    self.coords_to_pixels[scaled_coords] = [i["id"]]
                if i["name"] != 'None':
                    self.PC.update_pixel(scaled_coords, self.Utils.getHeight(i['height']), i['color'])
            self.PC.send_table_data()
        
    def serialSend(self, pixel): 
        coords = self.pixel_assignment[pixel['id']]
        node_id = self.Utils.pixel_assignment[(coords[1], coords[0])][0] 
        self.PC.update_pixel(coords, pixel['height'], pixel['color'])
        self.PC.send_node_data(node_id)

    def serialReceive(self, pixels):  
        output = []
        node = int(pixels[0])
        for i in range(1, len(pixels)): 
            pixel = [int(j) for j in pixels[i]]
            id = self.PC.node_location[node][i-1]
            [name, color] = utils.getRGB888([pixel[2], pixel[3]])
            data = {'color': color, 'height': pixel[0], 'id': id, 'interactive': 'Web', 'name': name}
            updateTotal(data) 
            output.append(data)
        return output
        
class Utils:
    def __init__(self):
        self.GRID_COLS = 8
        self.GRID_ROWS = 12
        self.NUM_PIXELS = 96
        
        self.VIEW_COLS = 8
        self.VIEW_ROWS = 12
        self.VIEW_PIXELS = 96
        
        self.TABLE_COLS = 8
        self.TABLE_ROWS = 12
        self.NODE_COLS = 2
        self.NODE_ROWS = 4
        self.NUM_NODES_ROW = 4

        self.CELL_SIZE = 40
        self.MAX_HEIGHT = 50
        self.rgb888Dict = {}
        self.rgb565Dict = {}
    
    def setInitSettings(self, header, num_pixels):
        self.GRID_COLS = header["ncols"]
        self.GRID_ROWS = header["nrows"]
        self.CELL_SIZE = header["cellSize"]
        self.NUM_PIXELS = num_pixels
    
    def updateSettings(self, cols, rows, len_pixels):
        self.VIEW_COLS = cols
        self.VIEW_ROWS = rows
        self.VIEW_PIXELS = len_pixels
        
    def setColorMapping(self, types): 
        for i in types.keys():
            rgbColor = self.hextoRGB(types[i]["color"])
            color565 = self.RGB565(rgbColor)
            self.rgb888Dict[tuple(rgbColor)] = [i,color565]
            self.rgb565Dict[tuple(color565)] = [i, rgbColor]
    
    def setMaxHeight(self, pixels):
        self.MAX_HEIGHT = max(pixels, key= lambda p: p["height"])["height"]

    def getHeight(self, height):
        use_height = height
        if height is None: 
            use_height = 0
        return use_height/self.MAX_HEIGHT
    
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
        return rgb888Dict[tuple(color)][0]
        
    ## get rgb888 color that matches inputted rgb565 color
    def getRGB888(self, color): 
        absVal = 510
        index = 0
        for i in rgb565Dict.keys(): 
            tempabs = abs(i[0]-color[0]) + abs(i[1]-color[1])
            if tempabs < absVal:
                absVal = tempabs
                index = i        
        return rgb565Dict[index]
    
    ## get rgb565 color that matches inputted rgb888 color
    def getRGB565(color): 
        return rgb888Dict[tuple(color)][1]