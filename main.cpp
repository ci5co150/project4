//Francisco J. Gomez
//Fall 2021
//Csci_172
//Pro_4
#include <string.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <iostream>

#include <math.h>

#include <stdio.h>
//use to get the soil library
#include <SOIL.h>
#include <vector>
using namespace std;
// Flag for to display wireframe
bool WireFrame = false;
//GLfloat lightX = 2.0;
//GLfloat lightY = 5.0;
//GLfloat lightZ = 5.0;
const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };
//GLfloat light_position[] = { lightX, lightY, lightZ, 0.0 };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };
// for y and x axis angles
int theta=0;
int thetaY = 0;
int skyX = 0; // used to rotate the skybox
int skyY = 0; // used to rotate the skybox
int spin = 0; // used to move the light
int spinY = 0; // used to move the light
int camX = 0.0; // used to move the camara
int camY = 0.0; // used to move the camara
// for scaling the cube
float alpha = 0.0;




/* GLUT callback Handlers */
// create a point class for the vertices
class Point{
	// keep them public for eazy access
	public:
	float x=0.0;
	float y=0.0;
	float z=0.0;
void printPoint(){ // print the points
	cout << x << " " << y << " " << z << "\n";
}

void printTex(){ // used to print the texture
	cout << x << " " << y << "\n";
}
};
//declare a vector of points for the vertices
vector<Point> vertices;
//declare a vector of points for the normals
vector<Point> verticesNormals;
//declare a vector of points for the normals
vector<Point> verticesTex;
//declare a vector of vectors to hold the faces
vector<vector<int>> faces;
vector<vector<float>> m;
float myMatrix[16];
//declare a Gl integer
GLuint tex[2]; // because for project 4 we use more than one an array is
               // create to hold the two textures.

 static void resize(int width, int height)
{
     double Ratio;

     width = (float)width;
     height = (float)height;

     Ratio= (double)width /(double)height;

    glViewport(0,0,(GLsizei) width,(GLsizei) height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

  gluPerspective (60.0f,Ratio,0.1f,1500.0f);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

 }
// This function is use to get the data from the object file
void getObject(){
	Point point;
	// Becasue openGL starts at 1 set dummy data for index 0
	vertices.push_back(point);
	// Becasue openGL starts at 1 set dummy data for index 0
	verticesNormals.push_back(point);
	// Becasue openGL starts at 1 set dummy data for index 0
	verticesTex.push_back(point);


    // The following lines are use to tokenize the file lines
	FILE *file;
	char* line = NULL;
	size_t len = 0;
	ssize_t read;

	// The following are used to check if the line has information regarding
	// vertices, normals, textures, or faces
	char v[] = "v";
	char vn[] = "vn";
	char vt[] = "vt";
	char f[] = "f";

	// open the file
	file = fopen("Cube-1.obj", "r");
	// check if the file is open sucessfully
	if( file == NULL){
		exit(EXIT_FAILURE);
	}

	// read each line of the file
	while((read = getline(&line, &len, file)) != -1){
        		char* token = strtok(line, " ");
            //check if the line has information regarding vertices
			if (strcmp(token,v) == 0){
				Point point;
				// add the x coordinate
				token = strtok(NULL, " ");
				point.x = atof(token);
				// add the y coordinate
				token = strtok(NULL, " ");
				point.y = atof(token);
				// add the z coordinate
				token = strtok(NULL, " ");
				point.z = atof(token);
				// push the point to the vector of points
				vertices.push_back(point);
			}
			//check if the line has information regarding vertices normals
			else if(strcmp(token,vn) == 0){
				Point point;
				// add the x coordinate
				token = strtok(NULL, " ");
				point.x = atof(token);
				// add the y coordinate
				token = strtok(NULL, " ");
				point.y = atof(token);
				// add the z coordinate
				token = strtok(NULL, " ");
				point.z = atof(token);
				// push the point to the vector of the normals
				verticesNormals.push_back(point);
			}
			//check if the line has information regarding vertices texture
			else if(strcmp(token,vt) == 0){
				Point point;
				// add the x coordinate
				token = strtok(NULL, ",");
				point.x = atof(token);
				// add the y coordinate
				token = strtok(NULL, ",");
				point.y = atof(token);
				// push the point to the vector of the normals
				verticesTex.push_back(point);
			}
			// populate the vector of faces vector
			else if(strcmp(token,f) == 0){
				vector<int> facesPrime;
				// populate the faces vector
				for(int i = 0; i < 3; i++){
					token = strtok(NULL, "/");
					facesPrime.push_back(atoi(token));
					token = strtok(NULL, "/");
					facesPrime.push_back(atoi(token));
					token = strtok(NULL, " ");
					facesPrime.push_back(atoi(token));
				}
				faces.push_back(facesPrime);
			}
	}
    // close the file
	fclose(file);
	if (line){
		free(line);
	}

	//print to check if the vectors have the information they have to have
	cout << "Check vector of points:\n";
	for( int i = 0; i < vertices.size(); i++){
		vertices[i].printPoint();
	}
	cout << "Check vector of Normals:\n";
	for( int i = 0; i < verticesNormals.size(); i++){
		verticesNormals[i].printPoint();
	}
	cout << "Check vector of Texture:\n";
	for( int i = 0; i < verticesTex.size(); i++){
		verticesTex[i].printTex();
	}
	cout << "Check vector of Faces:\n";
	for( int i = 0; i < faces.size(); i++){
		for(int j = 0; j <faces[i].size(); j++){
			cout << faces[i][j] << " ";
		}
		cout << endl;
	}

}
//function to draw the object from the data
void drawObject(){
        //Loop for each face of the object
        for(int i = 0; i < faces.size(); i++){
        //Feed information to the triangles using the data extracted from .obj file
        //stored in the vectors.
        glBegin(GL_TRIANGLES);
        glTexCoord2f(verticesTex[faces[i][1]].x,verticesTex[faces[i][1]].y);
        glNormal3f(verticesNormals[faces[i][2]].x,verticesNormals[faces[i][2]].y, verticesNormals[faces[i][2]].z);
        glVertex3f(vertices[faces[i][0]].x,vertices[faces[i][0]].y, vertices[faces[i][0]].z);

        glTexCoord2f(verticesTex[faces[i][4]].x,verticesTex[faces[i][4]].y);
        glNormal3f(verticesNormals[faces[i][5]].x,verticesNormals[faces[i][5]].y, verticesNormals[faces[i][5]].z);
        glVertex3f(vertices[faces[i][3]].x,vertices[faces[i][3]].y, vertices[faces[i][3]].z);

        glTexCoord2f(verticesTex[faces[i][7]].x,verticesTex[faces[i][7]].y);
        glNormal3f(verticesNormals[faces[i][8]].x,verticesNormals[faces[i][8]].y, verticesNormals[faces[i][8]].z);
        glVertex3f(vertices[faces[i][6]].x,vertices[faces[i][6]].y, vertices[faces[i][6]].z);

        glEnd();
    }
}
void drawSkybox(){
    glDisable(GL_LIGHTING); //Disable the light avoid calculations concerning light sources for this object.
    glBegin(GL_QUADS);
//  Back Face
    glNormal3f(0.0f, 0.0f, -1.0f);
    glTexCoord2f(1.0f, 0.33334f);
    glVertex3f(-1.0f,  1.0f,  1.0f);  // Top right Of The Texture and Quad
    glTexCoord2f(0.75f, 0.33334);
    glVertex3f( 1.0f,  1.0f,  1.0f);  // Top left Of The Texture and Quad
    glTexCoord2f(0.75f, 0.66667f);
    glVertex3f( 1.0f, -1.0f,  1.0f);  // Bottom left Of The Texture and Quad
    glTexCoord2f(1.0f, 0.66667f);
    glVertex3f(-1.0f, -1.0f,  1.0f);  // Bottom right Of The Texture and Quad

  // Front Face
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.25f, 0.33334f);
    glVertex3f(-1.0f,  1.0f, -1.0f);  // Bottom Right Of The Texture and Quad
    glTexCoord2f(0.25f, 0.66667f);
    glVertex3f(-1.0f, -1.0f, -1.0f);  // Top Right Of The Texture and Quad
    glTexCoord2f(0.50f, 0.66667f);
    glVertex3f( 1.0f, -1.0f, -1.0f);  // Top Left Of The Texture and Quad
    glTexCoord2f(0.50f, 0.33334f);
    glVertex3f( 1.0f,  1.0f, -1.0f);  // Bottom Left Of The Texture and Quad

    // Top Face
    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.50f, 0.33334f);
    glVertex3f( 1.0f,  1.0f, -1.0f);  // Bottom right Of The Texture and Quad
    glTexCoord2f(0.50f, 0.0f);
    glVertex3f( 1.0f,  1.0f,  1.0f);  // Top right Of The Texture and Quad
    glTexCoord2f(0.25f, 0.0f);
    glVertex3f(-1.0f,  1.0f,  1.0f);  // Top left Of The Texture and Quad
    glTexCoord2f(0.25f, 0.33334f);
    glVertex3f(-1.0f,  1.0f, -1.0f);  // Bottom left Of The Texture and Quad

    // Bottom Face
    glNormal3f(0.0f, -1.0f, 0.0f);
    glTexCoord2f(0.25f, 1.0f);
    glVertex3f( -1.0f, -1.0f,  1.0f);  // Bottom Right Of The Texture and Quad
    glTexCoord2f(0.50f, 1.0f);
    glVertex3f(1.0f, -1.0f,  1.0f);  // Bottom Left Of The Texture and Quad
    glTexCoord2f(0.50f, 0.666677f);
    glVertex3f(1.0f, -1.0f, -1.0f);  // Top Left Of The Texture and Quad
    glTexCoord2f(0.25f, 0.666677f);
    glVertex3f( -1.0f, -1.0f, -1.0f);  // Top Right Of The Texture and Quad

    // Right face  Y
    glNormal3f(1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.50f, 0.33334f);
    glVertex3f( 1.0f,  1.0f, -1.0f);  // Bottom Right Of The Texture and Quad
    glTexCoord2f(0.50f, 0.66667f);
    glVertex3f( 1.0f, -1.0f, -1.0f);  // Top Right Of The Texture and Quad
    glTexCoord2f(0.75f, 0.66667f);
    glVertex3f( 1.0f, -1.0f,  1.0f);  // Top Left Of The Texture and Quad
    glTexCoord2f(0.75f, 0.33334f);
    glVertex3f( 1.0f,  1.0f,  1.0f);  // Bottom Left Of The Texture and Quad

    // Left Face
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.25f, 0.33334f);
    glVertex3f(-1.0f,  1.0f, -1.0f);  // Top Left Of The Texture and Quad
    glTexCoord2f(0.0f, 0.33334f);
    glVertex3f(-1.0f,  1.0f,  1.0f);  // Top Right Of The Texture and Quad
    glTexCoord2f(0.0f, 0.66667f);
    glVertex3f(-1.0f, -1.0f,  1.0f);  // Bottom Right Of The Texture and Quad
    glTexCoord2f(0.25f, 0.66667f);
    glVertex3f(-1.0f, -1.0f, -1.0f);  // Boottom Left Of The Texture and Quad
    glEnd();

    glEnable(GL_LIGHTING); //Enable the lighting for the rest of the scene.
}
// This function is used to load an image to use as texture
// it takes the string representing the path of the image.
 void imageLoad(char * fileName, GLuint i){
 glEnable(GL_TEXTURE_2D);
 glGenTextures(2, tex);
 glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
 glBindTexture(GL_TEXTURE_2D, tex[i]); // images are 2D arrays of pixels, bound to the GL_TEXTURE_2D target.
 int width, height; // width & height for the image reader
 unsigned char* image;
 image = SOIL_load_image(fileName, &width, &height, 0, SOIL_LOAD_RGB);
 glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
// binding image data
 SOIL_free_image_data(image);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

 }
static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


    glTranslated(camX,camY,0.0); // use to move camera right,left,up,down
    gluLookAt(0,5,10,0.0,0.0,0.0,0.0,1.0,0.0);

    if(WireFrame){
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);	//Draw Our Mesh In Wireframe Mesh
    }
	else{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);	//Toggle WIRE FRAME
	}

    // your code here
    // This matrix holds the light
    glPushMatrix(); //It was given by the professor so I used to move the light
        glRotatef(spinY,0,1,0);
        glRotatef(spin,1,0,0);
        glTranslated(2.0,0.0,0.0);
        glLightfv(GL_LIGHT0,GL_POSITION,light_position);
    glPopMatrix();
    // This matrix holds the cube
    glPushMatrix();
    imageLoad("images/crate.jpg",0); // direct to an image for the crane.
    glRotatef(theta,0,1,0);
    glRotatef(thetaY,1,0,0);

    glScaled(1+alpha,1+alpha,1+alpha);

    //draw the cube
    drawObject();

    glPopMatrix();

    // This matrix holds the skybox
    glPushMatrix();
    glRotatef(skyX,1,0,0);
    glRotatef(skyY,0,1,0);
    glScaled(20.0 + alpha,20.0 + alpha, 20.0 + alpha);
    imageLoad("images/skybox1.jpg",1); //direct to an image for the skybox.
    drawSkybox();
    glPopMatrix();
    glutSwapBuffers();


}

static void key(unsigned char key, int x, int y)
{
    // check if the key is spacebar if so reset scene
    if(key == 32){ //if so then reset the values to zero.
            theta=0;
            thetaY = 0;
            skyX = 0;
            skyY = 0;
            spin = 0;
            spinY = 0;
            camX = 0;
            camY = 0;
            alpha = 0;

    }
    switch (key)
    {
        case 'f': //Show wireframe. It used to be 'w' in project 3, but now 'w' is used for something else.
            WireFrame = !WireFrame;
            break;
        case 'q':
            exit(0);
            break;
        case '0':
            spin = 0;
            spinY = 0;
            break;
        //The following rotates the lights
        case '1':
            spinY -= 5 % 360 ;
            break;
        case '2':
            spinY += 5 % 360 ;
            break;
        case '3':
            spin += 5 % 360 ;
            break;
        case '4':
            spin -= 5 % 360 ;
            break;
        //The following rotates the model
        case 'a':
            theta -= 5 % 360 ;
            break;
        case 'd':
            theta += 5 % 360 ;
            break;
        case 'w':
            thetaY -= 5 % 360 ;
            break;
        case 's':
            thetaY += 5 % 360 ;
            break;
        //The following are for zoom in and out of the scene
        // both to the model and skybox for a "zoom
        // affect". The zoom looks weird because of the size difference
        // of the model & skybox.
        case 'z':
            alpha += 0.25;
            break;
        case 'x':
            alpha -= 0.25;
            break;
        // Move the camera left, right, bottom
        // & reset
        case 'j':
            camX += 1;
        break;
        case 'k':
            camX -= 1;
        break;
        case 'i':
            camY -= 1;
        break;
        case 'l':
            camY += 1;
        break;
        case 'o': //reset the camera
            camY = 0;
            camX = 0;
        break;
    }
}
// Used to rotate the skybox
void Specialkeys(int key, int x, int y)
{
    switch(key)
    {
    case GLUT_KEY_LEFT:
            skyY -=5 % 360;
        break;
    case GLUT_KEY_RIGHT:
            skyY +=5 % 360;
        break;
    case GLUT_KEY_UP:
            skyX -=5 % 360;
        break;
    case GLUT_KEY_DOWN:
            skyX +=5 % 360;
        break;
    case GLUT_KEY_END:
            alpha += 0.25;
    break;
    case GLUT_KEY_HOME:
            alpha -= 0.25;
    break;
   }
  glutPostRedisplay();
}

static void idle(void)
{
    glutPostRedisplay();
}

static void init(void)
{
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);

    glEnable(GL_DEPTH_TEST);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glShadeModel(GL_SMOOTH);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);


}


/* Program entry point */

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);

    glutInitWindowSize(800,600);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("GLUT Shapes");
    init();
    glutReshapeFunc(resize);
    getObject(); //get the information about the object
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutSpecialFunc(Specialkeys);

    glutIdleFunc(idle);
    glutMainLoop();

    return EXIT_SUCCESS;
}
