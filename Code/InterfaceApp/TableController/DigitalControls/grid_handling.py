import math
import sys
# insert at 1, 0 is the script path (or '' in REPL)
sys.path.insert(1, '../PhysicalControls')
from main import PhysicalController
from collections import Counter 
from utils import Utils

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
                    grid_size=(self.Utils.TABLE_ROWS, self.Utils.TABLE_COLS),
                    com_port ='/dev/cu.usbmodem6893170', BAUDRATE=2000000)
    
    def setSelected(self, scale, pixels):
        '''
        Handle updates from interface app on scale or translate change
        '''
        #update settings, pixels, and scale
        self.scale= scale
        self.selectedPixels = pixels
        self.Utils.updateSettings(int(self.Utils.TABLE_COLS*scale), 
                                  int(self.Utils.TABLE_ROWS*scale), 
                                  len(pixels), scale)
        self.table_to_grid = {}
        self.ids_to_table = {}
        for x,y,table_coord,r_scale in self.get_coords_loop():
            #get all (x,y) coordinates in range of current pixel (one pixel if scale is 1)
            cell_coords = [(x+i,y+j, (y+j)*self.Utils.VIEW_COLS+(x+i)) for i in range(r_scale) for j in range(r_scale) if x+i < self.Utils.VIEW_COLS and y+j < self.Utils.VIEW_ROWS]
            #get pixel data for all cells in cell_coords
            pixel_group = [pixels[scaled_id] for a,b,scaled_id in cell_coords]
            print(pixel_group)
            #get average color and height for current pixel
            color = Counter([tuple(pixel["color"]) for pixel in pixel_group]).most_common(1)[0][0]
            avg_height = sum([pixel["height"] for pixel in pixel_group])/len(pixel_group)
            #update PC with new information
            self.PC.update_pixel(table_coord, self.Utils.getHeight(avg_height), color)
            #update table_to_grid and ids_to_table to keep mapping up to date
            self.table_to_grid[table_coord] = [[a,b, pixels[scaled_id]] for a,b,scaled_id in cell_coords]
            for pixel in pixel_group:
                self.ids_to_table[pixel["id"]] = table_coord
        #send all data
        self.PC.send_table_data()
                                
    def serialSend(self, in_px): 
        '''
        handles brush events from the interface app
        '''
        table_coord = self.ids_to_table[in_px['id']]
        pixel_group = self.table_to_grid[table_coord]
        for i in range(len(pixel_group)):
            if pixel_group[i][2]['id'] == in_px["id"]:
                pixel_group[i][2] = in_px
        #compute new color and new height
        color = Counter([tuple(pixel[2]["color"]) for pixel in pixel_group]).most_common(1)[0][0]
        avg_height = sum([pixel[2]["height"] for pixel in pixel_group])/len(pixel_group)
        #get node
        node_id = self.Utils.pixel_assignment[(table_coord[1], table_coord[0])][0] 
        #update pixel in physical controller and send NODE data
        self.PC.update_pixel(table_coord, self.Utils.getHeight(avg_height), color)
        self.PC.send_node_data(node_id)

    def serialRead(self):
        if self.PC:
            return self.PC.read_pixel_data()
        return None
        
    def serialReceive(self, pixels):  
        '''
        handles data received from table
        '''
        output = []
        node = int(pixels[0])
        for i in range(1, len(pixels)): 
            pixel = [int(j) for j in pixels[i]]
            id = self.PC.node_location[node][i-1]
            [name, color] = self.Utils.getRGB888([pixel[2], pixel[3]])
            data = {'color': color, 'height': pixel[0], 'id': id, 'interactive': 'Web', 'name': name}
            output.append(data)
        return output
        
    def get_coords_loop(self):
        '''
        loop through (x,y) cells in selected features
        - to handle float scales, overlap pixels to get average
            * use ceil for # of pixels in group, use floor for step of for loop
        - yields x,y, (x,y) coordinate on table (scaled down of x,y), and ceiled scale
        '''
        scale = self.scale
        use_scale = scale
        if (scale!=int(scale)):
            use_scale = math.ceil(scale)
        
        for y in range(0,self.Utils.VIEW_ROWS,math.floor(scale)):
            for x in range(0, self.Utils.VIEW_COLS, math.floor(scale)):
                table_coord = (int(x/scale), int(y/scale))
                yield(x,y,table_coord,use_scale)