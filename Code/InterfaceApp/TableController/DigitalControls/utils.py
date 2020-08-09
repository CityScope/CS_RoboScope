rgb888Dict = {}
rgb565Dict = {}
maxHeight = 50
## onStart create dictionaries that map land use to rgb888 and rgb 565 colors
def colorMapping(properties): 
    global rgb888Dict
    global rgb565Dict
    types = properties["types"]
    for i in types.keys():
        rgbColor = hextoRGB(types[i]["color"])
        color565 = RGB565(rgbColor)
        rgb888Dict[tuple(rgbColor)] = [i,color565]
        rgb565Dict[tuple(color565)] = [i, rgbColor]

## changes hex string to RGB888 color list
def hextoRGB(hex): 
    h = hex.lstrip('#')
    return [int(h[i:i+2], 16) for i in (0, 2, 4)]

## compute rgb565 color from rgb888 color
def RGB565(color):
    val = ("%0.4X" % ( ((color[0] & 0xf8)<<8) + ((color[1] & 0xfc)<<3)+(color[2]>>3)))
    output = [int(val[0:2], 16), int(val[2:4], 16)]
    return output

## defines heighest building height
def setMaxHeight(height):
    global maxHeight
    maxHeight = height

## defines 0-1 height
def getHeight(height):
    return height/maxHeight
    
## computes the node and local motor ID needed from pixel ID 
## to-do: make general
def ID_to_table(id):
    coor = [int(id%8), int(id/8)]
    node = int(coor[1]/4)*4+int(coor[0]/2)
    local_id = (coor[1]%4)*2+(id%2)
    return [node, local_id]

## computes pixel ID from  node and local motor ID
def table_to_ID(node, local):
    return node

## get rgb565 color that matches an inputted rgb888 color
def getRGB565(color): 
    return rgb888Dict[tuple(color)][1]
    
## get rgb888 color that matches inputted rgb565 color
def getRGB888(color): 
    absVal = 510
    index = 0
    for i in rgb565Dict.keys(): 
        tempabs = abs(i[0]-color[0]) + abs(i[1]-color[1])
        if tempabs < absVal:
            absVal = tempabs
            index = i        
    return rgb565Dict[index]


## method to handle creating interaction byte once needed
def interaction(id):
    return int('10101010',2)