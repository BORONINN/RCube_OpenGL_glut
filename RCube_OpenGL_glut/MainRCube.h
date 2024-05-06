#pragma once
#include <stack>
#include <iostream>
#include <vcruntime_string.h>
#include <glut.h>

class mini_cubs27
{
private:
	
public:
	unsigned char _color[4];
	unsigned int miniCubeColor[6]; 
	double size;

	mini_cubs27()
	{
		memset(miniCubeColor, 0, sizeof(miniCubeColor));
		size = 0.0;
	}

	void rotateZ();

	void rotateY();

	void rotateX();

	void rotateX_reverse();

	void rotateZ_reverse();

	void rotateY_reverse();

	void setColor(int i, int color);

	void CoutColor(int color) {
		//  16711680  16737792   65331    255       16777215  16776960
		//  красный  оранжевый  зеленый   синий     белый     желтый 
		if (color == 16711680) {
			std::cout << "red ";
		}
		if (color == 16776960) {
			std::cout << "yellow ";
		}
		if (color == 65331) {
			std::cout << "green ";
		}
		if (color == 255) {
			std::cout << "blue ";
		}
		if (color == 16777215) {
			std::cout << "white ";
		}
		if (color == 16737792) {
			std::cout << "orange ";
		}
		if (color == 0){
			std::cout << "0 ";
		}
	}

	void getColor()
	{
		for (int i = 0; i < 6; i++){
			CoutColor(miniCubeColor[i]);
		}
		//std::cout << '\n';
		
	}

	unsigned char* at(int i);

	void draw();

	void draw(double x, double y, double z);


};

class RCube
{


public:
	mini_cubs27 a[3][3][3];

	double size;

	unsigned int color[6];

	int rotate[6];

	int RotNOW;

	RCube(){}

	RCube(double size, unsigned int* color)
	{
		clear(size, color);
	}


	void clear(double size, unsigned int* color);

	void draw();

	void rot90(int idx, int sign);

	void Rotate(int idx, int angle);

	void rot902(int idx, int sign);

	void Rotate2(int idx, int angle);

	bool WhiteCrossSloved();

	void sloveWhiteCross();

private:
	int _angle[4];
	bool is_rotating[4][4][4];
	mini_cubs27 tmp[4][4];
};