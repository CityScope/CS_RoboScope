def waveTest(num_wave): 
    teensy_output = []
    IA_output = []
    interval= math.pi/12
    if num_wave != 0:
        interval = math.pi/num_wave
    
    #sin wave x values from 0 to pi with defined intervals
    sin_x = [x for x in numpy.arange(0.0, math.pi, interval)]
    
    for k in range(settings.NUM_PIXELS-num_wave):  
        if num_wave == 0:
            [teensy_table, IA_table] = waveTestNodes(k, sin_x)
        else: 
            [teensy_table, IA_table] = waveTestPixels(k, num_wave, sin_x)
        teensy_output.append(teensy_table)
        IA_output.append(IA_table)
    
    return [teensy_output, IA_output]
        
def waveTestNodes(k, sin_x):
    teensy_table = []
    IA_table = []
    for j in range(len(nodes)):
        node = nodes[j]
        output_node = [j]
        for pixel in node: 
            output_node.extend(getValTest(sin_x[(j+k)%12], pixel, True))
            IA_table.append(getDictValTest(sin_x[(j+k)%12], pixel, True))    
        teensy_table.append(output_node)
    return [teensy_table, IA_table]
    
def waveTestPixels(i, num_wave, sin_x):
    teensy_table = []
    IA_table = []
    updated_nodes = {}
        
    for j in range(0,num_wave):
        node_id = utils.ID_to_table(j+i)[0]
        if node_id not in updated_nodes:
            updated_nodes[node_id] = [j]
        else: 
            updated_nodes[node_id].append(j)
        
    # for every node to be updated
    for node_id in updated_nodes.keys():
        node = nodes[node_id]
        output_node = [node_id]
            
        # for every pixel in that node 
        for pixel in node:
            bool = False
            x = 0 # either not being changed
            if pixel-i in updated_nodes[node_id]:
                bool = True 
                x = sin_x[pixel-i] # or being set to a height specified by sin wave
            output_node.extend(getValTest(x, pixel, bool))
            IA_table.append(getDictValTest(x, pixel, bool))
        teensy_table.append(output_node)       
    
    return [teensy_table, IA_table]
    
def getValTest(x, pixel, height_bool):
    colors = utils.getRGB565(total_grid[pixel]['color'])
    height = 0
    if bool:
        height = int((math.sin(x))*255)
    val = [utils.interaction(pixel), height, colors[0], colors[1]]
    return [str(i) for i in val]

def getDictValTest(x, pixel, bool):
    color = total_grid[pixel]['color']
    name = utils.getNameofColor(color)
    height = 0
    if bool:
        height = int(math.sin(x)*settings.MAX_HEIGHT*2)
    total_grid[pixel]['height'] = height
    data = {'color': color, 'height': height, 'id': pixel, 'interactive': 'Web', 'name': name}
    return data
