// Численное решение трансцендентного уравнения с одним неизвестным.
// Метод дихотомии, золотого сечения и хорд. 
//

#include "stdafx.h"
#include "math.h"
#include <iostream>
#include <clocale>
using namespace std;

struct ansverForF
{
	double x, r;
	bool twoAnswers = false;
	int iter = 0;
	double getX() { return x; }
	double getR() { return r; }
	void setAns(double xSet, double rSet) { x = xSet; r = rSet;}
	void setTwoAns() { twoAnswers = true; }
	bool isTwoAnsvers() { return twoAnswers; }
	void plusIter() { iter++; }
	int getIter() { return iter; }
};

// выразите y и запишите сюда Ваше уравнение
float F(float x)
{
	return x * x - 2;
}

char * getF()
{
	return "F = x^2 -2";
} 

ansverForF mofDichotomy(float a, float b, float eps )
{
	float xMiddle, r;
	ansverForF ans;
	if (F(a)*F(b) > 0)
		cout << "Выберите другой промежуток, на котором произведение F(a) * F(b) <= 0.\n ";
	else
	{
		if (F(a) * F(b) == 0)
		{
			if (F(a) == 0 && F(b) == 0)
				ans.setTwoAns();
			else
			{
				if (F(a) == 0)
					ans.setAns(a, 0);
				else
					ans.setAns(b, 0);
			}
		}
		else
		{
			float xLeft = a, xRight = b;
			r = xRight - xLeft;
			while (r > eps)
			{
				xMiddle = xLeft + (xRight - xLeft) / 2;

				if (F(xMiddle) * F(xRight) < 0)
					xLeft = xMiddle;
				else
				{
					if (F(xLeft) * F(xMiddle) < 0)
						xRight = xMiddle;
					else
					{
						r = 0;
						break;
					}
				}
				r /= 2;
				ans.plusIter();
			}
			ans.setAns(xMiddle, r);
		}
	}
	return ans;
}

ansverForF mOfGoldenSection(float a, float b, float eps)
{
	float xGolden, r, Phidias = 0.618;
	ansverForF ans;
	bool ansFind = false;

	if (F(a)*F(b) > 0)
		cout << "Выберите другой промежуток, на котором произведение F(a) * F(b) <= 0.\n ";
	else
	{
		if (F(a) * F(b) == 0)
		{
			if (F(a) == 0 && F(b) == 0)
				ans.setTwoAns();
			else
			{
				if (F(a) == 0)
					ans.setAns(a, 0);
				else
					ans.setAns(b, 0);
			}
		}
		else
		{
			float xLeft = a, xRight = b;
			float xGoldenLeft, xGoldenRight, fXGoldenLeft, fXGoldenRight;
			r = abs(xRight - xLeft);
			while (!ansFind && r > eps)
			{
				xGoldenRight = xLeft + (xRight - xLeft) * Phidias;
				xGoldenLeft = xRight - (xRight - xLeft) * Phidias;
				fXGoldenLeft = F(xGoldenLeft);
				fXGoldenRight = F(xGoldenRight);

				if (fXGoldenLeft * fXGoldenRight == 0)
				{
					if (fXGoldenLeft == 0 && fXGoldenRight == 0)
						ans.setTwoAns();
					else
					{
						if (fXGoldenLeft == 0)
							ans.setAns(xGoldenLeft, 0);
						else
							ans.setAns(xGoldenRight, 0);
					}
					ansFind = true;
					break;
				}
				else
				{
					// Значения имеют одинаковый знак;
					if (fXGoldenLeft * fXGoldenRight > 0)
					{
						// Проверяем, с каким концом он совпадает
						if (fXGoldenRight * F(xLeft) > 0)
							xLeft = xGoldenRight;
						else
							xRight = xGoldenLeft;
					}
					else
					{
						if (fXGoldenLeft * F(xLeft) > 0)
							xLeft = xGoldenLeft;
						else
							xRight = xGoldenLeft;
						if (fXGoldenRight * F(xRight) > 0)
							xRight = xGoldenRight;
						else
							xLeft = xGoldenRight;
					}
					r = abs(xRight - xLeft);
				}
				ans.plusIter();
			}
			if (!ansFind)
			{
				if (abs(fXGoldenLeft) < abs(fXGoldenRight))
					ans.setAns(xGoldenLeft, r);
				else
					ans.setAns(xGoldenRight, r);
			}
		}
	}
	return ans;
}

ansverForF mOfСhord(float a, float b, float eps)
{
	float xi, r;
	ansverForF ans;
	if (F(a)*F(b) > 0)
		cout << "Выберите другой промежуток, на котором произведение F(a) * F(b) <= 0.\n ";
	else
	{
		if (F(a) * F(b) == 0)
		{
			if (F(a) == 0 && F(b) == 0)
				ans.setTwoAns();
			else
			{
				if (F(a) == 0)
					ans.setAns(a, 0);
				else
					ans.setAns(b, 0);
			}
		}
		else
		{
			float xLeft = a, xRight = b;
			r = xRight - xLeft;
			while (r > eps)
			{
				xi = xLeft - ((F(xLeft) * (xRight - xLeft)) / (F(xRight) - F(xLeft))); 
				if (F(xi) * F(xRight) < 0)
					xLeft = xi;
				else
				{
					if (F(xLeft) * F(xi) < 0)
						xRight = xi;
					else
					{
						r = 0;
						break;
					}
				}
				r = abs(F(xi));
				ans.plusIter();
			}
			ans.setAns(xi, r);
		}
	}
	return ans;
}



int main()
{
	setlocale(LC_CTYPE, "rus");
	float a = 1, b = 0, eps;
	int k = 2;
	cout << "Для функции " << getF() << " введите границы a и b и степень k точности вычислений eps = 10^-k:\n";
	while (a >= b)
	{
		cout << "	a = ";
		cin >> a;
		cout << "	b = ";
		cin >> b;
		cout << "Вы ввели a = " << a << ", b = " << b << ".\n";
		if (a >= b)
			cout << "Введите a < b.\n";
	}
	while (k <= 2)
	{
		cout << "	k = ";
		cin >> k;
		cout << "Вы ввели k = " << k << ".\n";
		if (k <= 2)
			cout << "Введите k > 2.\n";
	}
	eps = pow(10, -k);
	ansverForF ansForDichotomy = mofDichotomy(a, b, eps);
	cout << "Метод дихотомии: \n";
	cout << " Ответ был найден на " << ansForDichotomy.getIter() << " итерации.\n";
	if (ansForDichotomy.isTwoAnsvers())
		cout << "На данном промежутке 2 корня. Введите другой промежуток.\n";
	else
		cout << "Решением уравнения " << getF() << " является x = " << ansForDichotomy.getX() << ", абсолютная погрешность r = " << ansForDichotomy.getR() << ".\n";
	cout << "\n\n";


	ansverForF ansForGoldenSection = mOfGoldenSection(a, b, eps);
	cout << "Метод золотого сечения: \n";
	cout << " Ответ был найден на " << ansForGoldenSection.getIter() << " итерации.\n";

	if (ansForGoldenSection.isTwoAnsvers())
		cout << "На данном промежутке 2 корня. Введите другой промежуток.\n";
	else
		cout << "Решением уравнения " << getF() << " является x = " << ansForGoldenSection.getX() << ", абсолютная погрешность r = " << ansForGoldenSection.getR() << ".\n";
	
	ansverForF ansForChord = mOfСhord(a, b, eps);
	cout << "Метод хорд: \n";
	cout << " Ответ был найден на " << ansForChord.getIter() << " итерации.\n";
	if (ansForChord.isTwoAnsvers())
		cout << "На данном промежутке 2 корня. Введите другой промежуток.\n";
	else
		cout << "Решением уравнения " << getF() << " является x = " << ansForChord.getX() << ", абсолютная погрешность r = " << ansForChord.getR() << ".\n";
	cout << "\n\n";

    return 0;
}

