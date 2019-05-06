// Численное решение обыкновенных ДУ методами Эйлера и Рунге - Кутта)
// Лекции по теме: https://www.youtube.com/watch?v=nhhup8wxVbw&list=PLDrmKwRSNx7L3zu3Piuk6sJ3KvGWnv9-d&index=22, https://www.youtube.com/watch?v=6VFVeCTlSu8&list=PLDrmKwRSNx7L3zu3Piuk6sJ3KvGWnv9-d&index=23
// Огромная благодарность Бояршинову Борису Сергеевичу и Фоминой Любови Николаевне за их преподавательский труд
// https://vk.com/snigiriovakoles

#include "stdafx.h"
#include <iostream>
#include <math.h>
#include "clocale"
#include <fstream>
using namespace std;


// Введите сюда свою функцию
long float  f(long float x, long float y) // y' = F(x, y);
{
	return x + y;
}

// Текстовое представление дифференциального уравнения первого порядка    
char * getY_()
{
	return "y' = x + y";
}

long float getY(long float x, long float A)
{
	return A * exp(x) - 1 - x;
}

long float * exactY(int n, long float * x, long float y0)
{
	long float *y = new long float[n];
	y[0] = y0;
	long float A = (y0 + 1 + x[0]) / exp(x[0]);
	for (int i = 1; i < n; i++)
		y[i] = getY(x[i], A);

	return y;
}

long float * Euler(int, long float *, long float);
long float * ImprovedEuler(int, long float *, long float);
long float * Runge_Kutta(int, long float *, long float);


int main()
{
	setlocale(LC_ALL, "rus");
	int n;
	cout << "	Данная программа решает заданное дифференциальное уравнение " << getY_() << " первого порядка методами Эйлера и Рунге - Кутта.\n";
	cout << " Для работы с другими дифференциальными уравнениями, впишите функцию f / y' = f(x, y) в 'f' и задайте текстовое представление в 'getY_' перед функцией 'main'.\n";
	cout << " Если Вам известно точное решение, расскомментируйте многострочный коммнтарий в функции 'main', \n";
	cout << "задайте его представление в функцию 'getY' и к-т A в функции 'exactY' (К-ты перед e^x расчитываются через начальное приближение).\n\n";
	cout << " Введите количество экспериментальных значений (n >= 2):\n";
	do
	{
		cout << " n = ";
		cin >> n;
		if (n < 2)  cout << "Введите n >= 2\n";
	} while (n < 2);

	long float *x = new long float[n];
	long float y0;
	
	cout << "\n Введите начальное приближение (x0, y0):\n	x0 = ";
	cin >> x[0];
	cout << "	y0 = ";
	cin >> y0;
	
	long float dx;
	cout << "\n Введите расстояние между xi и xi+1 (dx > 0):\n";
	do
	{
		cout << " dx = ";
		cin >> dx;
		if (dx <= 0)  cout << "Введите dx > 0\n";
	} while (dx <= 0);

	for (int i = 0; i < n - 1; i++)
		x[i + 1] = x[i] + dx;

	
	long float *yE = Euler(n, x, y0);
	long float *yIE = ImprovedEuler(n, x, y0);
	long float *yRK = Runge_Kutta(n, x, y0);
	/*
	long float *y = exactY(n, x, y0);
	*/
	// /*
	long float *y = new long float[n];
	memcpy(y, yRK, sizeof(long float)*n);
	// */

	ofstream FOut;
	FOut.open("DY.txt", ios::out);
	if (FOut)
	{
		cout << "\n Данные значения выгружаются в файл DY.txt,\nимпортируйте документ в формат Excel для лучшей читаемости данных;\n";
		cout << endl << "|---|----------|-------------|-------------|----------|-------------|----------|-------------|----------|" << endl;
		cout << "| i |    x     |      y      |    Euler    |    r     |   I Euler   |    r     | Runge–Kutta |    r     |" << endl;
		cout << "|---|----------|-------------|-------------|----------|-------------|----------|-------------|----------|" << endl;
		FOut << "i x y Euler r IEuler r RK r\n";
		for (int i = 0; i < n; i++)
		{
			printf("|%3i|", i);
			if (x[i]<0) printf("%6f |", x[i]);
			else printf(" %6f |", x[i]);
			if (y[i]<0) printf("%11f |", y[i]);
			else printf(" %11f |", y[i]); 
			if (yE[i]<0) printf("%11f |", yE[i]);
			else printf(" %11f |", yE[i]);
			printf(" %6f |", abs(yE[i] - y[i]));
			if (yIE[i]<0) printf("%11f |", yIE[i]);
			else printf(" %11f |", yIE[i]);
			printf(" %6f |", abs(yIE[i] - y[i]));
			if (yRK[i]<0) printf("%11f |", yRK[i]);
			else printf(" %11f |", yRK[i]);
			printf(" %6f |", abs(yRK[i] - y[i]));
			cout << endl << "|---|----------|-------------|-------------|----------|-------------|----------|-------------|----------|" << endl;
			FOut << i << " " << x[i] << " " << y[i] << " " << yE[i] << " " << abs(yE[i] - y[i]) << " " << yIE[i] << " " << abs(yIE[i] - y[i]) << " " << yRK[i] << " " << abs(yRK[i] - y[i]) << "\n";
		}
		FOut.close();
	}
	cout << "\n\n";

	delete[] yRK;
	delete[] yIE;
	delete[] yE;
	delete[] y;
	delete[] x;

	return 0;
}


long float * Euler(int n, long float * x, long float y0)
{
	long float dx = x[1] - x[0];
	long float *y = new long float[n];
	y[0] = y0;
	for (int i = 0; i < n - 1; i++)
		y[i + 1] = y[i] + f(x[i], y[i]) * dx;

	return y;
}

long float * ImprovedEuler(int n, long float * x, long float y0)
{
	long float dx = x[1] - x[0];
	long float *y = new long float[n];
	y[0] = y0;
	for (int i = 0; i < n - 1; i++)
	{
		y[i + 1] = f(x[i], y[i]);
		y[i + 1] = y[i] + dx *(y[i + 1] + f(x[i + 1], y[i] + y[i + 1] * dx)) / 2;
	}
	return y;
}

long float * Runge_Kutta(int n, long float * x, long float y0)
{
	long float dx = x[1] - x[0];
	long float *y = new long float[n];
	y[0] = y0;
	long float *k = new long float[4];
	for (int i = 0; i < n - 1; i++)
	{
		k[0] = f(x[i], y[i]) * dx;
		k[1] = f(x[i] + dx / 2, y[i] + k[0] / 2) * dx;
		k[2] = f(x[i] + dx / 2, y[i] + k[1] / 2) * dx;
		k[3] = f(x[i] + dx, y[i] + k[2]) * dx;
		y[i + 1] = y[i] + (k[0] + 2 * k[1] + 2 * k[2] + k[3]) / 6;

	}
	return y;
}
