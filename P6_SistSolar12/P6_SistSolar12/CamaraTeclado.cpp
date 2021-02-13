//Alberto Ruibal Ojea

#include <windows.h>	//Inclusion de variables de windows

#include <glut.h>	//Inclusion de GLUT
#include <gl.h>		//Inclusion de GL		 	
#include <glu.h>	
#include <stdio.h>
#include <math.h>	//Inclusion de librerias auxiliares	
#define INCREMENTO .05
#define DISTANCIA 1
#define PI 3.1416

float alpha = 0.4;
float beta = 0.3;
float i = 0, j = 0, k = 0;

int mode = 1;
extern bool activarOrbitas;


void myCamara() {

	//Configuración de la matriz de proyeccion
	glMatrixMode(GL_PROJECTION);
	//La ponemos auno
	glLoadIdentity();
	glOrtho(-240.0, 240.0f, -220.0, 220.0f, -420.0f, 420.0f);
	gluLookAt(((float)DISTANCIA*(float)sin(alpha) * cos(beta)) + i, ((float)DISTANCIA * (float)sin(beta)) + j, ((float)DISTANCIA*(float)cos(alpha)*cos(beta)) + k, i, j, k, 0, mode, 0);
	
}

void myTelescopio(float distancia, float angulo, float distanciaAstro, float anguloAstro) {
	
	//Configuramos la matriz de proyección
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0, 1.0, 0.25f, 1000.0f);
	gluLookAt(distancia*cos(angulo*PI / 180.0), 0, -1 * distancia*sin(angulo*PI / 180.0), distanciaAstro*cos(anguloAstro*PI / 180.0), 0, -1 * distanciaAstro*sin(anguloAstro*PI / 180.0), 0, 1, 0);
}

void myTelescopioToSatellite(float distancia, float angulo, float distanciaAstro, float anguloAstro) {
	
	//Configuramos la matriz de proyección
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0, 1.0, 0.25f, 1000.0f);
	gluLookAt(distancia*cos(angulo*PI / 180.0), 0, -1 * distancia*sin(angulo*PI / 180.0), (distancia*cos(angulo*PI / 180.0) + (distanciaAstro * cos((anguloAstro)*PI / 180.0))), 0, (-1* distancia * sin(angulo*PI / 180.0)) +  (-1 * distanciaAstro*sin((anguloAstro)*PI / 180)), 0, 1, 0);
}

void myTeclado(unsigned char tras, int x, int y)
{
	switch (tras) {
	case 'l':
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	case 'c':
		glDisable(GL_CULL_FACE);
		break;
	case 'r':
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case 'p':
		glEnable(GL_CULL_FACE);
		break;
	case 'o':
		activarOrbitas = !activarOrbitas;
		break;
	default:
		break;
	}
	// Se se modificou algo redebúxase
	glutPostRedisplay();
}

/**************************************
FUNCION DE CONTROL DE TECLAS ESPECIAIS:
**************************************
*/
void myTeclasespeciales(int cursor, int x, int y)
{
	switch (cursor)
	{
		//Traslaciones:
	case GLUT_KEY_F1:
		//Izquierda
		i += INCREMENTO * 10;
		break;
	case GLUT_KEY_F2:
		//Derecha
		i -= INCREMENTO * 10;
		break;
	case GLUT_KEY_F3:
		//Anajo
		j += INCREMENTO * 10;
		break;
	case GLUT_KEY_F4:
		//Arriba
		j -= INCREMENTO * 10;
		break;
	case GLUT_KEY_F5:
		//Alejarse
		k -= INCREMENTO * 10;
		break;
	case GLUT_KEY_F6:
		//Acercarse
		k += INCREMENTO * 10;
		break;

		//Giros:
	case GLUT_KEY_UP:
		beta += INCREMENTO;
		break;
	case GLUT_KEY_DOWN:
		beta -= INCREMENTO;
		break;
	case GLUT_KEY_RIGHT:
		alpha -= INCREMENTO;
		break;
	case GLUT_KEY_LEFT:
		alpha += INCREMENTO;
		break;
	default:
		break;
	}

	//Correcciones de la camara
	if (beta <= 0.05) beta = 0.05;
	if (beta >= 3.1)  beta = 3.1;
	if (alpha >= PI * 2.0 || alpha <= -PI * 2.0) alpha = 0;
	if (beta >= PI * 2.0 || beta <= -PI * 2.0) beta = 0;

	if (beta >= PI / 2 && beta <= ((3 * PI) / 2)) {
		mode = -1;
	} else {
		if (beta <= -PI / 2 && beta >= -((3 * PI) / 2)) {
			mode = -1;
		} else {
			mode = 1;
		}
	}


	glutPostRedisplay();
}

void myEjes() {

	glColor3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(.75f, 0.0f, 0.0f);
	glEnd();


	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, .75f, 0.0f);
	glEnd();


	glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0, .75f);
	glEnd();

	////Diagonal
	//glColor3f(1.0f, 1.0f, 1.0f);
	//glBegin(GL_LINES);
	//glVertex3f(0.0f, 0.0f, 0.0f);
	//glVertex3f(.35f, .35, .35f);
	//glEnd();

}