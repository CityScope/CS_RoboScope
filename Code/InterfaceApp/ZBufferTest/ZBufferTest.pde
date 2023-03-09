PShader depthShader;
float angle = 0.0;


void setup(){
    
    // Set screen size and renderer
    size(600, 480, P3D);
    noStroke();
    
    // Load shader
    depthShader = loadShader("frag.glsl", "vert.glsl");
    //depthShader.set("near", 40.0); // Standard: 0.0
    //depthShader.set("far", 60.0); // Standard: 100.0
    //depthShader.set("nearColor", 1.0, 0.0, 0.0, 1.0); // Standard: white
    //depthShader.set("farColor", 0.0, 0.0, 1.0, 1.0); // Standard: black
    
}

 
void draw(){
    
    // Fill background and set camera
    background(#000000);
    camera(0.0, 0.0, 50.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    
    // Bind shader
    shader(depthShader);
    
    // Calculate angle
    angle += 0.01;
    
    // Render "sky"-cube
    pushMatrix();
    rotate(angle, 0.0, 1.0, 0.0);
    box(100.0);
    popMatrix();
    
    // Render cubes
    pushMatrix();
    translate(-30.0, 20.0, -50.0);
    rotate(angle, 1.0, 1.0, 1.0);
    box(25.0);
    popMatrix();
    pushMatrix();
    translate(30.0, -20.0, -50.0);
    rotate(angle, 1.0, 1.0, 1.0);
    box(25.0);
    popMatrix();
    
    // Render spheres
    pushMatrix();
    translate(-30.0, -20.0, -50.0);
    rotate(angle, 1.0, 1.0, 1.0);
    sphere(20.0);
    popMatrix();
    pushMatrix();
    translate(30.0, 20.0, -50.0);
    rotate(angle, 1.0, 1.0, 1.0);
    sphere(20.0);
    popMatrix();
    
}
