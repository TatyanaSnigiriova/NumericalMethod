// Многочлен Лагранжа
// Подробное описание того, что вообще происходит, смотрите на https://www.youtube.com/watch?v=n20YcXpZL5E
// Огромная благодарность Бояршинову Борису Сергеевичу и Фоминой Любови Николаевне за их преподавательский труд

#include "stdafx.h"
#include <iostream>
#include <math.h>
#include "clocale"
#include <fstream>


using namespace std;

int factorial(int num)
{
	if (num == 0) return 1;
	return num * factorial(num - 1);
}

// Введите сюда свою функцию
long float  function(long float x)
{
	//return  sin(exp(pow(x, 2)));
	return sin(x);
}

long float * Lagrange(int, long float, int, int, long float *, long float *);
long float * accuracy(int, long float *, long float *);

int main()
{
	setlocale(LC_ALL, "rus");
	int n;
	cout << " Введите количество узловых точек x (n >= 2):\n";
	do
	{
		cout << " n = ";
		cin >> n;
		if (n < 2)  cout << "Введите n >= 2\n";
	} while (n < 2);

	long float X0;
	cout << "\n Введите X0:\n X0 = ";
	cin >> X0;

	long float dX;
	cout << "\n Введите расстояние между узловыми точками Xi,Xj (i,j = {0,...,n-1}; i != j) (dX > 0):\n";
	do
	{
		cout << " dX = ";
		cin >> dX;
		if (dX <= 0)  cout << "Введите dX > 0\n";
	} while (dX <= 0);

	int countIntermediateX; // Вынужденное длинное название
	cout << "\n Введите число промежуточных точек для отрезка XiXj (i,j = {0, ..., n-1}; i != j) (countIntermediateX >= 0):\n";
	do
	{
		cout << " countIntermediateX = ";
		cin >> countIntermediateX;
		if (countIntermediateX < 0)  cout << "Введите countIntermediateX >= 0\n";
	} while (countIntermediateX < 0);

	int countXY = (n - 1) * (countIntermediateX + 1) + 1; // Общее количество точек 
	long float dXIntermediate = dX / (countIntermediateX + 1); 
	long float *X = new long float[countXY];// X0 = X[0], X1 = X[10], x2 = X2 =  X[20]...
	X[0] = X0;
	for (int i = 1; i < countXY; i++)
		X[i] = X[i - 1] + dXIntermediate;

	long float *Y = new long float[countXY];
	for (int i = 0; i < countXY; i++)
		Y[i] = function(X[i]);

	long float *L = Lagrange(n, dX, countIntermediateX, countXY, X, Y);
	long float *R = accuracy(countXY, Y, L);

	ofstream FOut;
	FOut.open("Lagrange.txt", ios::out);
	if (FOut)
	{
		cout << "\n Данные значения выгружаются в файл Lagrange.txt,\nимпортируйте документ в формат Excel для лучшей читаемости данных;\n";
		cout << endl << "|---|----------|-----------|-------------|-----------|" << endl;
		cout << "| i |    x     |     y     |      L      |     R     |" << endl;
		cout << "|---|----------|-----------|-------------|-----------|" << endl;
		FOut << "i x y L R \n";
		for (int i = 0; i < countXY; i++)
		{
			printf("|%3i|", i);
			if (X[i]<0) printf("%2.6f |", X[i]);
			else printf(" %2.6f |", X[i]);
			if (Y[i]<0) printf("%3.7f |", Y[i]);
			else printf(" %3.7f |", Y[i]);
			if (L[i]<0) printf(" %3.7f  |", L[i]);
			else printf("  %3.7f  |", L[i]);
			if (R[i]<0) printf("%1.7f |", R[i]);
			else printf(" %1.7f |", R[i]);
			cout << "\n|---|----------|-----------|-------------|-----------|" << endl;
			FOut << i << " " << X[i] << " " << Y[i] << " " << L[i] << " " << R[i] << "\n";
		}
		FOut.close();
	}
	cout << "\n\n";

	delete[] R;
	delete[] L;
	delete[] Y;
	delete[] X;
	return 0;
}



long float * Lagrange(int n, long float dX, int countIntermediateX, int countXY, long float * X, long float * Y)
{
	long float *L = new long float[countXY];
	for (int i = 0; i < countXY; i++)
		L[i] = 0;

	long float hugeMultiplier;

	for (int xi = 0; xi < countXY; xi++)
	{
		for (int yi = 0; yi < n; yi++) // В функции Лагранжа, помимо (xi) участвуют только узловые точки 
		{
			hugeMultiplier = 1;
			for (int xyj = 0; xyj < n; xyj++)
				if (xyj != yi) // Произведение всех пар X[xi] - X[xyj], где xyj != yi  
					hugeMultiplier *= (X[xi] - X[xyj*(countIntermediateX + 1)]);

			hugeMultiplier /= pow(dX, n - 1) *  factorial(yi) * factorial(n - 1 - yi) * pow(-1, n - 1 - yi);
			L[xi] += Y[yi*(countIntermediateX + 1)] * hugeMultiplier;
		}
	}
	return L;
}

long float * accuracy(int countXY, long float * a, long float * b)
{
	long float *R = new long float[countXY];
	for (int i = 0; i < countXY; i++)
	{
		R[i] = abs(a[i] - b[i]);
	}
	return R;
}

