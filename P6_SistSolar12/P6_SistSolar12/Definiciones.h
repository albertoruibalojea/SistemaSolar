//Alberto Ruibal Ojea

//Variables
#define MYTIEMPO 49
static float angulo = 0.0175;
bool activarOrbitas = 1;
int camara;


typedef struct {
	float x;
	float y;
	float z;
} punto;

typedef struct {
	float x;
	float y;
	float z;
} vector;

typedef struct {
	float distancia;
	float velocidad_trans;
	float angulo_trans;
	float velocidad_rot;
	float angulo_rot;
	int tamano;
	GLint textura;
	int listarender;
} astro;

//Funciones de la cámara
void myCamara();
void myTeclado(unsigned char tras, int x, int y);
void myTeclasespeciales(int cursor, int x, int y);
void myEjes();
extern void myTelescopio(float distancia, float angulo, float distanciaAstro, float anguloAstro);
extern void myTelescopioToSatellite(float distancia, float angulo, float distanciaAstro, float anguloAstro);