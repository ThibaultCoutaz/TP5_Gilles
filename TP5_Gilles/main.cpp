/*
 *	Tableau des points permettant de gérer les points de controles
 * On sélectionne le point en se déplaçant avec + et -, ...
 * On sélectionne ensuite si on veut faire monter, descendre amener vers la gauche ou la droite le point.
 *   d : translation à droite
 *   q : à gauche
 *   z : en haut
 *   s : en bas
 *   
 */



#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "struct.h"
#include <glut.h>

/* au cas ou M_PI ne soit defini */
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define ESC 27

float tx=-5.0;
float ty=-5.0;

// Tableau des points de contrôles en global ...
point3 TabPC[4];
point3 TabPC2[4];
// Ordre de la courbre  : Ordre
// Degré de la courbe = Ordre - 1
int OrdreTotale = 7;
int OrdrePC = 4;
int OrdrePC2 = 5;




// Point de controle selectionné
int numPoint = 0;

// Fonction Factorielle
float fact(int n)
{
	if (n > 1)
		return n*fact(n - 1);
	else
		return 1;
}

float Bernstein(int i, int n, float t)
{
	return fact(n) / (fact(i)*fact(n - i))*std::pow(t,i)*std::pow(1 - t, n - i);
}

/* Courbe d'Hermite */
void Hermite(point3 p0, point3 p1, point3 v0, point3 v1){


	float i = 0;
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINE_STRIP);
	while (i < 1){

		float i2 = i*i;
		float i3 = i*i*i;

		float x = (2 * i3 - 3 * i2 + 1)*p0.x + (-2 * i3 + 3 * i2)*p1.x + (i3 - 2 * i2 + i)*v0.x + (i3 - i2)*v1.x;
		float y = (2 * i3 - 3 * i2 + 1)*p0.y + (-2 * i3 + 3 * i2)*p1.y + (i3 - 2 * i2 + i)*v0.y + (i3 - i2)*v1.y;
		float z = (2 * i3 - 3 * i2 + 1)*p0.z + (-2 * i3 + 3 * i2)*p1.z + (i3 - 2 * i2 + i)*v0.z + (i3 - i2)*v1.z;

		glVertex3f(x, y, 0);
		i += 0.01;
	}
	glEnd();
}



/* initialisation d'OpenGL*/
static void init()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	
	// Initialisation des points de contrôles
	TabPC[0] = point3(-2,-2,0);
	TabPC[1] = point3(-1, 1, 0);
	TabPC[2] = point3(1, 1, 0);
	TabPC[3] = point3(2, -2, 0);


	TabPC2[0] = point3(1, 1, 0);
	TabPC2[1] = point3(2,-2,0);
	TabPC2[2] = point3(3, -2, 0);
	TabPC2[3] = point3(1, -4, 0);
	TabPC2[4] = point3(-2, -3, 0);

}

void DoubleBezier()
{
	float t = 0;
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINE_STRIP);
	while (t < 1){

		float x = 0, y = 0, z = 0;
		for (int i = 0; i < OrdrePC; i++){
			x += TabPC[i].x* Bernstein(i, OrdrePC - 1, t);
			y += TabPC[i].y* Bernstein(i, OrdrePC - 1, t);
			z += TabPC[i].z* Bernstein(i, OrdrePC - 1, t);
		}

		glVertex3f(x, y, z);
		t += 0.01;
	}
	glEnd();

	// Dessiner ici la courbe de Bézier.
	// Vous devez avoir implémenté Bernstein précédemment.*
	t = 0;
	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_LINE_STRIP);
	while (t < 1){

		float x = 0, y = 0, z = 0;
		for (int i = 0; i < OrdrePC2; i++){
			x += TabPC2[i].x* Bernstein(i, OrdrePC2 - 1, t);
			y += TabPC2[i].y* Bernstein(i, OrdrePC2 - 1, t);
			z += TabPC2[i].z* Bernstein(i, OrdrePC2 - 1, t);
		}

		glVertex3f(x, y, z);
		t += 0.01;
	}
	glEnd();
}

/* Dessin */
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//glTranslatef(tx,ty,0.0);

	//Hermite(point3(0, 0, 0), point3(2, 0, 0), point3(8, 8, 0), point3(8, -8, 0));

	point3 Ptemp;

	if (numPoint < 2){
		TabPC[numPoint] = TabPC[numPoint] + point3(tx, ty, 0);
	}
	else if (numPoint == 2) {
		TabPC[numPoint] = TabPC[numPoint] + point3(tx, ty, 0);
		TabPC2[0] = TabPC2[0] + point3(tx, ty, 0);
	}
	else if ( numPoint == 3){
		TabPC[numPoint] = TabPC[numPoint] + point3(tx, ty, 0);
		TabPC2[1] = TabPC2[1] + point3(tx, ty, 0);
	}
	else{
		TabPC2[numPoint - 2] = TabPC2[numPoint - 2] + point3(tx, ty, 0);
	}
	// Enveloppe des points de controles
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < OrdrePC; i++)
	{
		glVertex3f(TabPC[i].x, TabPC[i].y, TabPC[i].z);
	}
	glEnd();

	// Enveloppe des points de controles2
	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < OrdrePC2; i++)
	{
		glVertex3f(TabPC2[i].x, TabPC2[i].y, TabPC2[i].z);
	}
	glEnd();


	// Affichage du point de controle courant
	// On se déplace ensuite avec + et - 
	// ° d'un point de contrôle au suivant (+)
	// ° d'un point de contrôle au précédent (-)
	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_LINE_LOOP);
		if (numPoint < 3){
			glVertex3f(TabPC[numPoint].x + 0.1, TabPC[numPoint].y + 0.1, TabPC[numPoint].z);
			glVertex3f(TabPC[numPoint].x + 0.1, TabPC[numPoint].y - 0.1, TabPC[numPoint].z);
			glVertex3f(TabPC[numPoint].x - 0.1, TabPC[numPoint].y - 0.1, TabPC[numPoint].z);
			glVertex3f(TabPC[numPoint].x - 0.1, TabPC[numPoint].y + 0.1, TabPC[numPoint].z);
		}
		else if (numPoint == 2){
			glVertex3f(TabPC[numPoint].x + 0.1, TabPC[numPoint].y + 0.1, TabPC[numPoint].z);
			glVertex3f(TabPC[numPoint].x + 0.1, TabPC[numPoint].y - 0.1, TabPC[numPoint].z);
			glVertex3f(TabPC[numPoint].x - 0.1, TabPC[numPoint].y - 0.1, TabPC[numPoint].z);
			glVertex3f(TabPC[numPoint].x - 0.1, TabPC[numPoint].y + 0.1, TabPC[numPoint].z);

			glVertex3f(TabPC2[0].x + 0.1, TabPC2[0].y + 0.1, TabPC2[0].z);
			glVertex3f(TabPC2[0].x + 0.1, TabPC2[0].y - 0.1, TabPC2[0].z);
			glVertex3f(TabPC2[0].x - 0.1, TabPC2[0].y - 0.1, TabPC2[0].z);
			glVertex3f(TabPC2[0].x - 0.1, TabPC2[0].y + 0.1, TabPC2[0].z);
		}
		else if (numPoint == 3){
			glVertex3f(TabPC[numPoint].x + 0.1, TabPC[numPoint].y + 0.1, TabPC[numPoint].z);
			glVertex3f(TabPC[numPoint].x + 0.1, TabPC[numPoint].y - 0.1, TabPC[numPoint].z);
			glVertex3f(TabPC[numPoint].x - 0.1, TabPC[numPoint].y - 0.1, TabPC[numPoint].z);
			glVertex3f(TabPC[numPoint].x - 0.1, TabPC[numPoint].y + 0.1, TabPC[numPoint].z);

			glVertex3f(TabPC2[1].x + 0.1, TabPC2[1].y + 0.1, TabPC2[1].z);
			glVertex3f(TabPC2[1].x + 0.1, TabPC2[1].y - 0.1, TabPC2[1].z);
			glVertex3f(TabPC2[1].x - 0.1, TabPC2[1].y - 0.1, TabPC2[1].z);
			glVertex3f(TabPC2[1].x - 0.1, TabPC2[1].y + 0.1, TabPC2[1].z);
		}
		else{
			glVertex3f(TabPC2[numPoint - 2].x + 0.1, TabPC2[numPoint - 2].y + 0.1, TabPC2[numPoint - 2].z);
			glVertex3f(TabPC2[numPoint - 2].x + 0.1, TabPC2[numPoint - 2].y - 0.1, TabPC2[numPoint - 2].z);
			glVertex3f(TabPC2[numPoint - 2].x - 0.1, TabPC2[numPoint - 2].y - 0.1, TabPC2[numPoint - 2].z);
			glVertex3f(TabPC2[numPoint - 2].x - 0.1, TabPC2[numPoint - 2].y + 0.1, TabPC2[numPoint - 2].z);
		}
	glEnd(); 

	// Dessiner ici la courbe de Bézier.
	// Vous devez avoir implémenté Bernstein précédemment.*
	DoubleBezier();

	glEnd(); 
	glFlush();
}

/* Au cas ou la fenetre est modifiee ou deplacee */
void reshape(int w, int h)
{
   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-20, 20, -20, 20, -10,10);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y)
{
   switch (key) {
   case '+':
	   if (numPoint < OrdreTotale - 1)
		   numPoint = numPoint + 1;
        else
            numPoint = 0;
		tx=0;
		ty=0;
		break;
   case '-':
		if (numPoint > 0) 
		   numPoint = numPoint - 1;
        else
			numPoint = OrdreTotale - 1;
		tx=0;
		ty=0;
		break;

   case 'd':
	   printf("direction");
         tx=0.1;
		 ty=0;
      break;
   case 'q':
         tx=-0.1;
		 ty=0;
      break;
   case 'z':
         ty=0.1;
		 tx=0;
      break;
   case 's':
         ty=-0.1;
		 tx=0;
      break;
   case ESC:
      exit(0);
      break;
   default :
	   tx=0;
	   ty=0;
   }
   glutPostRedisplay();
}

int main(int argc, char **argv)
{
   glutInitWindowSize(400, 400);
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
   glutCreateWindow("Courbe de Bézier");
   init();
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);
   glutDisplayFunc(display);
   glutMainLoop();
   return 0;
}
