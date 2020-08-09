import utils 
# maintains entire grid properties
total_grid = []
num_nodes = 12
nodes = [[] for i in range (num_nodes)]

# handle on start (fill total_grid with grid information)
def tableStart(features, properties):
    #reset total_grid and nodes
    global total_grid
    global nodes 
    total_grid = []
    nodes = [[] for i in range (num_nodes)]
    
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
            val = [utils.interaction(pixel), total_grid[pixel]['height'], colors[0], colors[1]]
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
    for i in nodes[node_id]: 
        feature = total_grid[i]
        colors = utils.getRGB565(feature['color'])
        height = utils.getHeight(['height'])
        val = [utils.interaction(i), height, colors[0], colors[1]]
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
        print(data)
        output.append(data)
    return output