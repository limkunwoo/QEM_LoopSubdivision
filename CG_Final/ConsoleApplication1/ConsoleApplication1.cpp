// ConsoleApplication1.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include "Matrix.h"
using namespace std;

int main()
{
	Matrix<double> A(3, 3);
	A[0][0] = 2;
	A[0][1] = 6;
	A[0][2] = 2;
	A[1][0] = -3;
	A[1][1] = -8;
	A[1][2] = 0;
	A[2][0] = 4;
	A[2][1] = 9;
	A[2][2] = 2;

	cout << "The Matrix is..." << endl;
	A.Print();

	cout << "\nTransPose..." << endl;
	A.GetTransPose().Print();

	cout << endl;
	Matrix<double> L(3, 3), U(3, 3);
	if (A.LUDecomposition(L, U)) {
		cout << "L" << endl;
		L.Print();
		cout << endl << "U" << endl;
		U.Print();
	}

	Matrix<double> x;
	Matrix<double> b(3, 1);
	b[0][0] = 2;
	b[1][0] = 2;
	b[2][0] = 3;
	A.Solution(A, x, b);
	cout << endl << "해" << endl;
	x.Print();


}