#include "MainRCube.h"
#include <time.h>
#include <stdlib.h>
#include <glut.h>
#include <chrono>
#include <windows.h>
#include <string>
#include <vector>
#include <algorithm>

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

void rotationFull(int index, int angle) {
	//std::cout <<"rotationFull: " <<  index << " " << angle << '\n';
	cube.Rotate(index, angle);
	while (cube.RotNOW != -1) {
		cube.Rotate(index, angle);
		display();
	}
}


bool WhiteCrossSloved(RCube& cube) {
	if (cube.a[0][1][0].miniCubeColor[4] == 16777215 and             //1 3 4   -- death ----> complete
		cube.a[0][1][2].miniCubeColor[4] == 16777215 and			 //5 1 2 5 -- death ----> complete 
		cube.a[0][0][1].miniCubeColor[4] == 16777215 and
		cube.a[0][2][1].miniCubeColor[4] == 16777215) {
		return true;
	}
	return false;
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
	if (cube.a[0][1][0].miniCubeColor[1] == 16737792 and
		cube.a[0][1][2].miniCubeColor[0] == 16711680 and
		cube.a[0][0][1].miniCubeColor[2] == 65331    and
		cube.a[0][2][1].miniCubeColor[3] == 255) {
		return true;
	}
	return false;
}

//2 : 1
//< -------------- >
//2 : 2
//< -------------- >
//2 : 4
//< -------------- >
//< -------------- >

void sloveCorrectWhiteCross(RCube& cube) {
	while (!WhiteCorrectCrossSloved(cube)) {
		bool flag = false;
		if (cube.a[0][0][1].miniCubeColor[2] == 65331) {  //зеленый
			std::cout << "2 : 1" << '\n';
			if (cube.a[0][1][0].miniCubeColor[1] == 16737792) { //зеленый и оранжевый	корректен
				std::cout << "	2: 1.1" << '\n';
				rotationFull(1, -6);
				rotationFull(4, 6);
				rotationFull(1, 6);
				rotationFull(4, -6);
				rotationFull(1, -6);
			}
			else if (cube.a[0][1][2].miniCubeColor[0] == 16711680) { //зеленый и красный  корректен
				std::cout << "	2: 1.2" << '\n';
				rotationFull(3, -6);
				rotationFull(4, 6);
				rotationFull(3, 6);
				rotationFull(4, -6);
				rotationFull(3, -6);
			}
			else if (cube.a[0][2][1].miniCubeColor[3] == 255) { //зеленый и синий  корректен  (противоположные) 
				std::cout << "	2: 1.3" << '\n';
				rotationFull(1, -6);
				rotationFull(4, 6);
				rotationFull(1, 6);
				rotationFull(4, -6);
				rotationFull(1, -6);

				rotationFull(4, -6);
				flag = true;
			}
		}

		else if (cube.a[0][1][2].miniCubeColor[0] == 16711680 ) {  //красный
			std::cout << "2: 2" << '\n';

			if (cube.a[0][0][1].miniCubeColor[2] == 65331) { //красный и зеленый	корректен
				std::cout << "	2: 2.1" << '\n';
				rotationFull(3, -6);
				rotationFull(4, 6);
				rotationFull(3, 6);
				rotationFull(4, -6);
				rotationFull(3, -6);
			}
			else if (cube.a[0][2][1].miniCubeColor[3] == 255) { //красный и синий	корректен
				std::cout << "	2: 2.2" << '\n';
				rotationFull(0, 6);
				rotationFull(4, 6);
				rotationFull(0, -6);
				rotationFull(4, -6);
				rotationFull(0, 0);
			}
			else if (cube.a[0][1][0].miniCubeColor[1] == 16737792) { //красный и оранжевый	корректен  (противоположные)  
				std::cout << "	2: 2.3" << '\n';
				rotationFull(3, -6);
				rotationFull(4, 6);
				rotationFull(3, 6);
				rotationFull(4, -6);
				rotationFull(3, -6);

				rotationFull(4, -6);
				flag = true;
			}
		}

		else if (cube.a[0][2][1].miniCubeColor[3] == 255) {  //синий
			std::cout << "2: 3" << '\n';
			if (cube.a[0][1][2].miniCubeColor[0] == 16711680) { //синий красный   корректен
				std::cout << "	2: 3.1" << '\n';
				rotationFull(0, 6);
				rotationFull(4, 6);
				rotationFull(0, -6);
				rotationFull(4, -6);
				rotationFull(0, 0);
			}
			else if (cube.a[0][1][0].miniCubeColor[1] == 16737792) { //синий и оранжевый	корректен
				std::cout << "	2: 3.2" << '\n';
				rotationFull(2, 6);
				rotationFull(4, 6);
				rotationFull(2, -6);
				rotationFull(4, -6);
				rotationFull(2, 6);
			}
			else if (cube.a[0][0][1].miniCubeColor[2] == 65331) { //синий и зеленый	корректен  (противоположные)  
				std::cout << "	2: 3.3" << '\n';
				rotationFull(1, -6);
				rotationFull(4, 6);
				rotationFull(1, 6);
				rotationFull(4, -6);
				rotationFull(1, -6);

				rotationFull(4, -6);
				flag = true;
			}
		}

		else if (cube.a[0][1][0].miniCubeColor[1] == 16737792) {  //оранжевый
			std::cout << "2: 4" << '\n';
			if (cube.a[0][2][1].miniCubeColor[3] == 255) { //оранжевый и синий  корректен
				std::cout << "	2: 4.1" << '\n';
				rotationFull(2, 6);
				rotationFull(4, 6);
				rotationFull(2, -6);
				rotationFull(4, -6);
				rotationFull(2, 6);
			}
			else if (cube.a[0][0][1].miniCubeColor[2] == 65331) { //оранжевый и зеленый	корректен
				std::cout << "	2: 4.2" << '\n';
				rotationFull(1, -6);
				rotationFull(4, 6);
				rotationFull(1, 6);
				rotationFull(4, -6);
				rotationFull(1, -6);
			}
			else if (cube.a[0][1][2].miniCubeColor[0] == 16711680) { //оранжевый и красный	корректен  (противоположные)  
				std::cout << "	2: 4.3" << '\n';
				rotationFull(3, -6);
				rotationFull(4, 6);
				rotationFull(3, 6);
				rotationFull(4, -6);
				rotationFull(3, -6);

				rotationFull(4, -6);
				flag = true;
			}
		}
		if (!WhiteCorrectCrossSloved(cube)) {
			std::cout << " <--------------> " << '\n';
			rotationFull(4, 6);
		}
	}
}

bool FirstLayerSloved(RCube& cube) {
	//  16711680  16737792   65331    255       16777215  16776960
	//  красный  оранжевый  зеленый   синий     белый     желтый 
	if (WhiteCrossSloved(cube) and 
		WhiteCorrectCrossSloved(cube) and
		cube.a[0][0][0].miniCubeColor[1] == 16737792 and cube.a[0][0][0].miniCubeColor[2] == 65331 and cube.a[0][0][0].miniCubeColor[4] == 16777215 and
		cube.a[0][0][2].miniCubeColor[0] == 16711680 and cube.a[0][0][2].miniCubeColor[2] == 65331 and cube.a[0][0][2].miniCubeColor[4] == 16777215 and
		cube.a[0][2][0].miniCubeColor[1] == 16737792 and cube.a[0][2][0].miniCubeColor[3] == 255   and cube.a[0][2][0].miniCubeColor[4] == 16777215 and
		cube.a[0][2][2].miniCubeColor[0] == 16711680 and cube.a[0][2][2].miniCubeColor[3] == 255   and cube.a[0][2][2].miniCubeColor[4] == 16777215) {
		return true;
	}
	return false;
}

// синий  зеленый  красный      оранжевый    желтый      белый
// 255    65331    16711680     16737792     16776960     16777215
std::vector <int> colorsTempVec;

void SloveFirstLayer(RCube& cube) {
	while (!FirstLayerSloved(cube)) {

		
		bool flag;
		//проверка верхнего слоя (где желтый центр) на наличее угла с белым цветом
		while(true){
			colorsTempVec.clear();
			flag = false;
			for (int i = 0; i < 6; i++) {  // оранжево-зелено-желтый   (проверка есть ли белый цвет)
				if (cube.a[2][0][0].miniCubeColor[i] == 16777215) {
					flag = true;
				}
				else if (cube.a[2][0][0].miniCubeColor[i] != 0) {
					colorsTempVec.push_back(cube.a[2][0][0].miniCubeColor[i]);
				}
			}
			if (flag) {
				std::cout << "3: 1" << '\n';
				std::sort(colorsTempVec.begin(), colorsTempVec.end());
				if (colorsTempVec[0] == 65331 and colorsTempVec[1] == 16737792) { // бело-зелено-оранжевый на своем месте (пиф пафами опускаем)
					std::cout << "	3: 1.1" << '\n';
					int count = 0;
					while (true) {
						if (cube.a[0][0][0].miniCubeColor[4] == 16777215 and count % 2 == 1) break;

						rotationFull(0, 6);
						rotationFull(5, -6);
						rotationFull(0, -6);
						rotationFull(5, 6);
						count++;
					}
				}
				else if (colorsTempVec[0] == 65331 and colorsTempVec[1] == 16711680) { // бело-зелено-красный     (U)    на своем месте (пиф пафами опускаем)
					std::cout << "	3: 1.2" << '\n';
					rotationFull(5, -6);
					int count = 0;
					while (true) {
						if (cube.a[0][0][2].miniCubeColor[4] == 16777215 and count % 2 == 1) break;
						rotationFull(2, 6);
						rotationFull(5, -6);
						rotationFull(2, -6);
						rotationFull(5, 6);
						count++;
					}
				}
				else if (colorsTempVec[0] == 255 and colorsTempVec[1] == 16711680) { // бело-сине-красный     (U)(U)     на своем месте (пиф пафами опускаем)
					std::cout << "	3: 1.3" << '\n';
					rotationFull(5, -6);
					rotationFull(5, -6);
					int count = 0;
					while (true) {
						if (cube.a[0][2][2].miniCubeColor[4] == 16777215 and count % 2 == 1) break;
						rotationFull(1, -6);
						rotationFull(5, -6);
						rotationFull(1, 6);
						rotationFull(5, 6);
						count++;
					}
				}
				else if (colorsTempVec[0] == 255 and colorsTempVec[1] == 16737792) { // бело-сине-оранжевый     (U')     на своем месте (пиф пафами опускаем)
					std::cout << "	3: 1.4" << '\n';
					rotationFull(5, 6);
					int count = 0;
					while (true) {
						if (cube.a[0][2][0].miniCubeColor[4] == 16777215 and count % 2 == 1) break;
						rotationFull(3, -6);
						rotationFull(5, -6);
						rotationFull(3, 6);
						rotationFull(5, 6);
						count++;
					}
				}
			}
			else {
				std::cout << "3: 2" << '\n';
				bool Wflag1 = false, Wflag2 = false, Wflag3 = false, Wflag4 = false;
				for (int i = 0; i < 6; i++) {  
					if (cube.a[2][0][0].miniCubeColor[i] == 16777215) {
						Wflag1 = true;
						break;
					}
				}
				for (int i = 0; i < 6; i++) {  
					if (cube.a[2][0][2].miniCubeColor[i] == 16777215) {
						Wflag2 = true;
						break;
					}
				}
				for (int i = 0; i < 6; i++) {  
					if (cube.a[2][2][0].miniCubeColor[i] == 16777215) {
						Wflag3 = true;
						break;
					}
				}
				for (int i = 0; i < 6; i++) {  
					if (cube.a[2][2][2].miniCubeColor[i] == 16777215) {
						Wflag4 = true;
						break;
					}
				}
				if (Wflag1 or Wflag2 or Wflag3 or Wflag4) {
					std::cout << "	3: 2.1" << '\n';
					rotationFull(5, 6);
				}
				else {
					std::cout << "	3: 2.2" << '\n';
					break;
				}
				
			}
		}


		flag = false;
		bool flag2 = false;


		//проверяем, есть ли в нижнем слое белый цвет, который надор вытащить наверх        !!!!!!!!!! если стоит правильный не трогаем !!!!!!!!!!!


		for (int i = 0; i < 6; i++) {
			if (cube.a[0][0][0].miniCubeColor[i] == 16777215) { 
				if (cube.a[0][0][0].miniCubeColor[1] == 16737792 and cube.a[0][0][0].miniCubeColor[2] == 65331 and cube.a[0][0][0].miniCubeColor[4] == 16777215) {  //оранжево-зелено-белый угол
					break;
				}
				else {
					flag = true;
					break;
				}
			}
		}
		if (flag) {
			std::cout << "	 orange green 0 white 0    (0 0 0)" << '\n';
			while (true) {
				for (int i = 0; i < 6; i++) {  // оранжево-зелено-желтый   (проверка есть ли белый цвет)
					if (cube.a[2][0][0].miniCubeColor[i] == 16777215) {
						flag2 = true;
						break;
					}
				}
				if (flag2) {
					rotationFull(5, 6);  //если на ним есть белый, его надо сдивнуть, иначе он будет внизу
				}
				else {
					break;
				}
			}
			rotationFull(0, 6);
			rotationFull(5, -6);
			rotationFull(0, -6);
			rotationFull(5, 6);

		}



		flag = false;
		flag2 = false;
		for (int i = 0; i < 6; i++) {
			if (cube.a[0][0][2].miniCubeColor[i] == 16777215) {   //красно-зелено-белый угол
				if (cube.a[0][0][2].miniCubeColor[0] == 16711680 and cube.a[0][0][2].miniCubeColor[2] == 65331 and cube.a[0][0][2].miniCubeColor[4] == 16777215) {
					break;
				}
				else {
					flag = true;
					break;
				}
			}

		}
		if (flag) {
			std::cout << "	red 0 green 0 white 0    (0 0 2)" << '\n';
			while (true) {
				for (int i = 0; i < 6; i++) {  // красно-зелено-желтый   (проверка есть ли белый цвет)
					if (cube.a[2][0][2].miniCubeColor[i] == 16777215) {
						flag2 = true;
						break;
					}
				}
				if (flag2) {
					rotationFull(5, 6);  //если на ним есть белый, его надо сдивнуть, иначе он будет внизу
				}
				else {
					break;
				}
			}
			rotationFull(2, 6);
			rotationFull(5, -6);
			rotationFull(2, -6);
			rotationFull(5, 6);

		}



		flag = false;
		flag2 = false;
		for (int i = 0; i < 6; i++) {
			if (cube.a[0][2][2].miniCubeColor[i] == 16777215) {   //красно-сине-белый угол
				if (cube.a[0][2][2].miniCubeColor[0] == 16711680 and cube.a[0][2][2].miniCubeColor[3] == 255 and cube.a[0][2][2].miniCubeColor[4] == 16777215) {
					break;
				}
				else {
					flag = true;
					break;
				}
			}

		}
		if (flag) {
			std::cout << "	red 0 0 blue white 0    (0 2 2)" << '\n';
			while (true) {
				for (int i = 0; i < 6; i++) {  // красно-сине-желтый   (проверка есть ли белый цвет)
					if (cube.a[2][2][2].miniCubeColor[i] == 16777215) {
						flag2 = true;
						break;
					}
				}
				if (flag2) {
					rotationFull(5, 6);  //если на ним есть белый, его надо сдивнуть, иначе он будет внизу
				}
				else {
					break;
				}
			}
			rotationFull(1, -6);
			rotationFull(5, -6);
			rotationFull(1, 6);
			rotationFull(5, 6);

		}


		flag = false;
		flag2 = false;
		for (int i = 0; i < 6; i++) {
			if (cube.a[0][2][0].miniCubeColor[i] == 16777215) {   //оранжево-сине-белый угол
				if (cube.a[0][2][0].miniCubeColor[1] == 16737792 and cube.a[0][2][0].miniCubeColor[3] == 255 and cube.a[0][2][0].miniCubeColor[4] == 16777215) {
					break;
				}
				else {
					flag = true;
					break;
				}
			}

		}
		if (flag) {
			std::cout << "0 orange 0 blue white 0    (0 2 0)" << '\n';
			while (true) {
				for (int i = 0; i < 6; i++) {  // оранжево-сине-желтый   (проверка есть ли белый цвет)
					if (cube.a[2][2][0].miniCubeColor[i] == 16777215) {
						flag2 = true;
						break;
					}
				}
				if (flag2) {
					rotationFull(5, 6);  //если на ним есть белый, его надо сдивнуть, иначе он будет внизу
				}
				else {
					break;
				}
			}
			rotationFull(3, -6);
			rotationFull(5, -6);
			rotationFull(3, 6);
			rotationFull(5, 6);

		}
	}
}





bool SecondLayerSloved(RCube& cube) {
	//  16711680  16737792   65331    255       16777215  16776960
	//  красный  оранжевый  зеленый   синий     белый     желтый 
	if (WhiteCrossSloved(cube) and
		WhiteCorrectCrossSloved(cube) and
		FirstLayerSloved(cube) and
		cube.a[1][0][0].miniCubeColor[1] == 16737792 and cube.a[1][0][0].miniCubeColor[2] == 65331 and 
		cube.a[1][0][2].miniCubeColor[0] == 16711680 and cube.a[1][0][2].miniCubeColor[2] == 65331 and
		cube.a[1][2][0].miniCubeColor[1] == 16737792 and cube.a[1][2][0].miniCubeColor[3] == 255 and
		cube.a[1][2][2].miniCubeColor[0] == 16711680 and cube.a[1][2][2].miniCubeColor[3] == 255) {
		return true;
	}
	return false;
}

// синий  зеленый  красный      оранжевый    желтый      белый
// 255    65331    16711680     16737792     16776960     16777215
std::vector <int> colorsTempVecCopy;
void SloveSecondLayer(RCube& cube) {
	bool flag;

	while (!SecondLayerSloved(cube)) {

		while (true) {
			colorsTempVec.clear();
			colorsTempVecCopy.clear();
			flag = false;
			for (int i = 0; i < 6; i++) {
				if (cube.a[2][1][0].miniCubeColor[i] == 16776960) {   //смотрим желто-оранжевое ребро и проверяем наличе там желтого цвета
					flag = true;
				}
				else if (cube.a[2][1][0].miniCubeColor[i] != 0) {
					colorsTempVec.push_back(cube.a[2][1][0].miniCubeColor[i]);
					colorsTempVecCopy.push_back(cube.a[2][1][0].miniCubeColor[i]);
				}
			}
			if (!flag) {
				std::cout << "4" << '\n';
				std::sort(colorsTempVecCopy.begin(), colorsTempVecCopy.end());
				if (colorsTempVecCopy[0] == 65331 and colorsTempVecCopy[1] == 16737792) {  // оранжево-зеленое ребро 2 варианта
					std::cout << "4.1" << '\n';

					if (cube.a[2][1][0].miniCubeColor[1] == 16737792) {  //cube.a[2][1][0].miniCubeColor[1]  ---> та сторона, которая смотрит на нас
						rotationFull(5, 6);
						rotationFull(2, -6);
						rotationFull(5, -6);
						rotationFull(2, 6);
						rotationFull(5, -6);
						rotationFull(0, 6);
						rotationFull(5, 6);
						rotationFull(0, -6);
					}
					else if (cube.a[2][1][0].miniCubeColor[1] == 65331) {
						rotationFull(5, -6);  // повернули к нужному месту 

						rotationFull(5, -6);
						rotationFull(0, 6);
						rotationFull(5, 6);
						rotationFull(0, -6);
						rotationFull(5, 6);
						rotationFull(2, -6);
						rotationFull(5, -6);
						rotationFull(2, 6);


					}
				}
				else if (colorsTempVecCopy[0] == 255 and colorsTempVecCopy[1] == 16711680) {  //сине-красное 2 варианта
					std::cout << "4.2" << '\n';

					if (cube.a[2][1][0].miniCubeColor[1] == 255) {
						rotationFull(5, 6);

						rotationFull(5, -6);
						rotationFull(1, -6);
						rotationFull(5, 6);
						rotationFull(1, 6);
						rotationFull(5, 6);
						rotationFull(3, 6);
						rotationFull(5, -6);
						rotationFull(3, -6);
					}
					else if (cube.a[2][1][0].miniCubeColor[1] == 16711680) {
						rotationFull(5, 6);
						rotationFull(5, 6);

						rotationFull(5, 6);
						rotationFull(3, 6);
						rotationFull(5, -6);
						rotationFull(3, -6);
						rotationFull(5, -6);
						rotationFull(1, -6);
						rotationFull(5, 6);
						rotationFull(1, 6);
					}
				}
				else if (colorsTempVecCopy[0] == 255 and colorsTempVecCopy[1] == 16737792) {  //оранжево-синее   2 варианта
					std::cout << "4.3" << '\n';

					if (cube.a[2][1][0].miniCubeColor[1] == 255) {
						rotationFull(5, 6);

						rotationFull(5, 6);
						rotationFull(0, -6);
						rotationFull(5, -6);
						rotationFull(0, 6);
						rotationFull(5, -6);
						rotationFull(3, -6);
						rotationFull(5, 6);
						rotationFull(3, 6);
					}
					else if (cube.a[2][1][0].miniCubeColor[1] == 16737792) {
						rotationFull(5, -6);
						rotationFull(3, -6);
						rotationFull(5, 6);
						rotationFull(3, 6);
						rotationFull(5, 6);
						rotationFull(0, -6);
						rotationFull(5, -6);
						rotationFull(0, 6);
					}
				}
				else if (colorsTempVecCopy[0] == 65331 and colorsTempVecCopy[1] == 16711680) {   //красно-зеленое   2 варианта  65331    16711680 
					std::cout << "4.4" << '\n';

					if (cube.a[2][1][0].miniCubeColor[1] == 65331) {

						std::cout << "4.4 --> green" << '\n';

						rotationFull(5, -6);

						rotationFull(5, 6);
						rotationFull(1, 6);
						rotationFull(5, -6);
						rotationFull(1, -6);
						rotationFull(5, -6);
						rotationFull(2, 6);
						rotationFull(5, 6);
						rotationFull(2, -6);
					}
					else if (cube.a[2][1][0].miniCubeColor[1] == 16711680) {
						std::cout << "4.4 --> red" << '\n';

						rotationFull(5, 6);
						rotationFull(5, 6);

						rotationFull(5, -6);
						rotationFull(2, 6);
						rotationFull(5, 6);
						rotationFull(2, -6);
						rotationFull(5, 6);
						rotationFull(1, 6);
						rotationFull(5, -6);
						rotationFull(1, -6);
					}
				}
			}
			else {
				bool Yflag1 = false, Yflag2 = false, Yflag3 = false, Yflag4 = false;
				bool Ansflag1 = false, Ansflag2 = false, Ansflag3 = false, Ansflag4 = false;  // проверяем, остались ли ребра без желтого цвета, если да, то делаем поворот, иначе проверяем уже установленные

				for (int i = 0; i < 6; i++) {
					if (cube.a[2][0][1].miniCubeColor[i] == 16776960) {
						Yflag1 = true;
						break;
					}
				}
				if (!Yflag1) Ansflag1 = true;

				for (int i = 0; i < 6; i++) {
					if (cube.a[2][1][0].miniCubeColor[i] == 16776960) {
						Yflag2 = true;
						break;
					}
				}
				if (!Yflag2) Ansflag2 = true;

				for (int i = 0; i < 6; i++) {
					if (cube.a[2][1][2].miniCubeColor[i] == 16776960) {
						Yflag3 = true;
						break;
					}
				}
				if (!Yflag3) Ansflag3 = true;

				for (int i = 0; i < 6; i++) {
					if (cube.a[2][2][1].miniCubeColor[i] == 16776960) {
						Yflag4 = true;
						break;
					}
				}
				if (!Yflag4) Ansflag4 = true;

				if (Ansflag1 or Ansflag2 or Ansflag3 or Ansflag4) {
					std::cout << "not only yellow" << '\n';
					rotationFull(5, 6);
				}
				else {
					std::cout << "4.break" << '\n';
					break;
				}

			}
		}

		//cube.a[1][0][0].miniCubeColor[1] == 16737792 and cube.a[1][0][0].miniCubeColor[2] == 65331 and
		//cube.a[1][0][2].miniCubeColor[0] == 16711680 and cube.a[1][0][2].miniCubeColor[2] == 65331 and
		//cube.a[1][2][0].miniCubeColor[1] == 16737792 and cube.a[1][2][0].miniCubeColor[3] == 255 and
		//cube.a[1][2][2].miniCubeColor[0] == 16711680 and cube.a[1][2][2].miniCubeColor[3] == 255
		
		bool rotateFlag = false;
		if (cube.a[1][0][0].miniCubeColor[1] != 16737792 and cube.a[1][0][0].miniCubeColor[2] != 65331) {  //оранжево-зеленое ребро
			std::cout << "4.1.1" << '\n';
			flag = false;
			for (int i = 0; i < 6; i++) {
				if (cube.a[1][0][0].miniCubeColor[i] == 16776960) {   //проверяем наличе там желтого цвета
					flag = true;
				}
			}
			if (!flag) {
				rotateFlag = true;
				rotationFull(5, -6);
				rotationFull(0, 6);
				rotationFull(5, 6);
				rotationFull(0, -6);
				rotationFull(5, 6);
				rotationFull(2, -6);
				rotationFull(5, -6);
				rotationFull(2, 6);
			}
		}


		if (!rotateFlag and cube.a[1][0][2].miniCubeColor[0] != 16711680 and cube.a[1][0][2].miniCubeColor[2] != 65331) {  //красно-зеленое ребро
			std::cout << "4.2.2" << '\n';
			flag = false;
			for (int i = 0; i < 6; i++) {
				if (cube.a[1][0][2].miniCubeColor[i] == 16776960) {   //проверяем наличе там желтого цвета
					flag = true;
				}
			}
			if (!flag) {
				rotateFlag = true;
				rotationFull(5, -6);
				rotationFull(2, 6);
				rotationFull(5, 6);
				rotationFull(2, -6);
				rotationFull(5, 6);
				rotationFull(1, 6);
				rotationFull(5, -6);
				rotationFull(1, -6);
			}
		}

		if (!rotateFlag and cube.a[1][2][0].miniCubeColor[1] != 16737792 and cube.a[1][2][0].miniCubeColor[3] != 255) {  //оранжево-синее ребро
			std::cout << "4.3.3" << '\n';
			flag = false;
			for (int i = 0; i < 6; i++) {
				if (cube.a[1][2][0].miniCubeColor[i] == 16776960) {   //проверяем наличе там желтого цвета
					flag = true;
				}
			}
			if (!flag) {
				rotateFlag = true;
				rotationFull(5, -6);
				rotationFull(3, -6);
				rotationFull(5, 6);
				rotationFull(3, 6);
				rotationFull(5, 6);
				rotationFull(0, -6);
				rotationFull(5, -6);
				rotationFull(0, 6);
			}
		}

		if (!rotateFlag and cube.a[1][2][2].miniCubeColor[0] != 16711680 and cube.a[1][2][2].miniCubeColor[3] != 255) {  //красно-синее ребро
			std::cout << "4.4.4" << '\n';
			flag = false;
			for (int i = 0; i < 6; i++) {
				if (cube.a[1][2][2].miniCubeColor[i] == 16776960) {   //проверяем наличе там желтого цвета
					flag = true;
				}
			}
			if (!flag) {
				rotateFlag = true;
				rotationFull(5, 6);
				rotationFull(3, 6);
				rotationFull(5, -6);
				rotationFull(3, -6);
				rotationFull(5, -6);
				rotationFull(1, -6);
				rotationFull(5, 6);
				rotationFull(1, 6);
			}
		}
		

	}
}


bool YellowCrossSloved(RCube& cube) {
	//  16711680  16737792   65331    255       16777215  16776960
	//  красный  оранжевый  зеленый   синий     белый     желтый 
	if (WhiteCrossSloved(cube) and
		WhiteCorrectCrossSloved(cube) and
		FirstLayerSloved(cube) and
		SecondLayerSloved(cube) and
		cube.a[2][0][1].miniCubeColor[5] == 16776960 and 
		cube.a[2][1][0].miniCubeColor[5] == 16776960 and
		cube.a[2][1][2].miniCubeColor[5] == 16776960 and
		cube.a[2][2][1].miniCubeColor[5] == 16776960) {
		return true;
	}
	return false;
}

void SloveYellowCross(RCube& cube) {
	while (!YellowCrossSloved(cube)) {
		int Ycount = 0;
		bool YGflag = false, YOflag = false, YBflag = false, YRflag = false;
		if (cube.a[2][0][1].miniCubeColor[5] == 16776960) {
			Ycount++;
			YGflag = true;
		}
		if (cube.a[2][1][0].miniCubeColor[5] == 16776960) {
			Ycount++;
			YOflag = true;
		}
		if (cube.a[2][1][2].miniCubeColor[5] == 16776960) {
			Ycount++;
			YRflag = true;
		}
		if (cube.a[2][2][1].miniCubeColor[5] == 16776960) {
			Ycount++;
			YBflag = true;
		}

		if (Ycount == 2) {
			if (YOflag and YRflag) {
				rotationFull(2, 6);
				rotationFull(0, 6);
				rotationFull(5, -6);
				rotationFull(0, -6);
				rotationFull(5, 6);
				rotationFull(2, -6);
			}
			else if (YBflag and YGflag) {
				rotationFull(5, 6);

				rotationFull(2, 6);
				rotationFull(0, 6);
				rotationFull(5, -6);
				rotationFull(0, -6);
				rotationFull(5, 6);
				rotationFull(2, -6);
			}
			else if (YBflag and YRflag) {
				rotationFull(2, 6);
				rotationFull(0, 6);
				rotationFull(5, -6);
				rotationFull(0, -6);
				rotationFull(5, 6);
				rotationFull(0, 6);
				rotationFull(5, -6);
				rotationFull(0, -6);
				rotationFull(5, 6);
				rotationFull(2, -6);
			}
			else if (YBflag and YOflag) {
				rotationFull(5, 6);

				rotationFull(2, 6);
				rotationFull(0, 6);
				rotationFull(5, -6);
				rotationFull(0, -6);
				rotationFull(5, 6);
				rotationFull(0, 6);
				rotationFull(5, -6);
				rotationFull(0, -6);
				rotationFull(5, 6);
				rotationFull(2, -6);
			}
			else if (YGflag and YOflag) {
				rotationFull(5, 6);
				rotationFull(5, 6);

				rotationFull(2, 6);
				rotationFull(0, 6);
				rotationFull(5, -6);
				rotationFull(0, -6);
				rotationFull(5, 6);
				rotationFull(0, 6);
				rotationFull(5, -6);
				rotationFull(0, -6);
				rotationFull(5, 6);
				rotationFull(2, -6);
			}
			else if (YGflag and YRflag) {
				rotationFull(5, -6);

				rotationFull(2, 6);
				rotationFull(0, 6);
				rotationFull(5, -6);
				rotationFull(0, -6);
				rotationFull(5, 6);
				rotationFull(0, 6);
				rotationFull(5, -6);
				rotationFull(0, -6);
				rotationFull(5, 6);
				rotationFull(2, -6);
			}
		}
		if (Ycount == 1) {
			rotationFull(2, 6);  //палка
			rotationFull(0, 6);
			rotationFull(5, -6);
			rotationFull(0, -6);
			rotationFull(5, 6);
			rotationFull(2, -6);

			//rotationFull(5, 6);  //U2
			//rotationFull(5, 6);


		}
		

	}
}



void keys(unsigned char key, int, int)
{
	if (cube.RotNOW == -1 && key >= '0' && key <= '5')
	{ 
		cube.Rotate(key - '0', 6);
		display();
	}

	if (cube.RotNOW == -1 && key == '6') {
		sloveWhiteCross(cube);
		sloveCorrectWhiteCross(cube);
		SloveFirstLayer(cube);
		SloveSecondLayer(cube);
		SloveYellowCross(cube);
	}
	if (key == '7') {
		cube.clear(CUBE_SIZE, c);
		glutPostRedisplay();
	}
	if (key == '8') {
		Movment = 1 - Movment;
	}
	if (key == '9') {
		
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