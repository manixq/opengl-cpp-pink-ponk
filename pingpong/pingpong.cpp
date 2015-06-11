// pingpong.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "pingpong.h"

#define MAX_LOADSTRING 100
#include    <windows.h>                           // Header File For Windows
#include    <stdio.h>                         // Header File For Standard Input/Output ( ADD )
#include    <stdarg.h>                            // Header File For Variable Argument Routines
#include    <gl\gl.h>                         // Header File For The OpenGL32 Library
#include    <gl\glu.h>                            // Header File For The GLu32 Library
#include    <gl\glaux.h>                          // Header File For The GLaux Library
#include <time.h>      

// Global Variables:
HDC     hDC = NULL;                           // Private GDI Device Context
HGLRC       hRC = NULL;                           // Permanent Rendering Context
HWND        hWnd = NULL;                          // Holds Our Window Handle
HINSTANCE   hInstance;                          // Holds The Instance Of The Application

bool        keys[256];                          // Array Used For The Keyboard Routine
GLfloat LightAmbient[] = { 0.5f, 0.5f, 0.5f, 1.0f };                 // Ambient Light Values 
GLfloat LightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat LightPosition[] = { 0.0f, 3.0f, 4.0f, 0.0f };

GLuint  texture[8];                             // Storage for 3 textures
char* abc[8][14];
int io = 4;
int jo = 4;
GLfloat xball=-0.4f, zball=1.0f;
GLfloat xballv = 0.0f, zballv = 0.001f;
GLfloat ystaly = 0.0f;
GLfloat yzmienny = 0.0f;
GLfloat xstaly = 0.0f;
GLfloat xzmienny = 0.0f;


// Forward declarations of functions included in this code module:

LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);





AUX_RGBImageRec *LoadBMP(char *Filename)                    // Loads A Bitmap Image
{
	FILE *File = NULL;                            // File Handle

	if (!Filename)                              // Make Sure A Filename Was Given
	{
		return NULL;                            // If Not Return NULL
	}

	File = fopen(Filename, "r");                       // Check To See If The File Exists

	if (File)                               // Does The File Exist?
	{
		fclose(File);                           // Close The Handle
		return auxDIBImageLoad(Filename);               // Load The Bitmap And Return A Pointer
	}
	return NULL;                                // If Load Failed Return NULL
}

int LoadGLTextures()                                // Load Bitmaps And Convert To Textures
{
	abc[0][14] = "Data/podl.bmp";
	abc[1][14] = "Data/side.bmp";
	abc[2][14] = "Data/play.bmp";
	abc[3][14] = "Data/ball.bmp";
	abc[4][14] = "Data/ziro.bmp";
	abc[5][14] = "Data/jede.bmp";
	abc[6][14] = "Data/drwa.bmp";
	abc[7][14] = "Data/trzy.bmp";
	int Status = FALSE;                           // Status Indicator
	AUX_RGBImageRec *TextureImage[8];                   // Create Storage Space For The Texture
	memset(TextureImage, 0, sizeof(void *)* 1);                // Set The Pointer To NULL
	for (int i = 0; i < 8; i++)
	if (TextureImage[i] = LoadBMP(abc[i][14]))
	{
		Status = TRUE;                            // Set The Status To TRUE
		glGenTextures(8, &texture[i]);                  // Create Three Textures
		glBindTexture(GL_TEXTURE_2D, texture[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[i]->sizeX, TextureImage[i]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[i]->data);

		
		if (TextureImage[i])                            // If Texture Exists
		{
			if (TextureImage[i]->data)                   // If Texture Image Exists
			{
				free(TextureImage[i]->data);             // Free The Texture Image Memory
			}

			free(TextureImage[i]);                      // Free The Image Structure
		}
		
	}
	else return 0;
	return Status;                              // Return The Status
		
	
}
GLvoid ReSizeGLScene(GLsizei width, GLsizei height)             // Resize And Initialize The GL Window
{
	if (height == 0)                              // Prevent A Divide By Zero By
	{
		height = 1;                           // Making Height Equal One
	}

	glViewport(0, 0, width, height);                    // Reset The Current Viewport
	glMatrixMode(GL_PROJECTION);                        // Select The Projection Matrix
	glLoadIdentity();                           // Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW);                     // Select The Modelview Matrix
	glLoadIdentity();                           // Reset The Modelview Matrix
}
int InitGL(GLvoid)                              // All Setup For OpenGL Goes Here
{
	if (!LoadGLTextures()) return 0; //jak sie tekstury nie zaladuja
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);                        // Enables Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);                   // Black Background
	glClearDepth(1.0f);     
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);  // Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);                        // Enables Depth Testing
	glDepthFunc(GL_LEQUAL);                         // The Type Of Depth Test To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);          // Really Nice Perspective Calculations
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);
	glEnable(GL_LIGHT1);                            // Enable Light
	return TRUE;                                // Initialization Went OK
}
int DrawGLScene(GLvoid)                             // Here's Where We Do All The Drawing
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);         // Clear The Screen And The Depth Buffer
	glLoadIdentity();             
	glColor3f(1.0f, 1.0f, 1.0f); // Reset The Current Modelview Matrix
	glTranslatef(0.0f, 0.0f, -4.0f);                      // Translate Into/Out Of The Screen By z
	glRotatef(-10.0f, 1.0f, 0.0f, 0.0f);//x axis by -150
	
	

	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glBegin(GL_QUADS);
	// Front Face
	glNormal3f(0.0f, 0.0f, 1.0f);                  // Normal Pointing Towards Viewer
	glTexCoord2f(0.0f, 0.0f);	glVertex3f(-1.0f, -1.0f, 4.0f);  // Point 1 (Front)
	glTexCoord2f(1.0f, 0.0f);	glVertex3f(1.0f, -1.0f, 4.0f);  // Point 2 (Front)
	glTexCoord2f(1.0f, 1.0f);	glVertex3f(1.0f, 4.0f, 4.0f);  // Point 3 (Front)
	glTexCoord2f(0.0f, 1.0f); 	glVertex3f(-1.0f, 4.0f, 4.0f);  // Point 4 (Front)
	// Back Face
	glNormal3f(0.0f, 0.0f, -1.0f);                  // Normal Pointing Away From Viewer
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);  // Point 1 (Back)
	glTexCoord2f(1.0f, 3.0f); glVertex3f(-1.0f, 4.0f, -1.0f);  // Point 2 (Back)
	glTexCoord2f(0.0f, 3.0f); glVertex3f(1.0f, 4.0f, -1.0f);  // Point 3 (Back)
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);  // Point 4 (Back)
	// Top Face
	glNormal3f(0.0f, 1.0f, 0.0f);                  // Normal Pointing Up
	glTexCoord2f(0.0f, 3.0f); glVertex3f(-1.0f, 4.0f, -1.0f);  // Point 1 (Top)
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 4.0f, 4.0f);  // Point 2 (Top)
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 4.0f, 4.0f);  // Point 3 (Top)
	glTexCoord2f(1.0f, 3.0f); glVertex3f(1.0f, 4.0f, -1.0f);  // Point 4 (Top)
	// Right face
	glNormal3f(1.0f, 0.0f, 0.0f);                  // Normal Pointing Right
	glTexCoord2f(3.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);  // Point 1 (Right)
	glTexCoord2f(3.0f, 4.0f); glVertex3f(1.0f, 4.0f, -1.0f);  // Point 2 (Right)
	glTexCoord2f(0.0f, 4.0f); glVertex3f(1.0f, 4.0f, 4.0f);  // Point 3 (Right)
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 4.0f);  // Point 4 (Right)
	// Left Face
	glNormal3f(-1.0f, 0.0f, 0.0f);                  // Normal Pointing Left
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);  // Point 1 (Left)
	glTexCoord2f(3.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 4.0f);  // Point 2 (Left)
	glTexCoord2f(3.0f, 4.0f); glVertex3f(-1.0f, 4.0f, 4.0f);  // Point 3 (Left)
	glTexCoord2f(0.0f, 4.0f); glVertex3f(-1.0f, 4.0f, -1.0f);  // Point 4 (Left)
	glEnd();	
	// Bottom Face
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glBegin(GL_QUADS);
	
	
	glNormal3f(0.0f, -1.0f, 0.0f);                  // Normal Pointing Down
	glTexCoord2f(2.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);  // Point 1 (Bottom)
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f);  // Point 2 (Bottom)
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 4.0f);  // Point 3 (Bottom)
	glTexCoord2f(2.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 4.0f);  // Point 4 (Bottom)
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[2]);
	                          // Reset The Current Modelview Matrix
	
	glPushMatrix();//stos?

	glTranslatef(xstaly, 0.0f, 0.0f);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 1.0f);                  // Normal Pointing Towards Viewer
	glTexCoord2f(0.0f, 0.0f);	glVertex3f(-0.5f, -1.1f, -0.8f);  // Point 1 (Front)
	glTexCoord2f(1.0f, 0.0f);	glVertex3f(-0.3f, -1.1f, -0.8f);  // Point 2 (Front)
	glTexCoord2f(1.0f, 1.0f);	glVertex3f(-0.3f, -0.9f, -0.8f);  // Point 3 (Front)
	glTexCoord2f(0.0f, 1.0f); 	glVertex3f(-0.5f, -0.9f, -0.8f);  // Point 4 (Front)

	glTexCoord2f(0.0f, 0.0f);	glVertex3f(-0.5f, -1.1f, -0.8f);  // Point 1 (left)
	glTexCoord2f(1.0f, 0.0f);	glVertex3f(-0.3f, -1.1f, -0.6f);  // Point 2 
	glTexCoord2f(1.0f, 1.0f);	glVertex3f(-0.5f, -0.9f, -0.6f);  // Point 3 
	glTexCoord2f(0.0f, 1.0f); 	glVertex3f(-0.5f, -0.9f, -0.8f);  // Point 4 

	glTexCoord2f(0.0f, 0.0f);	glVertex3f(-0.5f, -1.1f, -0.8f);  // Point 1 (back)
	glTexCoord2f(1.0f, 0.0f);	glVertex3f(-0.3f, -1.1f, -0.8f);  // Point 2 
	glTexCoord2f(1.0f, 1.0f);	glVertex3f(-0.3f, -0.9f, -0.6f);  // Point 3 
	glTexCoord2f(0.0f, 1.0f); 	glVertex3f(-0.5f, -0.9f, -0.6f);  // Point 4 

	glTexCoord2f(0.0f, 0.0f);	glVertex3f(-0.5f, -0.9f, -0.8f);  // Point 1 top
	glTexCoord2f(1.0f, 0.0f);	glVertex3f(-0.3f, -0.9f, -0.8f);  // Point 2 
	glTexCoord2f(1.0f, 1.0f);	glVertex3f(-0.3f, -0.9f, -0.6f);  // Point 3 
	glTexCoord2f(0.0f, 1.0f); 	glVertex3f(-0.5f, -0.9f, -0.6f);  // Point 4 

	glTexCoord2f(0.0f, 0.0f);	glVertex3f(-0.3f, -1.1f, -0.8f);  // Point 1 right
	glTexCoord2f(1.0f, 0.0f);	glVertex3f(-0.3f, -1.1f, -0.6f);  // Point 2 
	glTexCoord2f(1.0f, 1.0f);	glVertex3f(-0.3f, -0.9f, -0.6f);  // Point 3 
	glTexCoord2f(0.0f, 1.0f); 	glVertex3f(-0.3f, -0.9f, -0.8f);  // Point 4 
	glEnd();
	xstaly += xzmienny;
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(ystaly, 0.0f, 0.0f);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 1.0f);                  // Normal Pointing Towards Viewer
	glTexCoord2f(0.0f, 0.0f);	glVertex3f(-0.5f, -1.1f, 2.5f);  // Point 1 (Front)
	glTexCoord2f(1.0f, 0.0f);	glVertex3f(-0.3f, -1.1f, 2.5f);  // Point 2 (Front)
	glTexCoord2f(1.0f, 1.0f);	glVertex3f(-0.3f, -0.9f, 2.5f);  // Point 3 (Front)
	glTexCoord2f(0.0f, 1.0f); 	glVertex3f(-0.5f, -0.9f, 2.5f);  // Point 4 (Front)

	glTexCoord2f(0.0f, 0.0f);	glVertex3f(-0.5f, -1.1f, 2.5f);  // Point 1 (left)
	glTexCoord2f(1.0f, 0.0f);	glVertex3f(-0.3f, -1.1f, 2.3f);  // Point 2 
	glTexCoord2f(1.0f, 1.0f);	glVertex3f(-0.5f, -0.9f, 2.3f);  // Point 3 
	glTexCoord2f(0.0f, 1.0f); 	glVertex3f(-0.5f, -0.9f, 2.5f);  // Point 4 

	glTexCoord2f(0.0f, 0.0f);	glVertex3f(-0.5f, -1.1f, 2.5f);  // Point 1 (back)
	glTexCoord2f(1.0f, 0.0f);	glVertex3f(-0.3f, -1.1f, 2.5f);  // Point 2 
	glTexCoord2f(1.0f, 1.0f);	glVertex3f(-0.3f, -0.9f, 2.3f);  // Point 3 
	glTexCoord2f(0.0f, 1.0f); 	glVertex3f(-0.5f, -0.9f, 2.3f);  // Point 4 

	glTexCoord2f(0.0f, 0.0f);	glVertex3f(-0.5f, -0.9f, 2.5f);  // Point 1 top
	glTexCoord2f(1.0f, 0.0f);	glVertex3f(-0.3f, -0.9f, 2.5f);  // Point 2 
	glTexCoord2f(1.0f, 1.0f);	glVertex3f(-0.3f, -0.9f, 2.3f);  // Point 3 
	glTexCoord2f(0.0f, 1.0f); 	glVertex3f(-0.5f, -0.9f, 2.3f);  // Point 4 

	glTexCoord2f(0.0f, 0.0f);	glVertex3f(-0.3f, -1.1f, 2.5f);  // Point 1 right
	glTexCoord2f(1.0f, 0.0f);	glVertex3f(-0.3f, -1.1f, 2.3f);  // Point 2 
	glTexCoord2f(1.0f, 1.0f);	glVertex3f(-0.3f, -0.9f, 2.3f);  // Point 3 
	glTexCoord2f(0.0f, 1.0f); 	glVertex3f(-0.3f, -0.9f, 2.5f);  // Point 4 
	glEnd();
	ystaly += yzmienny;
	glPopMatrix();

	glPushMatrix();//stos?
	glTranslatef(0.0f, 0.0f, -0.0f);
	glBindTexture(GL_TEXTURE_2D, texture[io]);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 1.0f);                  // Normal Pointing Towards Viewer
	glTexCoord2f(0.0f, 0.0f);	glVertex3f(-0.8f, 0.9f, -0.8f);  // Point 1 (Front)
	glTexCoord2f(1.0f, 0.0f);	glVertex3f(-0.3f, 0.9f, -0.8f);  // Point 2 (Front)
	glTexCoord2f(1.0f, 1.0f);	glVertex3f(-0.3f, 1.5f, -0.8f);  // Point 3 (Front)
	glTexCoord2f(0.0f, 1.0f); 	glVertex3f(-0.8f, 1.5f, -0.8f);  // Point 4 (Front)
	glEnd();
	glPopMatrix();

	glPushMatrix();//stos?
	glTranslatef(0.0f, 0.0f, -0.0f);
	glBindTexture(GL_TEXTURE_2D, texture[jo]);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 1.0f);                  // Normal Pointing Towards Viewer
	glTexCoord2f(0.0f, 0.0f);	glVertex3f(0.3f, 0.9f, -0.8f);  // Point 1 (Front)
	glTexCoord2f(1.0f, 0.0f);	glVertex3f(0.8f, 0.9f, -0.8f);  // Point 2 (Front)
	glTexCoord2f(1.0f, 1.0f);	glVertex3f(0.8f, 1.5f, -0.8f);  // Point 3 (Front)
	glTexCoord2f(0.0f, 1.0f); 	glVertex3f(0.3f, 1.5f, -0.8f);  // Point 4 (Front)
	glEnd();
	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glPushMatrix();
	GLUquadric *shape = gluNewQuadric();
	
	glTranslatef(xball, -0.9f, zball);

	gluQuadricTexture(shape, 1);
	gluSphere(shape,0.05, 20, 20);
	gluDeleteQuadric(shape);
	zball += zballv;
	xball += xballv;
	if ((zball > 2.3f) && ((xball < (ystaly-0.1)) && (xball > (ystaly - 0.5f)))&&(zball < 2.5f))
	{
		zballv = -zballv;
		if (zballv>-0.02)
		zballv = zballv-0.0005f;
		if (xballv >= 0)
			xballv = (xballv+3)/10000;
	}
	if ((zball < -0.6f) && ((xball < (xstaly-0.3)) && (xball >(xstaly - 0.5f)))&&(zball > -0.8f))
	{

		zballv = -zballv;
		if (zballv<0.02)
		zballv = zballv + 0.0005f;
		if (xballv >= 0)
			xballv = -(xballv + 5) / 10000;
	}
	if ((xball > 0.9) || (xball < -0.9))
	{
		xballv = -xballv;
	}
	glPopMatrix();
	
	if (zball>6){
		
		xball = -0.4f;
		zball = 1.0f;
		xballv = 0.0f;
		zballv = 0.001f;
		if (jo >= 7)
		{
			zballv = 0.0f;
			jo = jo - 1;
		}
		jo=jo+1;
	}
	if (zball<-2){
		
		xball = -0.4f;
		zball = 1.0f;
		xballv = 0.0f;
		zballv = -0.001f;
		if (io >= 7)
		{
			zballv = 0.0f;
			io = io - 1;
		}
		io=io+1;
	}
	
	return TRUE;                                // Everything Went OK
}
BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag)
{
	GLuint      PixelFormat;                        // Holds The Results After Searching For A Match
	WNDCLASS    wc;                         // Windows Class Structure
	DWORD       dwExStyle;                      // Window Extended Style
	DWORD       dwStyle;                        // Window Style
	RECT WindowRect;                            // Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left = (long)0;                        // Set Left Value To 0
	WindowRect.right = (long)width;                       // Set Right Value To Requested Width
	WindowRect.top = (long)0;                         // Set Top Value To 0
	WindowRect.bottom = (long)height;                     // Set Bottom Value To Requested Height
	hInstance = GetModuleHandle(NULL);            // Grab An Instance For Our Window
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;       // Redraw On Move, And Own DC For Window
	wc.lpfnWndProc = (WNDPROC)WndProc;                // WndProc Handles Messages
	wc.cbClsExtra = 0;                        // No Extra Window Data
	wc.cbWndExtra = 0;                        // No Extra Window Data
	wc.hInstance = hInstance;                    // Set The Instance
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);          // Load The Default Icon
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);          // Load The Arrow Pointer
	wc.hbrBackground = NULL;                     // No Background Required For GL
	wc.lpszMenuName = NULL;                     // We Don't Want A Menu
	wc.lpszClassName = "OpenGL";                 // Set The Class Name

	if (!RegisterClass(&wc)) return 0;//attenot to register windows class

	
	dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;           // Window Extended Style
	dwStyle = WS_OVERLAPPEDWINDOW;                    // Windows Style

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);     // Adjust Window To True Requested Size

	if(!(hWnd = CreateWindowEx(dwExStyle,              // Extended Style For The Window
		"OpenGL",               // Class Name
		title,                  // Window Title
		WS_CLIPSIBLINGS |           // Required Window Style
		WS_CLIPCHILDREN |           // Required Window Style
		dwStyle,                // Selected Window Style
		0, 0,                   // Window Position
		WindowRect.right - WindowRect.left,   // Calculate Adjusted Window Width
		WindowRect.bottom - WindowRect.top,   // Calculate Adjusted Window Height
		NULL,                   // No Parent Window
		NULL,                   // No Menu
		hInstance,              // Instance
		NULL))) return 0;                 // Don't Pass Anything To WM_CREATE

	static  PIXELFORMATDESCRIPTOR pfd =                  // pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),                  // Size Of This Pixel Format Descriptor
		1,                              // Version Number
		PFD_DRAW_TO_WINDOW |                        // Format Must Support Window
		PFD_SUPPORT_OPENGL |                        // Format Must Support OpenGL
		PFD_DOUBLEBUFFER,                       // Must Support Double Buffering
		PFD_TYPE_RGBA,                          // Request An RGBA Format
		bits,                               // Select Our Color Depth
		0, 0, 0, 0, 0, 0,                       // Color Bits Ignored
		0,                              // No Alpha Buffer
		0,                              // Shift Bit Ignored
		0,                              // No Accumulation Buffer
		0, 0, 0, 0,                         // Accumulation Bits Ignored
		16,                             // 16Bit Z-Buffer (Depth Buffer)
		0,                              // No Stencil Buffer
		0,                              // No Auxiliary Buffer
		PFD_MAIN_PLANE,                         // Main Drawing Layer
		0,                              // Reserved
		0, 0, 0                             // Layer Masks Ignored
	};
	if (!(hDC = GetDC(hWnd))) return FALSE;//did we get a device context?
	if (!(PixelFormat = ChoosePixelFormat(hDC, &pfd))) return FALSE;//did windows find a matching pixel format?
	if (!SetPixelFormat(hDC, PixelFormat, &pfd)) return FALSE; //are we able to set the pixel format
	if (!(hRC = wglCreateContext(hDC))) return FALSE;//are we able to get a rendering context?
	if (!wglMakeCurrent(hDC, hRC)) return FALSE;//activate the rendering context

	ShowWindow(hWnd, SW_SHOW);                       // Show The Window
	SetForegroundWindow(hWnd);                      // Slightly Higher Priority
	SetFocus(hWnd);                             // Sets Keyboard Focus To The Window
	ReSizeGLScene(width, height);                       // Set Up Our Perspective GL Screen
	InitGL();
	return TRUE;
}


//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND    hWnd,                   // Handle For This Window
	UINT    uMsg,                   // Message For This Window
	WPARAM  wParam,                 // Additional Message Information
	LPARAM  lParam)                 // Additional Message Information
{
	switch (uMsg)                               // Check For Windows Messages
	{		
		case WM_SYSCOMMAND:                     // Intercept System Commands
		{
			switch (wParam)                     // Check System Calls
			{
				case SC_SCREENSAVE:             // Screensaver Trying To Start?
				case SC_MONITORPOWER:               // Monitor Trying To Enter Powersave?
				return 0;                   // Prevent From Happening
			}
			break;                          // Exit
		}
		case WM_CLOSE:                          // Did We Receive A Close Message?
		{
			PostQuitMessage(0);                 // Send A Quit Message
			return 0;                       // Jump Back
		}
		case WM_KEYDOWN:                        // Is A Key Being Held Down?
		{
			keys[wParam] = TRUE;                    // If So, Mark It As TRUE
			return 0;                       // Jump Back
		}
		case WM_KEYUP:                          // Has A Key Been Released?
		{
			keys[wParam] = FALSE;                   // If So, Mark It As FALSE
			return 0;                       // Jump Back
		}
		case WM_SIZE:                           // Resize The OpenGL Window
		{
			ReSizeGLScene(LOWORD(lParam), HIWORD(lParam));       // LoWord=Width, HiWord=Height
			return 0;                       // Jump Back
		}
	}
	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}



int WINAPI WinMain(HINSTANCE   hInstance,              // Instance
	HINSTANCE   hPrevInstance,              // Previous Instance
	LPSTR       lpCmdLine,              // Command Line Parameters
	int     nCmdShow)               // Window Show State
{
	MSG msg;                                // Windows Message Structure
	BOOL    done = FALSE;                         // Bool Variable To Exit Loop
	if (!CreateGLWindow("pingpong", 640, 480, 16,false)) return 0; //create opengl window
	while (!done)                                // Loop That Runs Until done=TRUE
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))           // Is There A Message Waiting?
		{
			if (msg.message == WM_QUIT)               // Have We Received A Quit Message?
			{
				done = TRUE;                  // If So done=TRUE
			}
			else                            // If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);             // Translate The Message
				DispatchMessage(&msg);              // Dispatch The Message
			}
		}
		else                                // If There Are No Messages
		{
			DrawGLScene();              // Draw The Scene
			SwapBuffers(hDC);           // Swap Buffers (Double Buffering)
			if ((xstaly <= 1.25) && (keys['D']))
				xzmienny = 0.01f;
			else
			if (keys['A'] && (xstaly >= -0.45))
			{
				xzmienny = -0.01f;
			}
			else
				xzmienny = 0.0f;


			if ((ystaly <= 1.25)  && (keys[VK_RIGHT]))
				yzmienny = 0.01f;
			else
			if (keys[VK_LEFT] && (ystaly >= -0.45))
			{
				yzmienny = -0.01f;
			}
			else
				yzmienny = 0.0f;

			
		

		}
		
	}
	
	return (msg.wParam);                            // Exit The Program
}
