//#include<stdafx.h>
#include<stdio.h>
#include<time.h>
#include<windows.h>
#include<GL/glut.h>
#define UP 1
#define Down 2
#define LEFT 3
#define RIGHT 4

// Status Variables
GLint   lvl      = 1;
GLint   points    =0;
GLint   points1 = 0;
GLint   points2 = 0;
GLint	points3 = 0;
GLint   points4 = 0;
GLint   size  = 0;
GLbyte  gameOver = true;
GLbyte  EnableLight = true;    

// Snake Variables
GLint   bodyPos[2][100] = {{}};
GLint   _x       = 5;
GLint   _z       = 10;
GLint   _oldX[2] = {};
GLint   _oldZ[2] = {};
GLbyte  direction  = 0;

// Food Variables
GLint   _ax      = 0;
GLint	_az		 = 0;
GLint   _bx      = 0;
GLint   _bz      = 0;
GLint   _cx		 = 0;
GLint	_cz		 = 0;
GLint   _dx      = 0;
GLint	_dz		 = 0;
// Screen variables
GLint   _w       = 800;
GLint   _h       = 550;
GLint   _Giw     = 0;
GLint   _Gih     = 0;
GLint   _Gfw     = 150;
GLint   _Gfh     = 150;

//Variables for the Camera Angle
static GLfloat view_rotx=45.0F ;
static GLfloat view_roty=0.0F ;
static GLfloat view_rotz=0.0F ;

static GLfloat headRotation=90.0F ;
static GLfloat zoom=-300.0F ;

// Variables to check the FPS
DWORD   g_dwLastFPS = 0;
int	g_nFPS = 0, g_nFrames = 0;

//Configure the lightning
void initLight()
{
    //Add ambient light
    GLfloat ambientColor[] = {0.3f, 0.4f, 0.8f, 1.0f}; 
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);

    //Add positioned light
    GLfloat lightColor0[] = {0.5f, 0.5f, 0.5f, 1.0f}; 
    GLfloat lightPos0[] = {4.0f, 0.0f, 8.0f, 1.0f}; 
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);

    //Add directed light
    GLfloat lightColor1[] = {0.5f, 0.2f, 0.2f, 1.0f}; 
    //Coming from the direction (-1, 0.5, 0.5)
    GLfloat lightPos1[] = {-1.0f, 0.5f, 0.5f, 0.0f};
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
    glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
}

//initialize the first configurations
void Initialize(void) 
{
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.7f, 0.9f, 1.0f, 1.0f); //Change the background to sky blue
    if(EnableLight){
        glEnable(GL_COLOR_MATERIAL); //Enable color
        glEnable(GL_LIGHTING); //Enable lighting
        glEnable(GL_LIGHT0); //Enable light #0
        glEnable(GL_LIGHT1); //Enable light #1
        glEnable(GL_NORMALIZE); //Automatically normalize normals
    }
}

//Configure window resize
void resize (int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / (double)h, 1, 800.0);
}

void Write(char *string)
{//Write string on the screen
    while(*string)
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *string++);
}
//This Function will rotate the object according to the Angles
void ManipulateViewAngle() {
    glRotatef(view_rotx,1.0,0.0,0.0);//Rotate Arround X axis
    glRotatef(view_roty,0.0,1.0,0.0);//Rotate Arround Y axis
    glRotatef(view_rotz,0.0,0.0,1.0);//Rotate Arround Z axis
}

//This Function will reset the snake size and location...
void Reset(){
    _x  = 5;
    _z  = 10;
    direction  = 0;
    lvl      = 1;
    points1 = 0;
	points2 = 0;
	points3 = 0;
	points4 = 0;
    size  = 0;
    gameOver = false;
    view_rotx=45.0F ;
    view_roty=0.0F ;
    view_rotz=0.0F ;
    headRotation=90.0F ;
    zoom=-300.0F ;
}

//Display a welcome screen
void WelcomeScreen()
{
    char tmp_str[40];
	glClearColor(0.7f, 0.9f, 1.0f, 1.0f);
    glColor3f(1, 0, 0);
    glRasterPos2f(0, 25);
    Write("Welcome To Snake 3D Game.");

    glColor3f(1, 0, 1);
    glRasterPos2f(0, 15);
    Write("Prepared By Bhatt Mehul B. & Anupam Kumar.");

	glColor3f(1, 0, 1);
    glRasterPos2f(0, 8);
    Write("USN:-1RI12CS401 & USN:-1RI12CS005");
    
    glColor3f(0, 0, 1);
    glRasterPos2f(0, 0);
    Write("To Start Playing please press 'n'. Enjoy");
}

void DrawSnake(){
    int  i;

    //Drawing the head & the plane
    glPushMatrix();
    ManipulateViewAngle();
        //This will draw the plane that the snake will run on.
        glPushMatrix();
        glColor3f(0.6f, 0.7f, 0.8f);
        glTranslatef(75.0, -16.0, 75.0);
        glScalef(155,5.0,155);
        glutSolidCube(1);
        glPopMatrix();
    //Here we will draw the Head of the snake
    glColor3f(0.0,0.0,1.0);//Color it blue
    glTranslatef(_x,-10.0,_z);//Give it the location according to _x & _z
    glScalef(0.5,0.5,0.5);
    glutSolidSphere(10,20,20);//Draw the head as a sphere a litle bit bigger than the body spheres
    glRotatef(headRotation, 0.0, 1.0, 0.0);
    glColor3f(1.0,1.0,0.0);
    glTranslatef(0,0.0,6.0);
    glScalef(0.8,1.0,1.0);
    glutSolidCone(10,10,20,20);
    glColor3f(0,0,0);
    glTranslatef(-4.0,10.0,0.0);
    glScalef(0.3,0.3,0.3);
    glutSolidSphere(10,20,20);
    glTranslatef(26.0,0.0,0.0);
    glutSolidSphere(10,20,20);
    glPopMatrix();

    //Drawing the body
    for(i=0; i<size; i++){//Loop throw the size and draw spheres representing the body
        glPushMatrix();
        ManipulateViewAngle();
        glTranslatef(bodyPos[0][i],-10.0,bodyPos[1][i]);//this will locate the spheres
        glColor3f(1.0,1.0,0.6);//Color Red
        glScalef(0.5,0.5,0.5);
        glutSolidSphere(7,20,20);
        glPopMatrix();
    }
}
bool collision(){
    int i;

    for(i=0; i<size; i++){
        if((bodyPos[0][i] == _x && bodyPos[1][i] == _z) || 
        ((bodyPos[0][i] >= _x) && (bodyPos[0][i] <= _x + 5) && (bodyPos[1][i] >= _z) && (bodyPos[1][i] <= _z + 5)) ||
        ((bodyPos[0][i] <= _x) && (bodyPos[0][i] >= _x - 5) && (bodyPos[1][i] <= _z) && (bodyPos[1][i] >= _z - 5)) ||
        ((bodyPos[0][i] <= _x) && (bodyPos[0][i] >= _x - 5) && (bodyPos[1][i] >= _z) && (bodyPos[1][i] <= _z + 5)) ||
        ((bodyPos[0][i] >= _x) && (bodyPos[0][i] <= _x + 5) && (bodyPos[1][i] <= _z) && (bodyPos[1][i] >= _z - 5)))

        return true;
    }
    return false;
}

void DrawFood()
{ 
	char tmp_str[100];
    //Draw the Sphere representing the Food for the snake
    glPushMatrix();
    ManipulateViewAngle();
    glTranslatef(_bx,-10.0,_bz);
    glColor3f(0.8, 0.4, 0.4);
    glScalef(0.5,0.5,0.5);
    glutSolidSphere(7,20,20);
    glPopMatrix();
	 
}
void DrawFood2()
{
	char tmp_str[100];
	glPushMatrix();
    ManipulateViewAngle();
    glTranslatef(_ax,-10.0,_az);
    glColor3f(0.4, 0.6, 0.4);
    glScalef(0.5,0.5,0.5);
    glutSolidSphere(7,10,20);
    glPopMatrix();
}
void DrawFood3()
{
	char tmp_str[100];
	glPushMatrix();
    ManipulateViewAngle();
    glTranslatef(_cx,-10.0,_cz);
    glColor3f(0.3, 0.4, 0.3);
    glScalef(0.5,0.5,0.5);
    glutSolidSphere(7,10,20);
    glPopMatrix();


}
void DrawFood4()
{
	char tmp_str[100];
	glPushMatrix();
    ManipulateViewAngle();
    glTranslatef(_dx,-10.0,_dz);
    glColor3f(0.3, 0.9, 0.3);
    glScalef(0.5,0.5,0.5);
    glutSolidSphere(7,10,20);
    glPopMatrix();


}
//Generates Random Numbers for the location of the food that the snake will eat
int RandomNumber(int high, int low)
{
    return (rand() % (high-low))+low;
}
//Generate the New Food that the snake will eat
void newFood()
{
    time_t seconds;
    time(&seconds);
    srand((unsigned int) seconds);
    _bx = RandomNumber(_Gfw-_Giw, _Giw+10);
    _bz = RandomNumber(_Gfh-_Gih, _Gih+10);
}
int RandomNumber2(int high, int low)
{
    return (rand() % (high+low))-low;
}
void newFood2()
{
    time_t seconds;
    time(&seconds);
    srand((unsigned int) seconds);
	_ax = RandomNumber2(_Gfw-_Giw, _Giw+10);
    _az = RandomNumber2(_Gfh-_Gih, _Gih+10);
}
int RandomNumber3(int high, int low)
{
    return (rand() % (high+low));
}
void newFood3()
{
    time_t seconds;
    time(&seconds);
    srand((unsigned int) seconds);
    _cx = RandomNumber3(_Gfw-_Giw, _Giw+10);
    _cz = RandomNumber3(_Gfh-_Gih, _Gih+10);
}
int RandomNumber4(int high, int low)
{
    return (rand() % (high-low));
}
void newFood4()
{
    time_t seconds;
    time(&seconds);
    srand((unsigned int) seconds);
    _dx = RandomNumber4(_Gfw-_Giw, _Giw+10);
    _dz = RandomNumber4(_Gfh-_Gih, _Gih+10);
}


//This Function will calculate the frame per second to display on the screen
void getFPS(){
    char tmp_str[40]; 

    if( GetTickCount() - g_dwLastFPS >= 1000 )				// When A Second Has Passed...
    {
        g_dwLastFPS = GetTickCount();					// Update Our Time Variable
        g_nFPS = g_nFrames;						// Save The FPS
        g_nFrames = 0;							// Reset The FPS Counter
    }
    g_nFrames++;

    glRasterPos2f(75, 50);
    sprintf(tmp_str, "FPS: %d", g_nFPS);
    Write(tmp_str);
    
    glRasterPos2f(50, 60);
    sprintf(tmp_str, "Pos X: %d Pos Z: %d", _x, _z);
    Write(tmp_str);
}
//This Function Will Check for Collision
void pnts()
{
	points=points1+points2+points3+points4;	

}
void level()
{
	pnts();
	if((points1+points2+points3+points4)%20==0 && lvl<15) 
	{	lvl++;	}
}



void Run2()
{
		if((_x == _ax && _z == _az) ||
    ((_x >= _ax) && (_x <= _ax + 4) && (_z >= _az) && (_z <= _az + 4))||
    ((_x <= _ax) && (_x >= _ax - 4) && (_z <= _az) && (_z >= _az - 4))||
    ((_x <= _ax) && (_x >= _ax - 4) && (_z >= _az) && (_z <= _az + 4)) ||
    ((_x >= _ax) && (_x <= _ax + 4) && (_z <= _az) && (_z >= _az - 4)))
		{
        points2=points2+4;
        if(points2 < 500) size++;
		level();
		newFood2();
		}
}
void Run3()
{
	if((_x == _cx && _z == _cz) ||
    ((_x >= _cx) && (_x <= _cx + 4) && (_z >= _cz) && (_z <= _cz + 4))||
    ((_x <= _cx) && (_x >= _cx - 4) && (_z <= _cz) && (_z >= _cz - 4))||
    ((_x <= _cx) && (_x >= _cx - 4) && (_z >= _cz) && (_z <= _cz + 4)) ||
    ((_x >= _cx) && (_x <= _cx + 4) && (_z <= _cz) && (_z >= _cz - 4)))
		{
        points3=points3+6;
        if(points3 < 500) size++;
		level();
		newFood3();
		}
}
void Run4()
{
	if((_x == _dx && _z == _dz) ||
    ((_x >= _dx) && (_x <= _dx + 4) && (_z >= _dz) && (_z <= _dz + 4))||
    ((_x <= _dx) && (_x >= _dx - 4) && (_z <= _dz) && (_z >= _dz - 4))||
    ((_x <= _dx) && (_x >= _dx - 4) && (_z >= _dz) && (_z <= _dz + 4)) ||
    ((_x >= _dx) && (_x <= _dx + 4) && (_z <= _dz) && (_z >= _dz - 4)))
		{
        points4=points4+8;
        if(points4 < 500) size++;
		level();
		newFood4();
		}
}
//This Function will move the snake according to the directions 
//Taken from the Keyboard keys
void Run(int value)
{
    int i;

    _oldX[1] = _x;
    _oldZ[1] = _z;
    switch(direction){
      case RIGHT:
          headRotation =90;
          _x += 6;    
          if(_x > _Gfw-2) _x = _Giw-1;//This will check if the snake is going into the border so it will appear on the other side
          break;
      case LEFT:
          headRotation =-90;
          _x -= 6;    
         if(_x < 0) _x = _Gfw-2;//This will check if the snake is going into the border so it will appear on the other side
          break;
      case UP:
          headRotation =0;
          _z += 6;    
          if(_z > _Gfh-2) _z = _Gih-1;//This will check if the snake is going into the border so it will appear on the other side
          break;
      case Down:
          headRotation =180;
          _z -= 6;    
          if(_z < 2) _z = _Gfh-2;//This will check if the snake is going into the border so it will appear on the other side
          break;
    }

    //Checks for Collisoin if yes Game Over
    if(collision()) gameOver = 2;

    //Checks if the snake ate the food (check the X and Y)
    // If yes it will increase the points & the size of the snake & create a new food
    if((_x == _bx && _z == _bz) || 
    ((_x >= _bx) && (_x <= _bx + 4) && (_z >= _bz) && (_z <= _bz + 4)) ||
    ((_x <= _bx) && (_x >= _bx - 4) && (_z <= _bz) && (_z >= _bz - 4)) ||
    ((_x <= _bx) && (_x >= _bx - 4) && (_z >= _bz) && (_z <= _bz + 4)) ||
    ((_x >= _bx) && (_x <= _bx + 4) && (_z <= _bz) && (_z >= _bz - 4)))
	{
        points1=points1+2;
        if(points1 < 500) size++;
		level();
        newFood();
    }
	Run2();
	Run3();
	Run4();
    for(i = 0; i<size; i++){// Save the positions of the body parts
        _oldX[0]       = _oldX[1];
        _oldZ[0]       = _oldZ[1];
        _oldX[1]       = bodyPos[0][i];
        _oldZ[1]       = bodyPos[1][i];
        bodyPos[0][i] = _oldX[0];
        bodyPos[1][i] = _oldZ[0];
    }

    //Set the Timer
    glutTimerFunc(130-lvl*6, Run, 0);                    
}
void GameStatus(){

    char tmp_str[40];
    // Print the status of the game on the screen
    glColor3f(0, 0, 0);
    glRasterPos2f(5, 50);
	pnts();
    sprintf(tmp_str, "Level: %d Points: %d", lvl, points);
	Write(tmp_str);
}

void gameover()
{
	char tmp_str[40];
	if(points==0)
		WelcomeScreen();
	else
	{
    glColor3f(1, 0, 0);
    glRasterPos2f(0, 25);
    Write("GAME OVER.");

    glColor3f(1, 0, 1);
    glRasterPos2f(0, 8);
    sprintf(tmp_str, "Top Level: %d Total Points: %d", lvl, points);
	Write(tmp_str);
    
    glColor3f(0, 0, 1);
    glRasterPos2f(0, 0);
    Write("To Start Playing again please press 'n'. Enjoy");

	}
}
void Display(void){//Draw Function
    // Clear screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    if(EnableLight) initLight();

    glTranslatef (-60.0, 40.0, zoom);
    //Check if the value of the Flag "Game Over is not True it will continue the game"
    if(!gameOver){
        GameStatus();
        DrawFood();
		DrawFood2();
		DrawFood3();
		DrawFood4();
        DrawSnake();
    }
    else
		gameover();
 
    getFPS();

    // Updates the screen
    glutPostRedisplay();
    glutSwapBuffers();
}

void Special(int key, int x, int y){
    switch(key){
    case GLUT_KEY_RIGHT     :
        if(direction != LEFT)
            direction = RIGHT;
        break;
    case GLUT_KEY_LEFT      :
        if(direction != RIGHT)
            direction = LEFT;
        break;
    case GLUT_KEY_UP        :
        if(direction != UP)
            direction = Down;
        break;
    case GLUT_KEY_DOWN      :
        if(direction != Down)
            direction = UP;
        break;
    }
}

void keyboard (unsigned char key, int x, int y)
{
    switch (key) {
      //All Snake Rotation Over X, Y, Z angles
      case 'x'    : view_rotx +=2 ;
          glutPostRedisplay();
          break;
      case 'X'    : view_rotx -=2 ;
          glutPostRedisplay();
          break;
      case 'y'    : view_roty +=2 ;
          glutPostRedisplay();
          break;
      case 'Y'    : view_roty -=2 ;
          glutPostRedisplay();
          break;
      case 'z'    : view_rotz +=2 ;
          glutPostRedisplay();
          break;
      case 'Z'    : view_rotz -=2 ;
          glutPostRedisplay();
          break;
      case 'a'    : size++ ;
          glutPostRedisplay();
          break;
      case 'A'    : size-- ;
          glutPostRedisplay();
          break;
      case '+'    : zoom++ ;
          glutPostRedisplay();
          break;
      case '-'    : zoom-- ;
          glutPostRedisplay();
          break;
      case 'n'    : Reset() ;
          glutPostRedisplay();
          break;

      //ESC to Exit
      case 27:
          exit(0);
          break;
      default:
          break;
    }
}
void update(int value) {
	newFood();
	
	glutPostRedisplay(); //Tell GLUT that the display has changed
	
	glutTimerFunc(50000, update, 0);
}
void update2(int value) {
	newFood2();
	
	glutPostRedisplay(); //Tell GLUT that the display has changed
	
	glutTimerFunc(35000, update2, 0);
}
void update3(int value) {
	newFood3();
	
	glutPostRedisplay(); //Tell GLUT that the display has changed
	
	glutTimerFunc(30000, update3, 0);
}
void update4(int value) {
	newFood4();
	
	glutPostRedisplay(); //Tell GLUT that the display has changed
	
	glutTimerFunc(20000, update4, 0);
}

int main(void)
{
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(_w,_h);
    glutInitWindowPosition(80,80);
    glutCreateWindow("Snake Game - (DD)");
    glutSpecialFunc(Special);
    glutKeyboardFunc(keyboard);
    glutDisplayFunc(Display);
    glutReshapeFunc(resize);
	glutTimerFunc(50000, update, 0);
	glutTimerFunc(35000, update2, 0);
	glutTimerFunc(30000, update3, 0);
	glutTimerFunc(20000, update4, 0);
	
    newFood();
	newFood2();
	newFood3();
	newFood4();
    Run(0);
    
    Initialize();
    glutMainLoop();
}
 