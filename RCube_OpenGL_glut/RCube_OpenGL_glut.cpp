#include "MainRCube.h"
#include <time.h>
#include <stdlib.h>
#include <glut.h>
#include <chrono>
#include <windows.h>
#include <string>

#define CUBE_SIZE 30
#define TIMER 25
#define TIMER2 400
//                    16711680  16737792   65331    255       16777215  16776960
//                    красный  оранжевый  зеленый   синий     белый     желтый 
unsigned int c[6] = { 0xFF0000,0xFF6600 , 0x00FF33, 0x0000FF, 0xFFFFFF, 0xFFFF00 };


//        Стандарт
//R — правая сторона  :: R' -> 5
//L — левая  :: 4
//
//U — верхняя  :: U' -> 3
//D — нижняя  :: 2
//
//F — фронтальная  :: F' -> 1
//B — задняя  ::  0


GLfloat lightPos[] = { 0, 100, 200, 0 };

int xRot = 24, yRot = 34, zRot = 0;

double translateZ = -55.0;

RCube cube;

int Movment = 0;

int RCube_Sloving = 0;

void display()
{
	glPushMatrix();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1, 0, 0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glTranslatef(0, 0, translateZ);
	glRotatef(xRot, 1, 0, 0);
	glRotatef(yRot, 0, 1, 0);
	glTranslatef(CUBE_SIZE / -2.0, CUBE_SIZE / -2.0, CUBE_SIZE / -2.0);
	cube.draw();
	glPopMatrix();
	glutSwapBuffers();

	//glutPostRedisplay();
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	GLfloat fAspect = (GLfloat)w / (GLfloat)h;
	gluPerspective(60, fAspect, 1, 1000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void lightning()
{
	glClearColor(0.192, 0.192, 0.192, 0.0);

	srand(time(0));

	float mat_specular[] = { 0.3, 0.3, 0.3, 0 };
	float diffuseLight[] = { 0.2, 0.2, 0.2, 1 };
	float ambientLight[] = { 0.9, 0.9, 0.9, 1.0 };
	glShadeModel(GL_SMOOTH);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMateriali(GL_FRONT, GL_SHININESS, 128);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);

	cube.clear(CUBE_SIZE, c);
}


void timerCallback(int) {
	//glutTimerFunc(TIMER, timerCallback, 0);
	if (cube.RotNOW != -1) {
		cube.Rotate(cube.RotNOW, 6);
	}
	display();
	/*Sleep(3);*/
}

//        Стандарт
//R — правая сторона  :: R' -> 5
//L — левая  :: 4
//
//U — верхняя  :: U' -> 3
//D — нижняя  :: 2
//
//F — фронтальная  :: F' -> 1
//B — задняя  ::  0



//std::string swapp = "-";
//void ddd(std::string &swapp) {
//
//	std::switch (swapp) {
//	case "U'": {
//		cube.Rotate(3, 6);
//		timerCallback(0);
//		display();
//		break;
//	}
//	default:
//		break;
//	}
//}

bool WhiteCrossSloved(RCube& cube) {
	if (cube.a[0][1][0].miniCubeColor[4] == 16777215 and             //1 3 4   -- death ----> complete
		cube.a[0][1][2].miniCubeColor[4] == 16777215 and			 //5 1 2 5 -- death ----> complete 
		cube.a[0][0][1].miniCubeColor[4] == 16777215 and
		cube.a[0][2][1].miniCubeColor[4] == 16777215 ) {
		return true;
	}
	return false;
}
void rotationFull(int index, int angle) {
	std::cout <<"rotationFull: " <<  index << " " << angle << '\n';
	cube.Rotate(index, angle);
	while (cube.RotNOW != -1) {
		cube.Rotate(index, angle);
		display();
	}
}


void sloveWhiteCross(RCube& cube) {
	while (!WhiteCrossSloved(cube)) {
		//под белой стороной 

		if (cube.a[2][2][1].miniCubeColor[5] == 16777215) {  //бело-синее ребро за 2 движения
			std::cout << "1" << '\n';
			while (cube.a[0][2][1].miniCubeColor[4] == 16777215) {
				rotationFull(4, 6);
				display();
			}
			while (cube.a[0][2][1].miniCubeColor[4] != 16777215) {
				cube.Rotate(3, 6);
				display();
			}
			
		}
		if (cube.a[2][1][0].miniCubeColor[5] == 16777215) {  //бело-оранжевое ребро за 2 движения
			std::cout << "2" << '\n';
			while (cube.a[0][1][0].miniCubeColor[4] == 16777215) {
				rotationFull(4, 6);
				display();
			}
			while (cube.a[0][1][0].miniCubeColor[4] != 16777215) {
				cube.Rotate(0, 6);
				display();
			}
		}
		if (cube.a[2][0][1].miniCubeColor[5] == 16777215) { //бело-зеленое ребро за 2 движения
			std::cout << "3" << '\n';
			while (cube.a[0][0][1].miniCubeColor[4] == 16777215) {
				rotationFull(4, 6);
				display();
			}
			while (cube.a[0][0][1].miniCubeColor[4] != 16777215) {
				cube.Rotate(2, 6);
				display();
			}
		}
		if (cube.a[2][1][2].miniCubeColor[5] == 16777215) { //бело-красное ребро за 2 движения
			std::cout << "4" << '\n';
			while (cube.a[0][1][2].miniCubeColor[4] == 16777215) {
				rotationFull(4, 6);
				display();
			}
			while (cube.a[0][1][2].miniCubeColor[4] != 16777215) {
				cube.Rotate(1, 6);
				display();
			}
		}

		//   //   //   //   //  под белой стороной  //   //   //  //  //   //

		if (cube.a[2][2][1].miniCubeColor[3] == 16777215) {  //бело-синее ребро (развернуто)
			std::cout << "5" << '\n';
			while (cube.a[0][2][1].miniCubeColor[4] == 16777215) {
				rotationFull(4, 6);
				display();
			}
			rotationFull(5, -6);
			rotationFull(0, 6);
			rotationFull(3, 6);
			rotationFull(0, -6);
		}
		if (cube.a[2][1][0].miniCubeColor[1] == 16777215) {  //бело-оранжевое ребро (развернуто)
			std::cout << "6" << '\n';
			while (cube.a[0][1][0].miniCubeColor[4] == 16777215) {
				rotationFull(4, 6);
				display();
			}
			rotationFull(5, -6);
			rotationFull(2, 6);
			rotationFull(0, -6);
			rotationFull(2, -6);
		}
		if (cube.a[2][0][1].miniCubeColor[2] == 16777215) { //бело-зеленое ребро (развернуто)
			std::cout << "7" << '\n';
			while (cube.a[0][1][0].miniCubeColor[4] == 16777215) {
				rotationFull(4, 6);
				display();
			}
			rotationFull(5, -6);
			rotationFull(1, -6);
			rotationFull(2, -6);
			rotationFull(1, 6);
		}
		if (cube.a[2][1][2].miniCubeColor[0] == 16777215) { //бело-красное ребро (развернуто)
			std::cout << "8" << '\n';
			while (cube.a[0][1][2].miniCubeColor[4] == 16777215) {
				rotationFull(4, 6);
				display();
			}
			rotationFull(5, -6);
			rotationFull(3, -6);
			rotationFull(1, 6);
			rotationFull(3, -6);
		}


		//   //   //   если белая часть во втором слое   //  //   //

		if (cube.a[1][2][0].miniCubeColor[1] == 16777215) {  //синее-оранжевое ребро (оранжевое)
			std::cout << "9" << '\n';
			while (cube.a[0][2][1].miniCubeColor[4] == 16777215) {
				rotationFull(4, 6);
				display();
			}
			rotationFull(3, 6);
		}
		if (cube.a[1][2][0].miniCubeColor[3] == 16777215) {  //синее-оранжевое ребро (синий)
			std::cout << "10" << '\n';
			while (cube.a[0][1][0].miniCubeColor[4] == 16777215) {
				rotationFull(4, 6);
				display();
			}
			rotationFull(0, 6);
		}




		if (cube.a[1][0][0].miniCubeColor[1] == 16777215) {  //зелено-оранжевое ребро (оранжевое)        !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!  3   11
			std::cout << "11" << '\n';
			while (cube.a[0][0][1].miniCubeColor[4] == 16777215) {
				rotationFull(4, 6);
				display();
			}
			rotationFull(2, 6);

		}
		if (cube.a[1][0][0].miniCubeColor[2] == 16777215) { //зелено-оранжевое ребро (зелено)
			std::cout << "12" << '\n';
			while (cube.a[0][1][0].miniCubeColor[4] == 16777215) {
				rotationFull(4, 6);
				display();
			}
			rotationFull(0, -6);

		}




		if (cube.a[1][0][2].miniCubeColor[0] == 16777215) {  //зелено-красное ребро (красный)
			std::cout << "13" << '\n';
			while (cube.a[0][0][1].miniCubeColor[4] == 16777215) {
				rotationFull(4, 6);
				display();
			}
			rotationFull(2, -6);

		}
		if (cube.a[1][0][2].miniCubeColor[2] == 16777215) { //зелено-красное ребро (зеленый)
			std::cout << "14" << '\n';
			while (cube.a[0][1][2].miniCubeColor[4] == 16777215) {
				rotationFull(4, 6);
				display();
			}
			rotationFull(1, -6);

		}




		if (cube.a[1][2][2].miniCubeColor[3] == 16777215) {  //сине-красное ребро (синие)
			std::cout << "15" << '\n';
			while (cube.a[0][1][2].miniCubeColor[4] == 16777215) {
				rotationFull(4, 6);
				display();
			}
			rotationFull(1, 6);

		}
		if (cube.a[1][2][2].miniCubeColor[0] == 16777215) { //сине-красное ребро (красное)
			std::cout << "16" << '\n';
			while (cube.a[0][2][1].miniCubeColor[4] == 16777215) {
				rotationFull(4, 6);
				display();
			}
			rotationFull(3, -6);

		}

		//   //   //   перевертыш у креста   //  //   //

		if (cube.a[0][2][1].miniCubeColor[3] == 16777215) { //синий                             
			std::cout << "17" << '\n';
			rotationFull(3, 6);
			rotationFull(3, 6);
		}

		if (cube.a[0][1][2].miniCubeColor[0] == 16777215) {  //красный
			std::cout << "18" << '\n';
			rotationFull(1, 6);
			rotationFull(1, 6);
		}

		if (cube.a[0][0][1].miniCubeColor[2] == 16777215) {  //зеленый
			std::cout << "19" << '\n';
			rotationFull(2, 6);
			rotationFull(2, 6);
		}

		if (cube.a[0][1][0].miniCubeColor[1] == 16777215) {  //оранжевый
			std::cout << "20" << '\n';
			rotationFull(0, 6);
			rotationFull(0, 6);
		}
	}
}

bool WhiteCorrectCrossSloved(RCube& cube) {
	if (/*cube.a[0][1][0].miniCubeColor[4] == 16777215 and
		cube.a[0][1][2].miniCubeColor[4] == 16777215 and
		cube.a[0][0][1].miniCubeColor[4] == 16777215 and
		cube.a[0][2][1].miniCubeColor[4] == 16777215 and*/

		cube.a[0][1][0].miniCubeColor[1] == 16737792 and
		cube.a[0][1][2].miniCubeColor[0] == 16711680 and
		cube.a[0][0][1].miniCubeColor[2] == 65331    and
		cube.a[0][2][1].miniCubeColor[3] == 255) {
		return true;
	}
	return false;
}

void sloveCorrectWhiteCross(RCube& cube) {
	while (!WhiteCorrectCrossSloved(cube)) {
		bool flag = false;
		if (cube.a[0][0][1].miniCubeColor[2] == 65331) {
			std::cout << "2: 1" << '\n';
			if (cube.a[0][1][0].miniCubeColor[1] == 16737792) { //зеленый и оранжевый	корректен
				rotationFull(1, -6);
				rotationFull(4, 6);
				rotationFull(1, 6);
				rotationFull(4, -6);
				rotationFull(1, -6);
			}
			else if (cube.a[0][1][2].miniCubeColor[0] == 16711680) { //зеленый и красный  корректен
				rotationFull(3, -6);
				rotationFull(4, 6);
				rotationFull(3, 6);
				rotationFull(4, -6);
				rotationFull(3, -6);
			}
			else if (cube.a[0][2][1].miniCubeColor[3] == 255) { //зеленый и синий  корректен  (противоположные) 
				rotationFull(1, -6);
				rotationFull(4, 6);
				rotationFull(1, 6);
				rotationFull(4, -6);
				rotationFull(1, -6);

				//rotationFull(4, -6);
				//flag = true;
			}
		}

		else if (cube.a[0][1][2].miniCubeColor[0] == 16711680) {
			std::cout << "2: 2" << '\n';

			if (cube.a[0][0][1].miniCubeColor[2] == 65331) { //красный и зеленый	корректен
				rotationFull(3, -6);
				rotationFull(4, 6);
				rotationFull(3, 6);
				rotationFull(4, -6);
				rotationFull(3, -6);
			}
			else if (cube.a[0][2][1].miniCubeColor[2] == 255) { //красный и синий	корректен
				rotationFull(0, 6);
				rotationFull(4, 6);
				rotationFull(0, -6);
				rotationFull(4, -6);
				rotationFull(0, 0);
			}
			else if (cube.a[0][1][0].miniCubeColor[1] == 16737792) { //красный и оранжевый	корректен  (противоположные)  
				rotationFull(3, -6);
				rotationFull(4, 6);
				rotationFull(3, 6);
				rotationFull(4, -6);
				rotationFull(3, -6);

				//rotationFull(4, -6);
				//flag = true;
			}
		}

		else if (cube.a[0][2][1].miniCubeColor[2] == 255) {
			std::cout << "2: 3" << '\n';
			if (cube.a[0][0][1].miniCubeColor[2] == 16711680) { //синий красный   корректен
				rotationFull(0, 6);
				rotationFull(4, 6);
				rotationFull(0, -6);
				rotationFull(4, -6);
				rotationFull(0, 0);
			}
			else if (cube.a[0][1][0].miniCubeColor[1] == 16737792) { //снисй и оранжевый	корректен
				rotationFull(2, 6);
				rotationFull(4, 6);
				rotationFull(2, -6);
				rotationFull(4, -6);
				rotationFull(2, 6);
			}
			else if (cube.a[0][0][1].miniCubeColor[2] == 65331) { //синий и зеленый	корректен  (противоположные)  
				rotationFull(1, -6);
				rotationFull(4, 6);
				rotationFull(1, 6);
				rotationFull(4, -6);
				rotationFull(1, -6);

				//rotationFull(4, -6);

				//flag = true;
			}
		}

		else if (cube.a[0][1][0].miniCubeColor[1] == 16737792) {
			std::cout << "2: 4" << '\n';
			if (cube.a[0][2][1].miniCubeColor[2] == 255) { //оранжевый и синий  корректен
				rotationFull(2, 6);
				rotationFull(4, 6);
				rotationFull(2, -6);
				rotationFull(4, -6);
				rotationFull(2, 6);
			}
			else if (cube.a[0][0][1].miniCubeColor[2] == 65331) { //оранжевый и зеленый	корректен
				rotationFull(1, -6);
				rotationFull(4, 6);
				rotationFull(1, 6);
				rotationFull(4, -6);
				rotationFull(1, -6);
			}
			else if (cube.a[0][1][2].miniCubeColor[0] == 16711680) { //оранжевый и красный	корректен  (противоположные)  
				rotationFull(3, -6);
				rotationFull(4, 6);
				rotationFull(3, 6);
				rotationFull(4, -6);
				rotationFull(3, -6);

				//rotationFull(4, -6);

				//flag = true;
			}
		}
		if (!WhiteCorrectCrossSloved(cube)) {
			std::cout << " dddddddddddddddddddddddddddddddddddddddddddddd " << '\n';
			rotationFull(4, 6);
		}


	}


}
//2: 1
//ddddddddddd
//2 : 2
//ddddddddddd
//2 : 4
//ddddddddddd
//ddddddddddd



void keys(unsigned char key, int, int)
{
	if (cube.RotNOW == -1 && key >= '0' && key <= '5')
	{
		cube.Rotate(key - '0', 6);
		display();
	}

	if (cube.RotNOW == -1 && key == '6') {
		sloveWhiteCross(cube);
		//sloveCorrectWhiteCross(cube);
	}
	if (key == '7') {
		cube.clear(CUBE_SIZE, c);
		glutPostRedisplay();
	}
	if (key == '8') {
		Movment = 1 - Movment;
	}
	if (key == '9') {
		sloveCorrectWhiteCross(cube);
	}

	else if (key == 'w' or key == 'a' or key == 's' or key == 'd') {
		if (key == 's')
		{
			xRot += 9;
			if (xRot >= 360)
				xRot -= 360;
			glutPostRedisplay();
		}

		if (key == 'w')
		{
			xRot -= 9;
			if (xRot < 0)
				xRot += 360;
			glutPostRedisplay();
		}

		if (key == 'd')
		{
			yRot += 9;
			if (yRot >= 360)
				yRot -= 360;
			glutPostRedisplay();
		}

		if (key == 'a')
		{
			yRot -= 9;
			if (yRot < 0)
				yRot += 360;
			glutPostRedisplay();
		}
	}
}




void timer(int)
{
	glutTimerFunc(TIMER, timer, 0);
	if (Movment)
	{
		if (cube.RotNOW == -1) {
			int r = rand() % 6 + '0';
			keys(r, 0, 0);
			std::cout << "random: "<< r % 6  << '\n';
		}
		else {
			cube.Rotate(cube.RotNOW, 6);
		}
	}
	else
	{
		if (cube.RotNOW != -1) {
			cube.Rotate(cube.RotNOW, 6);
		}
	}
	display();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1100, 700);
	glutInitWindowPosition(1, 1);
	glutCreateWindow("RCube");
	lightning();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keys);
	/*glutTimerFunc(TIMER, timerCallback, 0);*/
	glutTimerFunc(TIMER, timer, 0);
	
	glutMainLoop();
	return 0;
}