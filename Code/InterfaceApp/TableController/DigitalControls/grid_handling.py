import math
import sys
# insert at 1, 0 is the script path (or '' in REPL)
sys.path.insert(1, '../PhysicalControls')
from main import PhysicalController
from collections import Counter 

class GridHandling:
    def __init__(self):
        self.PC = None
        self.scale = 1
        self.Utils = Utils()

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
                                  len(pixels), scale)
        self.table_to_grid = {}
        self.ids_to_table = {}
        x1, y1 = self.Utils.getGridCoords(pixels[0]["id"])
        for x,y,table_coord,r_scale in self.get_coords_loop(self.Utils.VIEW_ROWS, self.Utils.VIEW_COLS, scale):
            cell_coords = [(x+i,y+j, (y+j)*self.Utils.VIEW_COLS+(x+i)) for i in range(r_scale) for j in range(r_scale) if x+i < self.Utils.VIEW_COLS and y+j < self.Utils.VIEW_ROWS]
            pixel_group = [pixels[scaled_id] for a,b,scaled_id in cell_coords]
            color = Counter([tuple(pixel["color"]) for pixel in pixel_group]).most_common(1)[0][0]
            avg_height = sum([pixel["height"] for pixel in pixel_group])/len(pixel_group)
            self.PC.update_pixel(table_coord, self.Utils.getHeight(avg_height), color)
            
            self.table_to_grid[table_coord] = [[a,b, pixels[scaled_id]] for a,b,scaled_id in cell_coords]
            for pixel in pixel_group:
                self.ids_to_table[pixel["id"]] = table_coord
        self.PC.display_table()
                                
    def serialSend(self, in_px): 
        table_coord = self.ids_to_table[in_px['id']]
        pixel_group = self.table_to_grid[table_coord]
        print(pixel_group)
        for i in range(len(pixel_group)):
            if pixel_group[i][2]['id'] == in_px["id"]:
                pixel_group[i][2] = in_px
        print(pixel_group)
        color = Counter([tuple(pixel[2]["color"]) for pixel in pixel_group]).most_common(1)[0][0]
        avg_height = sum([pixel[2]["height"] for pixel in pixel_group])/len(pixel_group)
        node_id = self.Utils.pixel_assignment[(table_coord[1], table_coord[0])][0] 
        self.PC.update_pixel(table_coord, self.Utils.getHeight(avg_height), color)
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
        
    def get_coords_loop(self, rows, cols, scale):
        use_scale = scale
        if (scale!=int(scale)):
            use_scale = math.ceil(scale)
        
        for y in range(0,rows,math.floor(scale)):
            for x in range(0, cols, math.floor(scale)):
                table_coord = (int(x/scale), int(y/scale))
                yield(x,y,table_coord,use_scale)
        
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
        return use_height
    
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