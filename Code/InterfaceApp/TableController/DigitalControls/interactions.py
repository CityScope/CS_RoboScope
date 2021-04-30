class Interacts():
    def __init__(self):
        self.interact = False
        self.paintType = None
        self.paintHeight = None
        self.increaseHeightDist = 0.01
        menu = {
            'shortPress': self.paintBrush,
            'longPress': self.increaseHeight,
            'doubleTap': self.cycle
        }
    
    def setInteraction(self, editBool, type, Utils):
        self.interact = editBool
        if (type):
            self.paintType = Utils.types[type["name"]]  
            self.paintHeight = type["height"]
    
    def shortPress(self):
        return menu['shortPress']
            
    def paintBrush(self, p, Utils):
        if self.interact and p.get("interactive")!= None and self.paintType:
            data = {
                'color': self.paintType['color'], 
                'height': self.paintHeight, 
                'id': p['id'], 
                'interactive': 'Web', 
                'name': self.paintType['name']
            }
            return data
    
    def increaseHeight(self, p, Utils):
        if self.interact and p.get("interactive")!= None and self.paintType:
            data = p.copy()
            data['height'] = min(data['height']+self.increaseHeightDist, 1)
            return data
        
    def cycle(self, p, Utils):
        if self.interact and p.get("interactive")!= None:
            [name, color] = self.getNextType(Utils.types, p['name'])
            data = {
                'color': color, 
                'height': p['height'], 
                'id': p['id'], 
                'interactive': 'Web', 
                'name': name
            }
            return data
    
    def getNextType(self, types, name):
        '''
        Helper Function for cycle land usage interaction
        '''
        keys = list(types.keys())
        index = keys.index(name)
        new_name =  keys[(index+1)%len(keys)]
        return [new_name, types[new_name].color]   
    
        
