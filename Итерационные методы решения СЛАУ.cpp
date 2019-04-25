// Итерационные методы решения систем линейных алгебраических уравнений.
// Метод прямой итерации. Метод Гаусса-Зейделя.
// Подробное описание того, что вообще происходит, смотрите на https://www.youtube.com/watch?v=9Ru1M7C7cwQ
// Огромная благодарность Бояршинову Борису Сергеевичу и Фоминой Любови Николаевне за их преподавательский труд
// https://vk.com/snigiriovakoles

#include "stdafx.h"
#include "iostream"
#include "locale"
#include "math.h"

const int N = 3;

double matrixA[N][N] =
{
	{ 4, -1, 1},
	{ 2, 6, -1},
	{1, 2, -3}
};

double f[N] = { 4, 7, 0 };
double x0[N] = { 0, 0, 0};

int checkMatrix();
long float* sumOfVectors(int N, long float *a, long float *b);
long float mulVectorByVector(int na, long float *a, int nb, long float *b);
long float* mulMatrixByVector(int nA, int mA, long float **A, int nb, long float *b);
template <int N>
long float* IterativeMethod(long float(&A)[N][N], long float(&b)[N], long float(&x0)[N], double eps);
long float* mOfGauss_Seidel1(long float(&A)[N][N], long float(&b)[N], long float(&x0)[N], double eps);
long float* mOfGauss_Seidel2(long float(&A)[N][N], long float(&b)[N], long float(&x0)[N], double eps);

using namespace std;


int main()
{
	setlocale(LC_ALL, "rus");
	cout << "	Эта программа реализует алгоритмы нахождения корней системы линейных алгебраических уравнений итерационными методами.\n";
	cout << "Первый алгоритм является реализацией метода  простой  итерации, ";
	cout << "второй - Метода Гаусса-Зейделя в двух вариациях:\n";
	cout << "Если вы посмотрели лекцию, то на 20 минуте Борис Сергеевич решает СЛАУ с использованием метрицы (E - A), ";
	cout << "а на 26 минуте он представляет решение без использования этой матрицы, поэтому я решила написать оба варианта решения.\n\n";
	cout << "Следует подчеркнуть, что в коде нет обработки исходных данных, только сами методы.\n";
	cout << "Поэтому, в большинстве случаев, они не дадут вам сходящееся решение, а в качестве ответа Вы получите начальное приближение.\n\n";
	cout << "Для работы с другими СЛАУ, задайте количество неизвестных N, к-ты системы в матрицу 'MatrixA', ";
	cout << "результирующий вектор f и начальное приближение искомого вектора x перед функцией 'main'.\n\n";

	if (checkMatrix())
	{
		double eps;
		int m = 0;
		cout << "Введите степень m для точности eps = 10^-m (m > 1)\n";
		do
		{
			cout << "	m = ";
			cin >> m;
			cout << endl;
			if (m <= 1) cout << "Введите степень m > 1\n";
		} while (m <= 1);
		m = -m;
		eps = pow(double(10), m);

		long float * x1 = IterativeMethod(matrixA, f, x0, eps);
		cout << "	x = ( ";
		for (int i = 0; i < N; i++)
			cout << x1[i] << " ";
		cout << ").\n\n";

		long float * x2 = mOfGauss_Seidel1(matrixA, f, x0, eps);
		cout << "	x = ( ";
		for (int i = 0; i < N; i++)
			cout << x2[i] << " ";
		cout << ").\n\n";


		long float * x3 = mOfGauss_Seidel2(matrixA, f, x0, eps);
		cout << "	x = ( ";
		for (int i = 0; i < N; i++)
			cout << x3[i] << " ";
		cout << ").\n";

		return 0;
	}
}

	
int checkMatrix()
{
	/*
		Тут должна быть проверка на диагональные элементы матрицы:
		По возможности поменять строки исходной матрицы местами, так, чтобы диагональные элементы не равнялись 0;
		Если это невозможно, вернуть 0.
		И т.д.
		Но мне лень))))))))))))))))))))))
		Если где-нибудь буду это использовать, то, может, добавлю сюда проверки, а пока так.
	*/
		return 1;
}

long float* sumOfVectors(int N, long float *a, long float *b)
{
	for (int i = 0; i < N; i++)
		a[i] += b[i];
	return a;
}

long float mulVectorByVector(int na, long float *a, int nb, long float *b)
{
	long float c = 0;
	if (na == nb)
	{
		for (int i = 0; i < na; i++)
			c += a[i] * b[i];
	}
	else cout << "Количество элементов в каждом входном векторе должно совпадать!\n\n";
	return c;
}

long float* mulMatrixByVector(int nA, int mA, long float **A, int nb, long float *b)
{
	long float* c = new long float[nA];
	if (mA == nb)
	{
		for (int i = 0; i < nA; i++)
		{
			c[i] = A[i][0] * b[0];
			for (int j = 1; j < nb; j++)
				c[i] += A[i][j] * b[j];
		}
	}
	else cout << "Количество элементов в строке матрицы должно совпадать с размером входного вектора!\n\n";
	return c;
}

template <int N>
long float* IterativeMethod(long float(&A)[N][N], long float(&b)[N], long float(&x0)[N], double eps)
{
	long float ** E_A = new long float *[N];

	for (int i = 0; i < N; i++)
	{
		E_A[i] = new long float[N];
		for (int j = 0; j < N; j++)
			E_A[i][j] = -A[i][j];
		E_A[i][i] += 1;
	}

	long float * x = new long float[N];
	long float * xPrevious = new long float[N];
	int countOfIter = 0;

	// Не хочу писать do - while (Если хочешь - сам пиши :p)
	for (int i = 0; i < N; i++)
		xPrevious[i] = x0[i];

	x = sumOfVectors(N, mulMatrixByVector(N, N, E_A, N, xPrevious), b);

	bool checkEps = true;
	for (int i = 0; i < N; i++)
		if (abs(x[i] - xPrevious[i]) > eps)
		{
			checkEps = false;
			break;
		}
	countOfIter++;

	while (!checkEps && countOfIter < 50)
	{
		for (int i = 0; i < N; i++)
			xPrevious[i] = x[i];

		x = sumOfVectors(N, mulMatrixByVector(N, N, E_A, N, xPrevious), b);
		
		checkEps = true;
		for (int i = 0; i < N; i++)
			if (abs(x[i] - xPrevious[i]) > eps )
			{
				checkEps = false;
				break;
			}
		countOfIter++;
	}

	if (countOfIter == 50)
	{
		cout << "Выберите другое начальное приближение. В качестве ответа, в данном случае, IterativeMethod вернул заданный Вами вектор x.\n";
		for (int i = 0; i < N; i++)
			x[i] = x0[i];
	}
	else
		cout << "За " << countOfIter << " итераций было найдено решение СЛАУ методом простой итерации:\n";
	return x;
}

long float* mOfGauss_Seidel1(long float(&A)[N][N], long float(&b)[N], long float(&x0)[N], double eps)
{
	long float ** E_A = new long float *[N];

	for (int i = 0; i < N; i++)
	{
		E_A[i] = new long float[N];
		for (int j = 0; j < N; j++)
			E_A[i][j] = -A[i][j];
		E_A[i][i] += 1;
	}

	long float * x = new long float[N];
	long float * xPrevious = new long float[N];
	int countOfIter = 0;

	// Не хочу писать do - while (Ну правда, я его пишу максимум в 'main', Питон бы точно этого не одобрил )
	for (int i = 0; i < N; i++)
	{
		xPrevious[i] = x0[i];
		x[i] = x0[i];
	}

	for (int i = 0; i < N; i++)
		x[i] = mulVectorByVector(N, E_A[i], N, x) + b[i];

	bool checkEps = true;
	for (int i = 0; i < N; i++)
		if (abs(x[i] - xPrevious[i]) > eps)
		{
			checkEps = false;
			break;
		}
	countOfIter++;

	while (!checkEps && countOfIter < 50)
	{
		for (int i = 0; i < N; i++)
			xPrevious[i] = x[i];

		for (int i = 0; i < N; i++)
			x[i] = mulVectorByVector(N, E_A[i], N, x) + b[i];

		checkEps = true;
		for (int i = 0; i < N; i++)
			if (abs(x[i] - xPrevious[i]) > eps)
			{
				checkEps = false;
				break;
			}
		countOfIter++;
	}

	if (countOfIter == 50)
	{
		cout << "Выберите другое начальное приближение. В качестве ответа, в данном случае, mOfGauss_Seidel1 вернул заданный Вами вектор x.\n";
		for (int i = 0; i < N; i++)
			x[i] = x0[i];
	}
	else
		cout << "За " << countOfIter << " итераций было найдено решение СЛАУ методом Гаусса-Зейделя (1):\n";
	return x;
}

long float* mOfGauss_Seidel2(long float(&A)[N][N], long float(&b)[N], long float(&x0)[N], double eps)
{
	long float * x = new long float[N];
	long float * xPrevious = new long float[N];
	int countOfIter = 0;

	// Начальное приближение 
	for (int i = 0; i < N; i++)
	{
		xPrevious[i] = x0[i];
		x[i] = x0[i];
	}

	for (int i = 0; i < N; i++)
	{
		// Мы должны получить произведение всех элементов в i строке матрицы и вектора x, кроме i элементов.
		x[i] = b[i] - mulVectorByVector(N, A[i], N, x) + A[i][i] * x[i];
		x[i] /= A[i][i];
	}

	bool checkEps = true;
	for (int i = 0; i < N; i++)
		if (abs(x[i] - xPrevious[i]) > eps)
		{
			checkEps = false;
			break;
		}
	countOfIter++;

	while (!checkEps && countOfIter < 50)
	{
		for (int i = 0; i < N; i++)
			xPrevious[i] = x[i];

		for (int i = 0; i < N; i++)
		{
			x[i] = b[i] - mulVectorByVector(N, A[i], N, x) + A[i][i] * x[i];
			x[i] /= A[i][i];
		}

		checkEps = true;
		for (int i = 0; i < N; i++)
			if (abs(x[i] - xPrevious[i]) > eps)
			{
				checkEps = false;
				break;
			}
		countOfIter++;
	}
	if (countOfIter == 50)
	{
		cout << "Выберите другое начальное приближение. В качестве ответа, в данном случае, mOfGauss_Seidel2 вернул заданный Вами вектор x.\n";
		for (int i = 0; i < N; i++)
			x[i] = x0[i];
	}
	else
		cout << "За " << countOfIter << " итераций было найдено решение СЛАУ методом Гаусса-Зейделя (2): \n";
	return x;
}
