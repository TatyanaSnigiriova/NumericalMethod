// Разделённые разности
// Приближение функции полиномом Ньютона
// https://vk.com/snigiriovakoles

#include "stdafx.h"
#include "locale"
#include "iostream"
#include <fstream>

using namespace std;

int factorial(int num)
{
	if (num == 0) return 1;
	return num * factorial(num - 1);
}

long float Cnk(int n, int k)
{
	return factorial(n) / (factorial(k) * factorial(n - k));
}


// Просто расскоментируйте один из тестовых примеров в каждой функции перед main, или напишите свой. 

// Исходная функция F
long float F(long float x)
{
	// 1)
	// return 1 / (1 + x * x);
	// 2)
	return  sin(x);
	// 3) 
	// return x * exp(x);
	// 4)
	// return tan(pow(x, 2)) / (x + 1);
}

// Текстовое представление функции F
char * getF()
{
	// 1)
	// return "F = 1 / (1 + x * x)";
	// 2)
	return "F = sin(x)";
	// 3)
	// return "F = x * e^x";
	// 4)
	// return "F = tan(x^2)) / (x + 1)";
}


long float * Newton(int, long float, int, int, long float *, long float *);
long float * accuracy(int, long float *, long float *);



int main()
{
	setlocale(LC_ALL, "rus");
	int n;
	cout << "	Данная программа приближает заданную функцию ( в данном случае " << getF() << " ) полиномом Ньютона.\n";
	cout << " Для работы с другими функциями, впишите их в 'F' и задайте текстовое представление в 'getF' перед функцией 'main'.\n\n";
	cout << " Введите количество узловых точек x (n >= 2) для функции " << getF() <<":\n";
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
		Y[i] = F(X[i]);


	long float *N = Newton(n, dX, countIntermediateX, countXY, X, Y);
	long float *R = accuracy(countXY, Y, N);

	ofstream FOut;
	FOut.open("Newton.txt", ios::out);
	if (FOut)
	{
		cout << "\n Данные значения выгружаются в файл Newton.txt,\nимпортируйте документ в формат Excel для лучшей читаемости данных;\n";
		cout << endl << "|---|----------|-----------|-------------|-----------|" << endl;
		cout << "| i |    x     |     y     |      N      |     R     |" << endl;
		cout << "|---|----------|-----------|-------------|-----------|" << endl;
		FOut << "i x y N R \n";
		for (int i = 0; i < countXY; i++)
		{
			printf("|%3i|", i);
			if (X[i] < 0) printf("%2.6f |", X[i]);
			else printf(" %2.6f |", X[i]);
			if (Y[i] < 0) printf("%3.7f |", Y[i]);
			else printf(" %3.7f |", Y[i]);
			if (N[i] < 0) printf(" %3.7f  |", N[i]);
			else printf("  %3.7f  |", N[i]);
			if (R[i] < 0) printf("%1.7f |", R[i]);
			else printf(" %1.7f |", R[i]);
			cout << "\n|---|----------|-----------|-------------|-----------|" << endl;
			FOut << i << " " << X[i] << " " << Y[i] << " " << N[i] << " " << R[i] << "\n";
		}
		FOut.close();
	}
	cout << "\n\n";

	delete[] R;
	delete[] N;
	delete[] Y;
	delete[] X;
	return 0;
}


long float * Newton(int n, long float dX, int countIntermediateX, int countXY, long float * X, long float * Y)
{
	long float *N = new long float[countXY];
	for (int i = 0; i < countXY; i++)
		N[i] = 0;

	long float *kY = new long float[n]; // Набор значений производных k степени функции F в точке x0
	// kY[0] = F(x0), kY[1] = F'(x0), ..., kY[k] = F^k(x0)
	for (int k = 0; k < n; k++)
	{
		kY[k] = 0;
		for (int j = 0; j <= k; j++)
			kY[k] += pow(-1, j) * Cnk(k, k - j) * Y[(k - j) * (countIntermediateX + 1)]; 
	}

	long float hugeMultiplier;
	for (int i = 0; i < countXY; i++)
	{
		for (int k = 0; k < n; k++)
		{
			hugeMultiplier = 1;
			for (int j = 0; j < k; j++)
				hugeMultiplier *= (X[i] - X[j*(countIntermediateX + 1)]);
			hugeMultiplier /= factorial(k);
			N[i] += kY[k] * hugeMultiplier / pow(dX, k);
		}
	}

	return N;
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
