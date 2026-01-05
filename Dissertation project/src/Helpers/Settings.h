#ifndef SETTINGS
#define SETTINGS

/*----------Simulation----------*/

//Generating the environment
#define USE_NOISE true

//Take faces from the axis objects and put them into one array
#define COLLECT_FACES true

/*----------Simulation----------*/


/*----------IDrawable-----------*/

//Add occlusion culling to every drawable object (not the most efficient)
#define OCCLUSION_CULL_QUERY false

/*----------IDrawable-----------*/


/*-------------Axis-------------*/

//Cull faces that are blocked by touching other faces
#define FACE_CULL true

//Use the Greedy Meshing algorithm
#define GREEDY_MESH true

//Collect all of the faces into a single array for drawing the axis (not currently usable with COLLECT_FACES)
#define USE_INSTANCING false

/*-------------Axis-------------*/


/*----------DrawWindow----------*/

//Cull that backs of faces
#define CULL_FACES true

/*----------DrawWindow----------*/


#endif // !SETTINGS