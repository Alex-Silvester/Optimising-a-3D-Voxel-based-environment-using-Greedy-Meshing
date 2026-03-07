#ifndef SETTINGS
#define SETTINGS

/*----------Simulation----------*/

//Amount of samples taken for the average fps test
#define AVERAGE_FPS_SAMPLES 20

//space between samples being taken for the average fps in seconds 
// (or how much time it takes to change the average fps value)
#define FPS_SAMPLE_SPACING 0.1

//whether imgui is enabled or not
#define USE_IMGUI true

//show and get data from selected faces
#define FACE_CHECKING false;

//Generating the environment
#define USE_NOISE true

//Take faces from the axis objects and put them into one array
#define COLLECT_FACES true

//Does a pre-pass on the z-buffer to fill it out before rendering all of the geometry fully
#define Z_BUFFER_PRE_PASS true

//turns on and off frustum culling
#define FRUSTUM_CULLING true

//show closest points
#define CLOSEST_POINTS false;

//Write the profiling to file
#define WRITE_TO_FILE false

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

//defines whether using the tab key activates/deactivates the cursor visibility
#define TAB_CHANGES_CURSOR false

//whether there is the ability to use the freecam
#define FREECAM_ACTIVE false

/*----------DrawWindow----------*/

/*----------WindowBase----------*/

//whether the window whould have the bar on the top or not
#define DECORATE_WINDOW false

/*----------WindowBase----------*/



#endif // !SETTINGS

