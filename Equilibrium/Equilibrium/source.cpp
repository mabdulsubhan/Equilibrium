/*		
					Equilibrium
		
		Developed by : Muhammad Abdul Subhan
		Seat No		 : B12101062
		Course		 : Computer Graphics (BS-514)
		Submitted To : Maam Humera Bashir
*/

#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <cmath>
#include <string.h>
#include <sstream>
#include <time.h>
#include <windows.h>
#include <glut.h>
#include <SFML/Audio.hpp>

using namespace sf;

struct Obstacle {
	float x;
	float y;
	float w;
	float h;
	float vx;
	float vy;
	float mass;
};

struct Player {
	float x;
	float y;
	float w;
	float h;
	float vx1;
	float vx2;	
	float vy1;
	float vy2;
};


struct Target {
	float x;
	float y;
	float w;
	float h;
	float r;
	float vx;
	float vy;
	int time;
	int rv;
	int gv;
	int bv;
};
void timer(int = 0);
void display();
void pressKeySpecial(int key,int x,int y);
void releaseKeySpecial(int key,int x,int y);
void mouse(int, int, int, int);
void mouseMotion(int x, int y);
void keyboard(unsigned char key, int x, int y);
void resize(int width, int height);

void drawBox(void);
void gameScene(void);
void drawMenu(void);
void print_bitmap_string(void* font, char* s);

int WIN;
int timerParticle=0;
int Mx = 0, My = 0;

int level = 1;
int distanceTar = 25;
int timeScore = 0;
int pleft=0, pright=0, pup=0, pdown=0;

int gameStart = 0;
int menuSelection = 0;
float rvalue= 1,gvalue=1,bvalue = 1;
std::vector<Obstacle> objects;
std::vector<Player> player;
std::vector<Target> target;

Music music;

int main(int argc, char **argv)
{
		music.openFromFile("backmusic.wav");
		music.play();
	
	srand(time(0));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(50, 50);
	WIN = glutCreateWindow("Equilibrium");
	glClearColor(1,1,1, 1.0f);
	glMatrixMode(GL_PROJECTION);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
	glEnable(GL_BLEND);
	glEnable(GL_POINT_SMOOTH);
	glLoadIdentity();
	glOrtho(-250.0, 250.0, -250.0, 250.0, 0, 1); // l r b t

	Player o1;
	o1.x = 0;
	o1.y = 0;
	o1.vx1 = 0;
	o1.vx2 = 0;
	o1.vy1 = 0;
	o1.vy2 = 0;
	player.push_back(o1);

	Obstacle ob1;
	ob1.x = -150;
	ob1.y = 0;
	ob1.mass = 10000;
	objects.push_back(ob1);

	Obstacle ob2;
	ob2.x = 150;
	ob2.y = 0;
	ob2.mass = 10000;
	objects.push_back(ob2);

	Obstacle ob3;
	ob3.x = 0;
	ob3.y = -150;
	ob3.mass = 10000;
	objects.push_back(ob3);

	Obstacle ob4;
	ob4.x = 0;
	ob4.y = 150;
	ob4.mass = 10000;
	objects.push_back(ob4);

	Target t1;
	t1.x = -70;
	t1.y = 130;
	t1.time = 20;
	t1.r = 12;
	t1.rv = 255;
	t1.gv = 0;
	t1.bv = 0;
	target.push_back(t1);

	glutSpecialFunc(pressKeySpecial);
	glutSpecialUpFunc(releaseKeySpecial);
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMotion);
	glutKeyboardFunc(keyboard);
	glutDisplayFunc(display);
	glutReshapeFunc(resize);

	timer();

	glutMainLoop();
	return 0;
}

void resize(int width, int height) {
    glutReshapeWindow( 500, 500);
}

void timer(int)
{
	if(gameStart == 1) {
		if(level<7)
			timeScore ++;

		timerParticle++;

		Player &p = player[0];
		Target &t = target[0];

		float distTar =  sqrt((t.x - p.x)*(t.x - p.x) + (t.y - p.y)*(t.y - p.y));
		if(distTar < t.r && t.time>=0){
			t.time--;
		}

		Obstacle ob1 = objects[0];
		Obstacle ob2 = objects[1];
		Obstacle ob3 = objects[2];
		Obstacle ob4 = objects[3];

		float dl = sqrt((ob1.x - p.x)*(ob1.x - p.x) + (ob1.y - p.y)*(ob1.y - p.y));
		float dr = sqrt((ob2.x - p.x)*(ob2.x - p.x) + (ob2.y - p.y)*(ob2.y - p.y));
		float db = sqrt((ob3.x - p.x)*(ob3.x - p.x) + (ob3.y - p.y)*(ob3.y - p.y));
		float dt = sqrt((ob4.x - p.x)*(ob4.x - p.x) + (ob4.y - p.y)*(ob4.y - p.y));

		if(dl>10 && pleft == 1){
			p.vx1 += 0.03 * ob1.mass / (dl*dl) * (p.x - ob1.x)/dl;
			//p.x-=p.vx;
		}
		if(dr>10 && pright == 1){
			p.vx2 += 0.03 * ob2.mass / (dr*dr) * (p.x - ob2.x)/dr;
			//p.x+=p.vx;
		}
		if(db>10 && pdown == 1){
			p.vy1 += 0.03 * ob3.mass / (db*db) * (p.y - ob3.y)/db;
			//p.y-=p.vy;
		}
		if(dt>10 && pup == 1){
			p.vy2 += 0.03 * ob4.mass / (dt*dt) * (p.y - ob4.y)/dt;
			//p.y+=p.vy;
		}

		p.x += p.vx1;
		p.y += p.vy1;
		p.x += p.vx2;
		p.y += p.vy2;


		// Boundary Check
		if(player[0].x<-150) {
			player[0].x = -150;
			//player[0].vx =0;
		}
		if(player[0].y<-150) {
			player[0].y = -150;
			//player[0].vy *= -1;
		}
		if(player[0].x > 150) {
			player[0].x = 150;
			//player[0].vx  *= -1;
		}
		if(player[0].y>150) {
			player[0].y = 150;
			//player[0].vy  *= -1;
		}
		
		glutTimerFunc(50, timer, 0);
		glutPostRedisplay();
	}
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	if(gameStart == 1){
		gameScene();
	}
	else if(gameStart == 0){
		if(timeScore > 0){
			glColor3f(0.6,0.2,0.3);
			glBegin(GL_POLYGON);
			glVertex2f(-80,-250);
			glVertex2f(-80,-200);
			glVertex2f(80,-200);
			glVertex2f(80,-250);
			glEnd();

			glColor3f(1,1,1);
			char  name[] = "Your Time : ";
			glRasterPos2f(-70, -230);
			char  bufa[BUFSIZ];
			sprintf(bufa, "%s", name);
			print_bitmap_string(GLUT_BITMAP_9_BY_15,bufa);

			glColor3f(1,1,1);
			char  bufa3[BUFSIZ];
			glRasterPos2f(45, -232);
			sprintf(bufa3, "%03d", timeScore);
			print_bitmap_string(GLUT_BITMAP_9_BY_15,bufa3);
		}
		if(menuSelection == 0){
			if(rvalue < 255 && gvalue < 255 && bvalue < 255){
				int value = rand()%3;
				if(value == 0){
					rvalue+=0.2;
				}
				else if(value == 1){
					gvalue+=0.2;
				}
				else if(value == 2){
					bvalue+=0.2;
				}
			}
			glClearColor(rvalue/255.0f,gvalue/255.0f,bvalue/255.0f,1);
			drawMenu();
		}
		else if(menuSelection == 1){
			gameStart = 1;
			glClearColor(1,1,1,1);
			glClear(GL_COLOR_BUFFER_BIT);
			timer();

			timerParticle=0;

			level = 1;
			distanceTar = 25;
			timeScore = 0;

		}
		else if(menuSelection == 2){
			glutDestroyWindow(WIN);
			exit(0);
		}
	}


	glutSwapBuffers();
}

void drawMenu(){

	glColor3f(0,0,0);
	glBegin(GL_POLYGON);
	glVertex2f(-120,61);
	glVertex2f(-118,61);
	glVertex2f(-118,-1);
	glVertex2f(-120,-1);
	glEnd();

	glColor3f(0,0,0);
	glBegin(GL_POLYGON);
	glVertex2f(120,-61);
	glVertex2f(118,-61);
	glVertex2f(118,-1);
	glVertex2f(120,-1);
	glEnd();

	// Lowest
	glColor3f(0,0,0);
	glBegin(GL_POLYGON);
	glVertex2f(50,-59);
	glVertex2f(50,-61);
	glVertex2f(120,-61);
	glVertex2f(120,-59);
	glEnd();

	glColor3f(0,0,0);
	glBegin(GL_POLYGON);
	glVertex2f(-50,-40);
	glVertex2f(-50,-80);
	glVertex2f(50,-80);
	glVertex2f(50,-40);
	glEnd();

	// Middle
	glColor3f(0,0,0);
	glBegin(GL_POLYGON);
	glVertex2f(-120,1);
	glVertex2f(-120,-1);
	glVertex2f(120,-1);
	glVertex2f(120,1);
	glEnd();

	glColor3f(0,0,0);
	glBegin(GL_POLYGON);
	glVertex2f(-50,-20);
	glVertex2f(-50,20);
	glVertex2f(50,20);
	glVertex2f(50,-20);
	glEnd();

	// Top
	glColor3f(0,0,0);
	glBegin(GL_POLYGON);
	glVertex2f(-50,59);
	glVertex2f(-50,61);
	glVertex2f(-120,61);
	glVertex2f(-120,59);
	glEnd();

	glColor3f(0,0,0);
	glBegin(GL_POLYGON);
	glVertex2f(-50,40);
	glVertex2f(-50,80);
	glVertex2f(200,80);
	glVertex2f(200,40);
	glEnd();


	glColor3f(1,1,1);
	char  name[] = "E q u i l i b r i u m";
	glRasterPos2f(-40, 56);
	char  bufa[BUFSIZ];
	sprintf(bufa, "%s", name);
	print_bitmap_string(GLUT_BITMAP_9_BY_15,bufa);

	glColor3f(1,1,1);
	char  name2[] = "S t a r t";
	glRasterPos2f(-40, -2);
	char  bufa2[BUFSIZ];
	sprintf(bufa, "%s", name2);
	print_bitmap_string(GLUT_BITMAP_9_BY_15,bufa2);

	glColor3f(1,1,1);
	char  name3[] = "E x i t";
	glRasterPos2f(-35, -63);
	char  bufa3[BUFSIZ];
	sprintf(bufa, "%s", name3);
	print_bitmap_string(GLUT_BITMAP_9_BY_15,bufa3);

	glutPostRedisplay();
}

void gameScene(){
	// Drawing Text	
	glColor3f(0,0,0);
	glBegin(GL_POLYGON);
	glVertex2f(-250,145);
	glVertex2f(-250,90);
	glVertex2f(-200,90);
	glVertex2f(-200,145);
	glEnd();

	glColor3f(1,1,1);

	char  name[] = "Your";
	glRasterPos2f(-240, 130);
	char  bufa[BUFSIZ];
	sprintf(bufa, "%s", name);
	print_bitmap_string(GLUT_BITMAP_8_BY_13,bufa);

	char  name2[] = "Time";
	glRasterPos2f(-245, 115);
	char  bufa2[BUFSIZ];
	sprintf(bufa2, "%s", name2);
	print_bitmap_string(GLUT_BITMAP_8_BY_13,bufa2);

	char  bufa3[BUFSIZ];
	glRasterPos2f(-240, 100);
	sprintf(bufa3, "%03d", timeScore);
	print_bitmap_string(GLUT_BITMAP_9_BY_15,bufa3);

	drawBox();

	// Drawing Player
	Player &p = player[0];
	glColor3f(0,0,0);
	glPointSize(5.0f);
	glBegin(GL_POINTS);
	glVertex2f(p.x,p.y);
	glEnd();  

	// Drawing Target
	Target &t = target[0];
	glColor3f(t.rv/255.0f,t.gv/255.0f,t.bv/255.0f);
	glBegin(GL_POLYGON);
	for(float a = 0; a < 2*3.1415; a+=0.2)
		glVertex2f(t.r*cos(a) + t.x, t.r*sin(a) + t.y);
	glEnd();        

	if(t.time == 0 && level <7){
		if(level == 1){
			glClearColor(249/255.0f,255/255.0f,77/255.0f,1);
			target[0].r -= 2;
			t.rv = 1;
			t.gv = 255;
			t.bv = 77;
		}
		if(level == 2){
			glClearColor(202/255.0f,177/255.0f,137/255.0f,1);
			target[0].r -= 2;
			t.rv = 91;
			t.gv = 48;
			t.bv = 39;
		}
		if(level == 3){
			glClearColor(53/255.0f,167/255.0f,255/255.0f,1);
			target[0].r -= 2;
			t.rv = 255;
			t.gv = 231;
			t.bv = 76;

		}
		if(level == 4){
			glClearColor(88/255.0f,129/255.0f,87/255.0f,1);
			target[0].r -= 2;
			t.rv = 190;
			t.gv = 213;
			t.bv = 88;
		}
		if(level == 5){
			glClearColor(200/255.0f,40/255.0f,40/255.0f,1);
			target[0].r -= 2;
			t.rv = 255;
			t.gv = 255;
			t.bv = 255;
		}
		t.time = 20;
		level++;
		t.x = rand()%260-130;
		t.y = rand()%260-130;
	}
	if(level>5){
		menuSelection = 0;
		gameStart = 0;
		rvalue= 1,gvalue=1,bvalue = 1;
	}
}
void drawBox(){
	
	// left
	if(pright== 1)
		glColor3f(0.3,0.5,0.8);
	else
		glColor3f(0,0,0);

	glBegin(GL_POLYGON);
	glVertex2f(-200,-200);
	glVertex2f(-150,-150);
	glVertex2f( -150,150);
	glVertex2f(-200,200);
	glEnd();

	// right
	if(pleft== 1)
		glColor3f(0.3,0.5,0.8);
	else
		glColor3f(0,0,0);

	glBegin(GL_POLYGON);
	glVertex2f(200,200);
	glVertex2f(150,150);
	glVertex2f( 150,-150);
	glVertex2f(200,-200);
	glEnd();

	// up
	if(pdown == 1)
		glColor3f(0.3,0.5,0.8);
	else
		glColor3f(0,0,0);

	glBegin(GL_POLYGON);
	glVertex2f(-200,200);
	glVertex2f(200,200);
	glVertex2f( 150,150);
	glVertex2f(-150, 150);
	glEnd();

	// down
	if(pup== 1)
		glColor3f(0.3,0.5,0.8);
	else
		glColor3f(0,0,0);

	glBegin(GL_POLYGON);
	glVertex2f(-200,-200);
	glVertex2f(200,-200);
	glVertex2f( 150,-150);
	glVertex2f(-150, -150);
	glEnd();
}


void pressKeySpecial(int key,int x,int y)
{
	switch(key)
	{
	case GLUT_KEY_RIGHT :
		objects[0].mass+=1000;
		pleft = 1;
		break;
	case GLUT_KEY_LEFT :
		pright = 1;
		objects[1].mass+=1000;
		break;
	case GLUT_KEY_DOWN :
		pup = 1;
		objects[3].mass+=1000;
		break;
	case GLUT_KEY_UP :
		pdown = 1;
		objects[2].mass+=1000;
		break;
	}

	glutPostRedisplay();
}

void releaseKeySpecial(int key,int x,int y){
	switch(key)
	{
	case GLUT_KEY_RIGHT :
		pleft = 0;
		break;
	case GLUT_KEY_LEFT :
		pright = 0;
		break;
	case GLUT_KEY_DOWN :
		pup = 0;
		break;
	case GLUT_KEY_UP :
		pdown = 0;
		break;
	}

	glutPostRedisplay();
}


void print_bitmap_string(void* font, char* s)

{

	if (s && strlen(s)) {

		while (*s) {

			glutBitmapCharacter(font, *s);

			s++;

		}

	}

}


void mouse(int button, int state, int x, int y)
{
	//set the coordinates
	Mx = x - 250;
	My =(y - 250)*-1;
	
	if(button == GLUT_LEFT_BUTTON){
		if(Mx > -50 && Mx < 200 && My > -20 && My < 20){
			menuSelection = 1;
		}

		else if(Mx > -50 && Mx < 200 && My > -80 && My < -40){
			menuSelection = 2;
		}
	}
}

void mouseMotion(int x, int y)
{
	Mx = x - 250;
	My = (y - 250)*-1;  
}

void keyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
	case 27:
		menuSelection = 0;
		gameStart = 0;
		rvalue= 1,gvalue=1,bvalue = 1;
		gameStart = 0;
		glClearColor(1,1,1,1);
		glClear(GL_COLOR_BUFFER_BIT);
		timerParticle=0;
		level = 1;
		distanceTar = 25;
		timeScore = 0;
		glutPostRedisplay();
		break;
	}
}

/*		
					Equilibrium
		
		Developed by : Muhammad Abdul Subhan
		Seat No		 : B12101062
		Course		 : Computer Graphics (BS-514)
		Submitted To : Maam Humera Bashir
*/