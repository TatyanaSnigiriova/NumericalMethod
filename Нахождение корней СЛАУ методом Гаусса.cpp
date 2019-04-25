// Нахождение корней СЛАУ методом Гаусса
// Подробное описание того, что вообще происходит, смотрите на https://www.youtube.com/watch?v=myreyS4PesA&t=2852s, https://www.youtube.com/watch?v=ME1ihEMIB70
// Огромная благодарность Бояршинову Борису Сергеевичу и Фоминой Любови Николаевне за их преподавательский труд
// https://vk.com/snigiriovakoles

#include "stdafx.h"
#include "iostream"
#include "locale.h"

const int N = 3;
// Исходная матрица
long float MatrixA[N][N] =
{
	{ 1, 2, 3 },
	{ 1, 1, 1},
	{2, 1, 4 }
};

long float f[N] = { 14, 6, 16 };

int checkMatrix();

template <int N>
long float* mOfGauss(long float (&A)[N][N], long float (&b)[N]);

template <int N>
long float** FindMatrixA_1(long float (&A)[N][N]);

template <int nb>
long float* mulMatrixByVector(int nA, int mA, long float **A, long float(&b)[nb]);

template <int N>
long float* mOfGauss_Jordan(long float (&A)[N][N], long float (&b)[N]);

using namespace std;


int main()
{
	setlocale(LC_ALL, "rus");
	cout << "	Эта программа реализует алгоритмы нахождения корней системы линейных алгебраических уравнений методом Гаусса.\n";
	cout << "Первый алгоритм основан на классическом методе Гаусса, второй является его модификацией - Метод Гаусса—Жордана - \n";
	cout << "Через нахождение матрицы, обратной к данной.\n\n";
	cout << "Для работы с другими СЛАУ, задайте количество неизвестных N, к-ты системы в матрицу 'MatrixA' и результирующий вектор f перед функцией 'main'.\n\n";

	if (checkMatrix())
	{
		long float* x1 = mOfGauss(MatrixA, f);
		long float* x2 = mOfGauss_Jordan(MatrixA, f);
		cout << "Корни системы, найденные с помощью метода Гаусса:\n";
		for (int i = 0; i < N; i++)
			cout << "x"<< i << " = " << x1[i] << "   ";
		cout << endl << endl;
		cout << "Корни системы, найденные с помощью метода Жордана-Гаусса:\n";
		for (int i = 0; i < N; i++)
			cout << "x" << i << " = " << x2[i] << "   ";
		cout << endl;
		delete[] x1;

	}
	return 0;
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

template <int N>
long float* mOfGauss(long float(&A)[N][N], long float (&b)[N])
{
	long float** UTMatrixA = new long float*[N]; // Верхнетреугольная матрица 
	long float* fForUTMatrixA = new long float[N];

	for (int i = 0; i < N; i++)
	{
		UTMatrixA[i] = new long float[N];
		for (int j = 0; j < N; j++)
			UTMatrixA[i][j] = A[i][j];
		fForUTMatrixA[i] = f[i];
	}

	long float* x = new long float[N];

	/*
	I этап метода Гаусса(прямой ход метода)
	Преобразование исходной матрицы к верхнетреугольному виду, используя пошаговое исключение переменных из системы.
	*/

	long float firstRate;
	for (int step = 0; step < N; step++)
	{
		firstRate = UTMatrixA[step][step];
		for (int j = step; j < N; j++)
			UTMatrixA[step][j] /= firstRate;
		fForUTMatrixA[step] /= firstRate;

		for (int i = step + 1; i < N; i++)
		{
			firstRate = UTMatrixA[i][step];
			for (int j = step; j < N; j++)
				UTMatrixA[i][j] -= UTMatrixA[step][j] * firstRate;
			fForUTMatrixA[i] -= firstRate * fForUTMatrixA[step];
		}
	}

	/*
	II этап метода Гаусса(обратный ход метода)
	Выражаем x, начиная с последнего уравнения системы.
	*/

	x[N - 1] = fForUTMatrixA[N - 1];
	for (int i = N - 2; i >= 0; i--)
	{
		x[i] = fForUTMatrixA[i];
		for (int j = i + 1; j < N; j++)
			x[i] -= UTMatrixA[i][j] * x[j];
	}

	return x;
}

template <int N>
long float** FindMatrixA_1(long float (&A)[N][N])
{
	long float** E = new long float*[N]; // Исходная матрица A, приведенная к единичной матрице;  
	long float** matrixA_1 = new long float*[N]; // Единичная матрица, приведённая к обратной матрице матрицы A;
	
	for (int i = 0; i < N; i++)
	{
		E[i] = new long float[N];
		matrixA_1[i] = new long float[N];
		for (int j = 0; j < N; j++)
		{
			E[i][j] = A[i][j];
			matrixA_1[i][j] = 0;
		}
		matrixA_1[i][i] = 1;
	}
	long float firstRate;

	//  
	// I этап метода Гаусса(прямой ход метода)
	// Преобразование исходной матрицы (в данном случае матрицы E) к верхнетреугольному виду, используя пошаговое исключение переменных из системы.
	//

	for (int step = 0; step < N; step++)
	{
		firstRate = E[step][step];
		for (int j = 0; j < step; j++)
			matrixA_1[step][j] /= firstRate;
		for (int j = step; j < N; j++)
		{
			E[step][j] /= firstRate;
			matrixA_1[step][j] /= firstRate;
		}

		for (int i = step + 1; i < N; i++)
		{
			firstRate = E[i][step];
			for (int j = 0; j < step; j++)
				matrixA_1[i][j] -= matrixA_1[step][j] * firstRate;
			for (int j = step; j < N; j++)
			{
				E[i][j] -= E[step][j] * firstRate;
				matrixA_1[i][j] -= matrixA_1[step][j] * firstRate;
			}
		}
	}
	
	//
	// II этап метода Гаусса(обратный ход метода)
	// Преобразовываем верхнетреугольную промежуточную матрицу (в данном случае матрицу E) к диагональному виду по тому же алгоритму,
	// как и в пункте 1, начиная с последнего уравнения системы. 
	//
	
	for (int step = N - 1; step > 0; step--)
	{
		for (int i = step - 1; i >= 0; i--)
		{
			firstRate = E[i][step];
			for (int j = 0; j < step; j++)
				matrixA_1[i][j] -= matrixA_1[step][j] * firstRate;
			for (int j = step; j < N; j++)
			{
				E[i][j] -= E[step][j] * firstRate;
				matrixA_1[i][j] -= matrixA_1[step][j] * firstRate;
			}
		}
	}

	return matrixA_1;
}

template <int nb>
long float* mulMatrixByVector(int nA, int mA, long float **A, long float (&b)[nb])
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
	else cout << "Количество элементов в строке матрицы должно совпадать с размером вектора!\n\n";
	return c;
}

template <int N>
long float* mOfGauss_Jordan(long float(&A)[N][N], long float (&b)[N])
{
	return mulMatrixByVector(N, N, FindMatrixA_1(A), b);
}
