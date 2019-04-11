// ��������� �������������� (����� 2).
// ����� �������� (����� �������).
// ��������� �������� ����, ��� ������ ����������, �������� �� https://www.youtube.com/watch?v=0Xk1SABeiVo
// �������� ������������� ���������� ������ ���������� � ������� ������ ���������� �� �� ����������������� ����


#include "stdafx.h"
#include <iostream>
#include <math.h>
#include "clocale"

using namespace std;

// ������ ���������������� ���� �� �������� �������� � ������ ������� ����� main, ��� �������� ����. 

// �������� ������� F
long float F(long float x)
{	
	// 1)
	// return  sin(x);
	// 2) 
	return x * exp(x);
	// 3)
	// return tan(pow(x, 2)) / (x + 1);
}

// �������� ����������� ������� F
long float FFourthDerivative(long float x)
{
	// 1) 
	// return  sin(x);
	// 2)
	return exp(x);
	// 3) 
	/*
	long float X = x * x;
	long float Answer = 256 * pow(x, 4) * pow(pow(tan(X), 2) + 1, 2) * tan(X);
	Answer -= (64 * pow(x, 3) * pow((pow(tan(X), 2) + 1),2)) / (x + 1);
	Answer -= (128 * pow(x, 3) * pow(tan(X),2) * (pow(tan(X), 2) + 1)) / (x + 1);
	Answer += 96 * x * x * pow(pow(tan(X), 2) + 1, 2);
	Answer += 192 *  x * x * (pow(tan(X), 2) + 1) * pow(tan(X), 2);
	Answer += (96 *  x * x  * (pow(tan(X), 2) + 1) * tan(X)) / (pow(x+1,2));
	Answer -= (96 * x * tan(X) * (pow(tan(X), 2) + 1)) / (x + 1);
	Answer -= (48 * x * (pow(tan(X), 2) + 1)) / (pow(x + 1,3));
	Answer += 24 * (pow(tan(X), 2) + 1) * tan(X);
	Answer += (24 * (pow(tan(X), 2) + 1)) / (pow(x + 1, 2));
	Answer += (24 * tan(X)) / (pow(x + 1, 4));
	Answer /= (x + 1);
	return Answer;
	*/
}

// ��������� ������������� ������� F  
char * getF()
{
	// 1)
	// return "F = sin(x)";
	// 2)
	return "F = x * e^x";
	// 3)
	// return "F = tan(x^2)) / (x + 1)";
}

long float mSimpson(long float, long float, int);
long float RForMSimpson(long float, long float, int);

int main()
{
	setlocale(LC_ALL, "rus");

	float a = 1, b = 0;
	// 1) a = 0, b = 3.1415926535897932384626433832795
	// 2) a = 0, b = 1
	// 3) a = 0, b = 1

	cout << "��� ������� " << getF() << " ������� ������� �������������� a � b:\n";
	while (a >= b)
	{
		cout << "	a = ";
		cin >> a;
		cout << "	b = ";
		cin >> b;
		cout << "�� ����� a = " << a << ", b = " << b << ".\n";
		if (a >= b)
			cout << "������� a < b.\n";
	}

	int countIntermediateX;
	cout << "\n ������� ���������� ������������� ����� ��� ������� [a; b]\n  (countIntermediateX >= 1 and countIntermediateX % 2 == 1):\n";
	do
	{
		cout << "	countIntermediateX = ";
		cin >> countIntermediateX;
		if (countIntermediateX < 1 || (countIntermediateX % 2) != 1)  cout << "������� �������� countIntermediateX >= 1.\n";
	} while (countIntermediateX < 1 || (countIntermediateX % 2) != 1 );
	cout << endl;
	long float integralVSimpson, rForSimpson;
	integralVSimpson = mSimpson(a, b, countIntermediateX);
	rForSimpson = RForMSimpson(a, b, countIntermediateX);
	cout << "�������� ��������� ������� " << getF() << " ������������ � ������� ������ ��������:\n";
	cout << "	S = " << integralVSimpson << endl;
	cout << "  �������� ���������� |RS| <= " << rForSimpson << endl;
	return 0;
}

long float mSimpson(long float a, long float b, int countIntermediateX)
{
	long float h;
	h = (b - a) / (countIntermediateX + 1);
	int n = countIntermediateX + 2;
	long float *X = new long float[n];
	
	X[0] = a;
	for (int i = 1; i < n - 1; i++)
		X[i] = X[0] + i*h;
	X[n - 1] = b;

	long float *Y = new long float[n];
	for (int i = 0; i < n; i++)
		Y[i] = F(X[i]);

	long float integralV = 0;
	for (int i = 1; i < n - 1; i += 2)
		integralV += Y[i - 1] + 4 * Y[i] + Y[i + 1];
	integralV *= h / 3;
	
	return integralV;
}

long float RForMSimpson(long float a, long float b, int countIntermediateX)
{
	long float h = (b - a) / (countIntermediateX + 1); // ��� ����� x(i) � x(i+1)
	int n = countIntermediateX + 2; // ����� ���������� ����� 

	long float M4 = abs(FFourthDerivative(a)), M4check;
	for (int i = 1; i < n; i++)
	{
		M4check = abs(FFourthDerivative(a + i * h));
		if (M4check > M4)
			M4 = M4check;
	}
	return (b - a)* M4 * h * h * h * h/ 180;
}

