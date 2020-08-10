import utils 
import math
import numpy
from settings import *
# maintains entire grid properties
total_grid = []
nodes = [[] for i in range (NUM_NODES)]

# handle on start (fill total_grid with grid information)
def tableStart(features, properties):
    #reset total_grid and nodes
    global total_grid
    global nodes 
    total_grid = []
    nodes = [[] for i in range (NUM_NODES)]
    
    # set up colorMapping between RGB565 and RGB888
    utils.colorMapping(properties)
    utils.setMaxHeight(50)
    # fill in total_grid and nodes with grid information
    for feature in features:
        node = utils.ID_to_table(feature["properties"]['id'])[0]
        nodes[node].append(feature["properties"]['id'])
        total_grid.append(feature["properties"])
    
    # send grid information to table on Start
    output = []
    # for each node in the table
    for j in range(len(nodes)):
        node = nodes[j]
        output_node = [j] #length 33 array (node number + 32 bytes of information)
        # for each pixel in a node
        for pixel in node: 
            colors = utils.getRGB565(total_grid[pixel]['color'])
            height = int(utils.getHeight(total_grid[pixel]['height'])*255)
            val = [utils.interaction(pixel), height, colors[0], colors[1]]
            output_node.extend([str(i) for i in val])
        output.append(output_node)
    
    return output

# update total_grid if a change from interface app or table is made
def updateTotal(pixel): 
    total_grid[pixel['id']]['color'] = pixel['color']
    total_grid[pixel['id']]['height'] = pixel['height']
    total_grid[pixel['id']]['name'] = pixel['name']

# send updates from interface app to table
def serialSend(pixel):   
    updateTotal(pixel)
    node_id = utils.ID_to_table(pixel['id'])[0]
    output = [node_id]
    for pixel in nodes[node_id]: 
        feature = total_grid[pixel]
        colors = utils.getRGB565(feature['color'])
        height = int(utils.getHeight(feature['height'])*255)
        val = [utils.interaction(pixel), height, colors[0], colors[1]]
        output.extend([str(i) for i in val])
    return output

# receive updates from table to be sent to interface app
def serialReceive(pixels):  
    output = []
    node = int(pixels[0])
    for i in range(1, len(pixels)): 
        pixel = [int(j) for j in pixels[i]]
        id = nodes[node][i-1]
        [name, color] = utils.getRGB888([pixel[2], pixel[3]])
        data = {'color': color, 'height': pixel[0], 'id': id, 'interactive': 'Web', 'name': name}
        updateTotal(data) 
        output.append(data)
    return output
        
def waveTestTable():
    teensy_output = []
    IA_output = []
    x = 0.0
        
    for x in numpy.arange(0.0, math.pi*2, 0.1):
        teensy_table = []
        IA_table = []
        for j in range(len(nodes)):
            node = nodes[j]
            output_node = [j]
            for pixel in node: 
                output_node.extend(getValTestTable(x, pixel))
                IA_table.append(getDictValTestTable(x, pixel))    
            teensy_table.append(output_node)
        teensy_output.append(teensy_table)
        IA_output.append(IA_table)
    return [teensy_output, IA_output]

def waveTestNode():
    teensy_output = []
    IA_output = []
    x = 0.0
    l = 0
    r = 6
    for x in numpy.arange(0.0, math.pi*2, 0.1):
        teensy_table = []
        IA_table = []
        for j in range(l,r+1):
            node_id = utils.ID_to_table(j)[0]
            node = nodes[node_id]
            output_node = [node_id]
            for pixel in node: 
                output_node.extend(getValTestNode(x, pixel, l, r, j))
                IA_table.append(getDictValTestNode(x, pixel,l,r, j))    
            teensy_table.append(output_node)
        teensy_output.append(teensy_table)
        IA_output.append(IA_table)
        l+=1
        r+=1
    return [teensy_output, IA_output]
    
def getValTestTable(x, pixel):
    colors = utils.getRGB565(total_grid[pixel]['color'])
    height = int((math.sin(x+(pixel%8)*math.pi/12)+1)*127)
    val = [utils.interaction(pixel), height, colors[0], colors[1]]
    return [str(i) for i in val]

def getDictValTestTable(x, pixel):
    color = total_grid[pixel]['color']
    name = utils.getNameofColor(color)
    height = int((math.sin(x+(pixel%8)*math.pi/12)+1)*MAX_HEIGHT*2)
    data = {'color': color, 'height': height, 'id': pixel, 'interactive': 'Web', 'name': name}
    return data

def getValTestNode(x, pixel, l, r, j):
    colors = utils.getRGB565(total_grid[pixel]['color'])
    if pixel <= r  and pixel >= l:
        height = int((math.sin(x + (j-l)*math.pi/6)+1)*127)
    else: 
        height = 0
    val = [utils.interaction(pixel), height, colors[0], colors[1]]
    return [str(i) for i in val]

def getDictValTestNode(x, pixel, l ,r, j):
    color = total_grid[pixel]['color']
    name = utils.getNameofColor(color)
    if pixel <= r  and pixel >= l:
        height = int((math.sin(x + (j-l)*math.pi/6)+1)*MAX_HEIGHT*2)
    else: 
        height = 0
    data = {'color': color, 'height': height, 'id': pixel, 'interactive': 'Web', 'name': name}
    return data
