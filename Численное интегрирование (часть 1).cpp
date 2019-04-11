// ��������� �������������� (����� 1)
// ������ ��������������� (�����, ������ � �����������) � ����� ��������.

#include "stdafx.h"
#include <iostream>
#include <math.h>
#include "clocale"
#include <fstream>

using namespace std;

// ������ ���������������� ���� �� �������� �������� � ������ ������� ����� main, ��� �������� ����. 

// ������� ���� ���� �������
long float  F(long float x)
{
	// 1)
	// return  sin(x);
	// 2) 
	return x * exp(x);
	// 3)
	// return tan(pow(x, 2)) / (x + 1);
}

// ������ ����������� ������� F
long float  FFirstDerivative(long float x)
{
	// 1)
	// return  cos(x);
	// 2) 
	return exp(x);
	// 3)
	// return (2 * x * (x + 1) * pow(cos(x * x), - 2) - tan(x * x)) / pow(x + 1, 2);
}

// ������ ����������� ������� F
long float  FSecondDerivative(long float x)
{
	// 1)
	// return  - sin(x);
	// 2) 
	return exp(x);
	// 3)
	/*
	long float Answer = x * ( 4 * x * (x + 1) * tan(x * x) - 1) + 1;
	Answer *= tan(x * x) + (x + 1) * pow(cos(x * x), -2);
	Answer *= 2 * (x + 1) * pow(x + 1, - 4);
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


long float mLeftRectangle(long float, long float, int);
long float mRightRectangle(long float, long float, int);
long float RForMLAndRRectangle(long float, long float, int);
long float mMiddleRectangle(long float, long float, int);
long float RForMMiddleRectangle(long float, long float, int);
long float mTrapeze(long float, long float, int);
long float RForMTtrapeze(long float, long float, int);

int main()
{
	setlocale(LC_ALL, "rus");
	float a = 1, b = 0;
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
	cout << "\n ������� ���������� ������������� ����� ��� ������� [a; b] (countIntermediateX > 0):\n";
	do
	{
		cout << " countIntermediateX = ";
		cin >> countIntermediateX;
		if (countIntermediateX <= 0)  cout << "������� countIntermediateX > 0\n";
	} while (countIntermediateX <= 0);
	cout << endl;

	long float integralVLRect, rVLRect, integralVRRect, rVRRect, integralVMRect, rVMRect, integralVTrapeze, rVTrapeze;
	integralVLRect = mLeftRectangle(a, b, countIntermediateX);
	rVLRect = RForMLAndRRectangle(a, b, countIntermediateX);
	integralVRRect = mRightRectangle(a, b, countIntermediateX);
	rVRRect = RForMLAndRRectangle(a, b, countIntermediateX);
	integralVMRect = mMiddleRectangle(a, b, countIntermediateX);
	rVMRect = RForMMiddleRectangle(a, b, countIntermediateX);
	integralVTrapeze = mTrapeze(a, b, countIntermediateX);
	rVTrapeze = RForMTtrapeze(a, b, countIntermediateX);

	cout << "�������� ��������� ������� " << getF() << " ������������ � ������� ������:\n";
	cout << "	����� ��������������� - Sl = " << integralVLRect << endl;
	cout << "	  |Rl| <= " << rVLRect << endl;
	cout << "	������ ��������������� - Sr = " << integralVRRect << endl;
	cout << "	  |Rr| <= " << rVRRect << endl;
	cout << "	������� ��������������� - Sm = " << integralVMRect << endl;
	cout << "	  |Rm| <= " << rVTrapeze << endl;
	cout << "	�������� - S� = " << integralVTrapeze << endl;
	cout << "	  |R| <= " << rVTrapeze << endl;

    return 0;
}


long float mLeftRectangle(long float a, long float b, int countIntermediateX)
{
	long float h = (b - a) / (countIntermediateX + 1); // ��� ����� x(i) � x(i+1)
	int n = countIntermediateX + 2; // ����� ���������� ����� 
	
	long float integralV = 0;
	for (int i = 0; i < n - 1; i++)
		integralV += F(a + i * h);
	integralV *= h;

	return integralV;
}

long float mRightRectangle(long float a, long float b, int countIntermediateX)
{
	long float h = (b - a) / (countIntermediateX + 1); // ��� ����� x(i) � x(i+1)
	int n = countIntermediateX + 2; // ����� ���������� ����� 

	long float integralV = 0;
	for (int i = 1; i < n; i++)
		integralV += F(a + i * h);
	integralV *= h;

	return integralV;
}

long float RForMLAndRRectangle(long float a, long float b, int countIntermediateX)
{
	long float h = (b - a) / (countIntermediateX + 1); // ��� ����� x(i) � x(i+1)
	int n = countIntermediateX + 2; // ����� ���������� ����� 

	long float M1 = abs(FFirstDerivative(a)), M1check;
	for (int i = 1; i < n; i++)
	{
		M1check = abs(FFirstDerivative(a + i * h));
		if (M1check > M1)
			M1 = M1check;
	}
	return pow(b - a, 2) * M1 / (2 * n - 1);
}

long float mMiddleRectangle(long float a, long float b, int countIntermediateX)
{
	long float h = (b - a) / (countIntermediateX + 1); // ��� ����� x(i) � x(i+1)
	int n = countIntermediateX + 2; // ����� ���������� ����� 

	long float integralV = 0;
	for (int i = 1; i < n; i++)
		integralV += F(a + (i - 0.5 ) * h);
	integralV *= h;

	return integralV;
}

long float RForMMiddleRectangle(long float a, long float b, int countIntermediateX)
{
	long float h = (b - a) / (countIntermediateX + 1); // ��� ����� x(i) � x(i+1)
	int n = countIntermediateX + 2; // ����� ���������� ����� 

	long float M2 = abs(FSecondDerivative(a)), M2check;
	for (int i = 1; i < n; i++)
	{
		M2check = abs(FSecondDerivative(a + i * h));
		if (M2check > M2)
			M2 = M2check;
	}
	return pow(b - a, 3) * M2 / (24 * pow(n - 1, 2));
}

long float mTrapeze(long float a, long float b, int countIntermediateX)
{
	long float h = (b - a) / (countIntermediateX + 1); // ��� ����� x(i) � x(i+1)
	int n = countIntermediateX + 2; // ����� ���������� ����� 

	long float integralV = (F(a) + F(b)) / 2;
	for (int i = 1; i < n - 1; i++)
		integralV += F(a + i * h);
	integralV *= h;

	return integralV;
}

long float RForMTtrapeze(long float a, long float b, int countIntermediateX)
{
	long float h = (b - a) / (countIntermediateX + 1); // ��� ����� x(i) � x(i+1)
	int n = countIntermediateX + 2; // ����� ���������� ����� 

	long float M2 = abs(FSecondDerivative(a)), M2check;
	for (int i = 1; i < n; i++)
	{
		M2check = abs(FSecondDerivative(a + i * h));
		if (M2check > M2)
			M2 = M2check;
	}
	return pow(b - a, 3) * M2 / 12;
}