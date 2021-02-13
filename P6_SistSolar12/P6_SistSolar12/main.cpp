//Alberto Ruibal Ojea

#include <windows.h>	//Inclusion de variables de windows

#include <glut.h>
#include <gl.h>
#include <glu.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "Definiciones.h"
// Es necesario para que el stb_image funcione
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define PI 3.14f

const int W_WIDTH = 1024;	 //Ancho de la ventana
const int W_HEIGHT = 720;		//Alto de la ventana

//Funciones externas auxiliares
extern void myCamara();
extern void myTelescopio(float distancia, float angulo, float distanciaAstro, float anguloAstro);
extern void myTelescopioToSatellite(float distancia, float angulo, float distanciaAstro, float anguloAstro);
extern void myTeclado(unsigned char tras, int x, int y);
extern void myTeclasespeciales(int cursor, int x, int y);
extern bool activarOrbitas;
extern int camara = 1;
extern GLuint myEsfera();

//Funciones y variables globales
GLuint stars;
GLuint listaOrbitas;
GLint listaEsfera;
void Resize(GLsizei w, GLsizei h);
void menu(void);
void setMenu(int opcion);
void genStars();
void printConsoleMenu();

//Variables de iluminación
GLfloat Ambient[]  =  { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat Diffuse[]  =  { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat Specular[] =  { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat SpecRef[]  =  { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat LuzPos[]   =  { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat SpotDir[]  =  { 1.0f, 1.0f, 1.0f };

//Declaración de los astros
astro sol		= { 0,	   0, 0, 10, 0, 35, 0, 0 };
astro mercurio	= { 45,   10, 0, 50, 0,  4, 0, 0 };
astro venus		= { 60,    7, 0, 40, 0,  6, 0, 0 };
astro tierra  	= { 75,    5, 0, 10, 0,  8, 0, 0 };
astro luna		= { 10,   10, 0, 10, 0,  2, 0, 0 };
astro iss		= { 11,   10, 0, 20, 0,  1, 0, 0 };
astro marte		= { 90,  4.5, 0, 10, 0,  6, 0, 0 };
astro jupiter	= { 130, 1.7, 0,  4, 0, 20, 0, 0 };
astro saturno	= { 165, 1.4, 0,  5, 0, 10, 0, 0 };
astro urano		= { 200, 1.2, 0,  6, 0, 13, 0, 0 };
astro neptuno	= { 230, 1.0, 0,  5, 0, 10, 0, 0 };




/*	*	*	*	* FUNCIONES INCIALIZACIÓN	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*/
GLuint Carga_Texturas(const char *archivo) {
	// load and create a texture 
	// -------------------------

	GLuint textura;
	unsigned char *data;
	//if (data) stbi_image_free(data);

	glGenTextures(1, &textura);
	glBindTexture(GL_TEXTURE_2D, textura); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
	data = stbi_load(archivo, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		//gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, width, height, GL_RGB, GL_UNSIGNED_BYTE, data); //con mimap 

	}
	else
	{
		//std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(data);

	return textura;



}

void initTextures(void) {
	sol.textura		 = Carga_Texturas("sun.bmp");
	mercurio.textura = Carga_Texturas("mercury.bmp");
	venus.textura	 = Carga_Texturas("venus.bmp");
	tierra.textura	 = Carga_Texturas("earth.bmp");
	luna.textura	 = Carga_Texturas("moon.jpg");
	//iss.textura		 = Carga_Texturas("iss.jpg");
	marte.textura	 = Carga_Texturas("mars.bmp");
	jupiter.textura	 = Carga_Texturas("jupiter.bmp");
	saturno.textura	 = Carga_Texturas("saturn.bmp");
	urano.textura	 = Carga_Texturas("uranus.bmp");
	neptuno.textura	 = Carga_Texturas("neptune.bmp");
	stars			 = Carga_Texturas("stars.bmp");
}

void initOrbitas(void) {
	listaOrbitas = glGenLists(1);
	glNewList(listaOrbitas, GL_COMPILE);

	glBegin(GL_LINE_LOOP);
	glColor3f(1.0f, 1.0f, 1.0f);

	float theta = -PI;
	while (theta < PI) {
		glVertex3f(20 * sin(theta), 0, 20 * cos(theta));
		theta += 0.1;
	}

	glEnd();
	glEndList();
}

void initLuces(void) {
	//Iluminación
	glLightfv(GL_LIGHT0, GL_AMBIENT, Ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, Diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, Specular);
	glLightfv(GL_LIGHT0, GL_POSITION, LuzPos);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, SpotDir);

	//Efectos de foco
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 179.0f);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 1.0f);

	//Activamos las luces
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

	//Popiedad luminosa de los materiales
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	glMaterialfv(GL_FRONT, GL_SPECULAR, SpecRef);
	glMateriali(GL_FRONT, GL_SHININESS, 1);
}



/*	*	*	*	* USO DEL TELESCOPIO		*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*/
void callCamara(void) {

	//Ponemos el telescopio a funcionar
	switch (camara) {
	case 1:
		myCamara();
		break;
	case 2:
		myTelescopio(tierra.distancia, tierra.angulo_trans, sol.distancia, sol.angulo_trans);
		break;
	case 3:
		myTelescopio(tierra.distancia, tierra.angulo_trans, mercurio.distancia, mercurio.angulo_trans);
		break;
	case 4:
		myTelescopio(tierra.distancia, tierra.angulo_trans, venus.distancia, venus.angulo_trans);
		break;
	case 5:
		myTelescopioToSatellite(tierra.distancia, tierra.angulo_trans, (luna.distancia + tierra.distancia), (luna.angulo_trans + tierra.angulo_trans));
		break;
	case 6:
		glRotatef(-90.0, 1.0, 0.0, 0.0);
		myTelescopioToSatellite(tierra.distancia, tierra.angulo_trans, (iss.distancia + tierra.distancia), (iss.angulo_trans + tierra.angulo_trans));
		break;
	case 7:
		myTelescopio(tierra.distancia, tierra.angulo_trans, marte.distancia, marte.angulo_trans);
		break;
	case 8:
		myTelescopio(tierra.distancia, tierra.angulo_trans, jupiter.distancia, jupiter.angulo_trans);
		break;
	case 9:
		myTelescopio(tierra.distancia, tierra.angulo_trans, saturno.distancia, saturno.angulo_trans);
		break;
	case 10:
		myTelescopio(tierra.distancia, tierra.angulo_trans, urano.distancia, urano.angulo_trans);
		break;
	case 11:
		myTelescopio(tierra.distancia, tierra.angulo_trans, neptuno.distancia, neptuno.angulo_trans);
		break;
	}
}



/*	*	*	*	* FUNCIONES DE DIBUJO	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*/
void drawOrbitas(float radio) {
	if (activarOrbitas) {
		glPushMatrix();
			glColor3f(1.0f, 1.0f, 1.0f);
			glScalef(radio / 20.0, radio / 20.0, radio / 20.0);
			glCallList(listaOrbitas);
		glPopMatrix();
	}
}

void myMovimiento(int movimiento) {
	angulo += 0.0175;

	//Sol
	sol.angulo_trans += sol.velocidad_trans;
	if (sol.angulo_trans > 360) sol.angulo_trans -= 360;
	sol.angulo_rot += sol.velocidad_rot;
	if (sol.angulo_rot > 360) sol.angulo_rot -= 360;

	//Mercurio
	mercurio.angulo_trans += mercurio.velocidad_trans;
	if (mercurio.angulo_trans > 360) mercurio.angulo_trans -= 360;
	mercurio.angulo_rot += mercurio.velocidad_rot;
	if (mercurio.angulo_rot > 360) mercurio.angulo_rot -= 360;

	//Venus
	venus.angulo_trans += venus.velocidad_trans;
	if (venus.angulo_trans > 360) venus.angulo_trans -= 360;
	venus.angulo_rot += venus.velocidad_rot;
	if (venus.angulo_rot > 360) venus.angulo_rot -= 360;

	//Tierra
	tierra.angulo_trans += tierra.velocidad_trans;
	if (tierra.angulo_trans > 360) tierra.angulo_trans -= 360;
	tierra.angulo_rot += tierra.velocidad_rot;
	if (tierra.angulo_rot > 360) tierra.angulo_rot -= 360;

	//Luna
	luna.angulo_trans += luna.velocidad_trans;
	if (luna.angulo_trans > 360) luna.angulo_trans -= 360;
	luna.angulo_rot += luna.velocidad_rot;
	if (luna.angulo_rot > 360) luna.angulo_rot -= 360;

	//ISS
	iss.angulo_trans += iss.velocidad_trans;
	if (iss.angulo_trans > 360) iss.angulo_trans -= 360;
	iss.angulo_rot += iss.velocidad_rot;
	if (iss.angulo_rot > 360) iss.angulo_rot -= 360;

	//Marte
	marte.angulo_trans += marte.velocidad_trans;
	if (marte.angulo_trans > 360) marte.angulo_trans -= 360;
	marte.angulo_rot += marte.velocidad_rot;
	if (marte.angulo_rot > 360) marte.angulo_rot -= 360;

	//Júpiter
	jupiter.angulo_trans += jupiter.velocidad_trans;
	if (jupiter.angulo_trans > 360) jupiter.angulo_trans -= 360;
	jupiter.angulo_rot += jupiter.velocidad_rot;
	if (jupiter.angulo_rot > 360) jupiter.angulo_rot -= 360;

	//Saturno
	saturno.angulo_trans += saturno.velocidad_trans;
	if (saturno.angulo_trans > 360) saturno.angulo_trans -= 360;
	saturno.angulo_rot += saturno.velocidad_rot;
	if (saturno.angulo_rot > 360) saturno.angulo_rot -= 360;

	//Urano
	urano.angulo_trans += urano.velocidad_trans;
	if (urano.angulo_trans > 360) urano.angulo_trans -= 360;
	urano.angulo_rot += urano.velocidad_rot;
	if (urano.angulo_rot > 360) urano.angulo_rot -= 360;

	//Neptuno
	neptuno.angulo_trans += neptuno.velocidad_trans;
	if (neptuno.angulo_trans > 360) neptuno.angulo_trans -= 360;
	neptuno.angulo_rot += neptuno.velocidad_rot;
	if (neptuno.angulo_rot > 360) neptuno.angulo_rot -= 360;


	glutPostRedisplay();
	glutTimerFunc(MYTIEMPO, myMovimiento, 1);
}

void myDisplay(void) {

	//Clear the windows with the current clearing color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	callCamara();

	//Guardar la matriz y hacer la rotación
	glMatrixMode(GL_MODELVIEW);
	//Inicializamos la matriz a la identidad
	glLoadIdentity();

	// Activamos y configuramos Blend
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//Generamos estrellas hasta las texturas
	//genStars();
	

	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);

	//Sol
	glPushMatrix();
		glRotatef(sol.angulo_trans, 0.0, 1.0, 0.0);
		glTranslatef(sol.distancia, 0.0, 0.0);
		glRotatef(sol.angulo_rot, 0, 1, 0);
		myEjes();
		glColor3f(1.0f, 1.0f, 1.0f);
		glScalef(sol.tamano, sol.tamano, sol.tamano);
		glBindTexture(GL_TEXTURE_2D, sol.textura);
		glCallList(sol.listarender);
		glBindTexture(GL_TEXTURE_2D, 0);		
	glPopMatrix();


	glEnable(GL_LIGHTING);	

	//Mercurio
	glPushMatrix();
		drawOrbitas(45);
		glRotatef(mercurio.angulo_trans, 0.0, 1.0, 0.0);
		glTranslatef(mercurio.distancia, 0.0, 0.0);
		glRotatef(mercurio.angulo_rot, 0, 1, 0);
		myEjes();
		glColor3f(0.5f, 0.3f, 0.0f);
		glScalef(mercurio.tamano, mercurio.tamano, mercurio.tamano);
		glBindTexture(GL_TEXTURE_2D, mercurio.textura);
		glCallList(mercurio.listarender);
		glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();

	//Venus
	glPushMatrix();
		drawOrbitas(60);
		glRotatef(venus.angulo_trans, 0.0, 1.0, 0.0);
		glTranslatef(venus.distancia, 0.0, 0.0);
		glRotatef(venus.angulo_rot, 0, 1, 0);
		myEjes();
		glColor3f(0.627f, 0.204f, 0.447f);
		glScalef(venus.tamano, venus.tamano, venus.tamano);
		glBindTexture(GL_TEXTURE_2D, venus.textura);
		glCallList(venus.listarender);
		glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();

	//Tierra
	glPushMatrix();
		drawOrbitas(75);
		glRotatef(tierra.angulo_trans, 0.0, 1.0, 0.0);
		glTranslatef(tierra.distancia, 0.0, 0.0);
		glPushMatrix();
			glRotatef(tierra.angulo_rot, 0, 1, 0);
			myEjes();
			glColor3f(0.0f, 0.1f, 0.8f);
			glScalef(tierra.tamano, tierra.tamano, tierra.tamano);
			glBindTexture(GL_TEXTURE_2D, tierra.textura);
			glCallList(tierra.listarender);
			glBindTexture(GL_TEXTURE_2D, 0);
		glPopMatrix();

		//Luna
		glPushMatrix();
			drawOrbitas(8);
			glRotatef(luna.angulo_trans, 0.0, 1.0, 0.0);
			glTranslatef(luna.distancia, 0.0, 0.0);
			glPushMatrix();
				glRotatef(luna.angulo_rot, 0, 1, 0);
				myEjes();
				glColor3f(0.8f, 0.8f, 0.9f);
				glScalef(luna.tamano, luna.tamano, luna.tamano);
				glBindTexture(GL_TEXTURE_2D, luna.textura);
				glCallList(luna.listarender);
				glBindTexture(GL_TEXTURE_2D, 0);
			glPopMatrix();
		glPopMatrix();

		//ISS
		glPushMatrix();
		glRotatef(-90.0, 1.0, 0.0, 0.0);
			drawOrbitas(10);
			glRotatef(iss.angulo_trans, 0.0, 1.0, 0.0);
			glTranslatef(iss.distancia, 0.0, 0.0);
			glPushMatrix();
				glRotatef(iss.angulo_rot, 0, 1, 0);
				myEjes();
				glColor3f(0.8f, 0.8f, 0.7f);
				glRotatef(-90.0, 1.0, 0.0, 0.0);
				glutSolidTetrahedron();
				glScalef(iss.tamano, iss.tamano, iss.tamano);
			glPopMatrix();
		glPopMatrix();

	glPopMatrix();

	//Marte
	glPushMatrix();
		drawOrbitas(90);
		glRotatef(marte.angulo_trans, 0.0, 1.0, 0.0);
		glTranslatef(marte.distancia, 0.0, 0.0);
		glRotatef(marte.angulo_rot, 0, 1, 0);
		myEjes();
		glColor3f(0.7f, 0.2f, 0.0f);
		glScalef(marte.tamano, marte.tamano, marte.tamano);
		glBindTexture(GL_TEXTURE_2D, marte.textura);
		glCallList(marte.listarender);
		glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();

	//Júpiter
	glPushMatrix();
		drawOrbitas(130);
		glRotatef(jupiter.angulo_trans, 0.0, 1.0, 0.0);
		glTranslatef(jupiter.distancia, 0.0, 0.0);
		glRotatef(jupiter.angulo_rot, 0, 1, 0);
		myEjes();
		glColor3f(0.760f, 0.69f, 0.47f);
		glScalef(jupiter.tamano, jupiter.tamano, jupiter.tamano);
		glBindTexture(GL_TEXTURE_2D, jupiter.textura);
		glCallList(jupiter.listarender);
		glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();

	//Saturno
	glPushMatrix();
		drawOrbitas(165);
		glRotatef(saturno.angulo_trans, 0.0, 1.0, 0.0);
		glTranslatef(saturno.distancia, 0.0, 0.0);
		glPushMatrix();
		glRotatef(saturno.angulo_rot, 0, 1, 0);
		myEjes();
		glColor3f(0.9f, 0.5f, 0.0f);
		glScalef(saturno.tamano, saturno.tamano, saturno.tamano);
		glBindTexture(GL_TEXTURE_2D, saturno.textura);
		glCallList(saturno.listarender);
		glBindTexture(GL_TEXTURE_2D, 0);
		glPopMatrix();

		//Anillos de Saturno
		glPushMatrix();
			glColor3f(0.54, 0.4, 0.258);
			glRotatef(-45.0, 1.0, 0.0, 0.0);
			glBindTexture(GL_TEXTURE_2D, saturno.textura);
			glCallList(saturno.listarender);
			glBindTexture(GL_TEXTURE_2D, 0);
			glutWireTorus(2, 15.0, 60, 60);
		glPopMatrix();

	glPopMatrix();

	//Urano
	glPushMatrix();
		drawOrbitas(200);
		glRotatef(urano.angulo_trans, 0.0, 1.0, 0.0);
		glTranslatef(urano.distancia, 0.0, 0.0);
		glRotatef(urano.angulo_rot, 0, 1, 0);
		myEjes();
		glColor3f(0.5f, 0.8f, 0.9f);
		glScalef(urano.tamano, urano.tamano, urano.tamano);
		glBindTexture(GL_TEXTURE_2D, urano.textura);
		glCallList(urano.listarender);
		glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();

	//Neptuno
	glPushMatrix();
		drawOrbitas(230);
		glRotatef(neptuno.angulo_trans, 0.0, 1.0, 0.0);
		glTranslatef(neptuno.distancia, 0.0, 0.0);
		glRotatef(neptuno.angulo_rot, 0, 1, 0);
		myEjes();
		glColor3f(0.4f, 0.7f, 0.9f);
		glScalef(neptuno.tamano, neptuno.tamano, neptuno.tamano);
		glBindTexture(GL_TEXTURE_2D, neptuno.textura);
		glCallList(neptuno.listarender);
		glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();


	//Textura de las estrellas
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, stars);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBegin(GL_POLYGON);
	glTexCoord2f(-1.0, 0.0); glVertex3f(-300, -300, -300);
	glTexCoord2f(2.0, 0.0); glVertex3f(300, -300, -300);
	glTexCoord2f(2.0, 2.0); glVertex3f(300, 300, -300);
	glTexCoord2f(-1.0, 2.0); glVertex3f(-300, 300, -300);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, stars);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(-300, -283, 300);
	glTexCoord2f(8.0, 0.0); glVertex3f(300, -283, 300);
	glTexCoord2f(8.0, 8.0); glVertex3f(300, -283, -300);
	glTexCoord2f(0.0, 8.0); glVertex3f(-300, -283, -300);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	


	glFlush();
	glutSwapBuffers();
}



/*	*	*	*	* MAIN	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*   */
int main(int argc, char **argv) {

	glutInit(&argc, argv);

	glutInitWindowPosition(500, 200);
	glutInitWindowSize(W_WIDTH, W_HEIGHT);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("Sistema Solar - OpenGL 1.2");

	//Detectar profundidad de objetos y no dibujar caras ocultas
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_NORMALIZE);

	//Eventos
	glutKeyboardFunc(myTeclado);
	glutSpecialFunc(myTeclasespeciales);
	glutReshapeFunc(Resize);
	glutDisplayFunc(myDisplay);

	initOrbitas();
	sol.listarender = mercurio.listarender = venus.listarender = tierra.listarender = luna.listarender = iss.listarender = marte.listarender = jupiter.listarender = saturno.listarender = urano.listarender = neptuno.listarender = myEsfera();
	initTextures();


	//Color para llenar los buffers	
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	//	Activa las texturas y el modelo de sombreado suavizado.
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);


	// Empieza en bucle principal
	printConsoleMenu();
	myMovimiento(0);
	menu();
	myCamara();
	initLuces();

	glutMainLoop();
	return 0;
}



/*	*	*	*	* FUNCIONES DEL MENÚ*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*/
void printConsoleMenu() {
	printf("\t\t\t\tBienvenida al Sistema Solar en version 1.2 de OpenGL.\n\n");
	printf("OPCIONES: \n");
	printf("\tTecla 'o': Activar/desactivar orbitas.\n");
	printf("\tBoton derecho con el raton: Menu de opciones multivista.\n");

	printf("\n\n\n\nBy Alberto Ruibal Ojea");

}

void setMenu(int opcion) {
	switch (opcion) {
	case 1:
		camara = 1;
		break;
	case 2:
		camara = 2;
		break;
	case 3:
		camara = 3;
		break;
	case 4:
		camara = 4;
		break;
	case 5:
		camara = 5;
		break;
	case 6:
		camara = 6;
		break;
	case 7:
		camara = 7;
		break;
	case 8:
		camara = 8;
		break;
	case 9:
		camara = 9;
		break;
	case 10:
		camara = 10;
		break;
	case 11:
		camara = 11;
		break;
	}
	glutPostRedisplay();
}

void menu(void) {
	int camaraaux;

	//Creamos las entradas del menú
	camaraaux = glutCreateMenu(setMenu);
	glutAddMenuEntry("Panorámica", 1);
	glutAddMenuEntry("Sol", 2);
	glutAddMenuEntry("Mercurio", 3);
	glutAddMenuEntry("Venus", 4);
	glutAddMenuEntry("Luna", 5);
	glutAddMenuEntry("ISS", 6);
	glutAddMenuEntry("Marte", 7);
	glutAddMenuEntry("Júpiter", 8);
	glutAddMenuEntry("Saturno", 9);
	glutAddMenuEntry("Urano", 10);
	glutAddMenuEntry("Neptuno", 11);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}



/*	*	*	*	* FUNCIONES AUXILIARES	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*/
void Resize(GLsizei w, GLsizei h) {
	GLfloat nRange = 100.0f;

	// Previne las divisiones por cero
	if (h == 0)
		h = 1;


	// Ajusta la vista a las dimensiones de la ventana
	glViewport(0, 0, w, h);

	// Reinicia el sistema de coordenadas
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Establece el volumen de trabajo (Izq, Dch, Arr, Abj, lejos, cerca)
	if (w <= h)
		glOrtho(-nRange, nRange, -nRange * h / w, nRange*h / w, -nRange * 2.0f, nRange*2.0f);
	else
		glOrtho(-nRange * w / h, nRange*w / h, -nRange, nRange, -nRange * 2.0f, nRange*2.0f);

}

void genStars() {
	int i, j, k, h = 0;
	
	srand(time(NULL));
	glBegin(GL_POINTS);

	while (h < 100) {
		i = (rand() % 200);
		j = (rand() % 200);
		k = (rand() % 200);
		glColor3f(1, 1, 1);
		glVertex3f(i, j, k);
		glVertex3f(i, -j, k);
		glColor3f(0, 1, 0);
		glVertex3f(-i, -j, k);
		glColor3f(0, 0, 1);
		glVertex3f(-i, j, -k);
		h++;
	}
	glEnd();

}
