#pragma once
#include <iostream>
using namespace std;

//matrix에서 한
template <class T>
class Row
{
public:
	T* c;
	int nCount;
public:
	Row<T>() : Row<T>(1) { }
	Row<T>(int n) {
		c = new T[n];
		nCount = n;
		for (int i = 0; i < n; i++) {
			c[i] = 0;
		}
	}
	Row<T>(const Row<T>& column) {
		c = new T[column.nCount];
		nCount = column.nCount;
		for (int i = 0; i < nCount; i++) {
			c[i] = column.c[i];
		}
	}
	~Row<T>() {
		delete[] c;
	}
	void Alloc(int n) {
		delete[] c;
		c = new T[n];
		nCount = n;
		for (int i = 0; i < n; i++) {
			c[i] = 0;
		}
	}
public:
	T& operator[] (int index) {
		return c[index];
	}
	void operator= (Row<T> row) {
		delete[] c;
		c = new T[row.nCount];
		nCount = row.nCount;
		for (int i = 0; i < nCount; i++) {
			c[i] = row[i];
		}
	}
	Row<T> operator* (T scalar) {
		Row<T> result(nCount);
		for (int i = 0; i < nCount; i++) {
			result[i] = c[i] * scalar;
		}
		return result;
	}
	Row<T> operator/ (T scalar) {
		Row<T> result(nCount);
		for (int i = 0; i < nCount; i++) {
			result[i] = c[i] / scalar;
		}
		return result;
	}
	Row<T> operator+ (Row<T> row) {
		Row<T> result(nCount);
		for (int i = 0; i < nCount; i++) {
			result[i] = c[i] + row[i];
		}
		return result;
	}
};
template <class T>
class Matrix
{
private:
	Row<T>* row;
private:
	int nRow, nColumn;
public:
	Matrix<T>() : Matrix<T>(1) {}
	Matrix<T>(int r) : Matrix<T>(r, r) { }
	Matrix<T>(int r, int c) {
		row = new Row<T>[r];
		for (int i = 0; i < r; i++) {
			row[i].Alloc(c);
		}
		nRow = r;
		nColumn = c;
	}

	Matrix<T>(const Matrix<T>& m) : Matrix<T>(m.nRow, m.nColumn) {
		for (int i = 0; i < nRow; i++) {
			for (int j = 0; j < nColumn; j++) {
				row[i].c[j] = m.row[i].c[j];
			}
		}
	}
	~Matrix() {
		delete[] row;
	}
public:
	void operator=(Matrix<T> m) {
		delete[] row;
		nRow = m.nRow;
		nColumn = m.nColumn;
		row = new Row<T>[m.nRow];
		for (int i = 0; i < m.nRow; i++) {
			row[i].Alloc(nColumn);
		}
		for (int i = 0; i < nRow; i++) {
			for (int j = 0; j < nColumn; j++) {
				(*this)[i][j] = m[i][j];
			}
		}
	}
	Row<T>& operator[](int index) {
		return row[index];
	}
	Matrix<T> operator* (Matrix<T> m) {
		if (nColumn != m.nRow)
		{
			printf("정의되지 않은 연산입니다.");
			return GetIdentity(nRow);
		}
		return Mul(*this, m);
	}
	Matrix<T> operator/ (T scalar) {
		Matrix<T> result(nRow, nColumn);
		for (int i = 0; i < nRow; i++) {
			for (int j = 0; j < nColumn; j++) {
				result[i][j] = (*this)[i][j] / scalar;
			}
		}
		return result;
	}
	Matrix<T> operator+ (Matrix<T> m) {
		if (nRow != m.nRow || nColumn != m.nColumn)
		{
			printf("정의되지 않은 연산입니다.");
			return GetIdentity(nRow);
		}
		Matrix<T> result(nRow, nColumn);
		for (int i = 0; i < nRow; i++) {
			for (int j = 0; j < nColumn; j++) {
				result[i][j] = (*this)[i][j] + m[i][j];
			}
		}
		return result;
	}
	Matrix<T> operator- (Matrix<T> m) {
		if (nRow != m.nRow || nColumn != m.nColumn)
		{
			printf("정의되지 않은 연산입니다.");
			return GetIdentity(nRow);
		}
		Matrix<T> result(nRow, nColumn);
		for (int i = 0; i < nRow; i++) {
			for (int j = 0; j < nColumn; j++) {
				result[i][j] = (*this)[i][j] - m[i][j];
			}
		}
		return result;
	}
public:
	Matrix<T> Mul(Matrix<T> m1, Matrix<T> m2) {
		if (m1.nColumn != m2.nRow) {
			cout << "정의되지 않은 연산입니다" << endl;
		}
		Matrix<T> result(m1.nRow, m2.nColumn);
		for (int i = 0; i < m1.nRow; i++) {
			for (int j = 0; j < m2.nColumn; j++) {
				for (int k = 0; k < m1.nColumn; k++) {
					result[i][j] += m1[i][k] * m2[k][j];
				}
			}
		}
		return result;
	}
	void Print() {
		for (int i = 0; i < nRow; i++)
		{
			for (int j = 0; j < nColumn; j++) {
				printf("%f ", (*this)[i][j]);
			}
			putchar('\n');
		}
	}
	bool LUDecomposition(Matrix<T>& L, Matrix<T>& U) {
		if (nRow != nColumn) {
			cout << "정방행렬이 아닙니다." << endl;
			return false;
		}
		L = Matrix(nRow);
		U = Matrix(nRow, nColumn);
		Copy(U, (*this));
		for (int i = 1; i <= nRow; i++) {
			L[i - 1][i - 1] = U[i - 1][i - 1];
			U[i - 1] = U[i - 1] / U[i - 1][i - 1];
			for (int j = i; j < nRow; j++) {
				L[j][i - 1] = U[j][i - 1];
				U[j] = U[j] + (U[i - 1] * -U[j][i - 1]);
			}
			if (L[i - 1][i - 1] == 0 || U[i - 1][i - 1] == 0) {
				return false;
			}
		}
		return true;
	}
public:
	static void Copy(Matrix<T>& destination, Matrix<T>& source) {
		for (int i = 0; i < source.nRow; i++) {
			for (int j = 0; j < source.nColumn; j++) {
				destination[i][j] = source[i][j];
			}
		}
	}

	//미완
	static Matrix<T> GetInverse() {
		if (nRow != nColumn) {
			cout << "정방행렬이 아닙니다.";
			return (*this);
		}
		Matrix<T> Inversed = GetIdentity(nRow);
	}
	Matrix<T> GetTransPose() {
		Matrix<T> result(nColumn, nRow);
		for (int i = 0; i < result.nRow; i++) {
			for (int j = 0; j < result.nColumn; j++) {
				result[i][j] = (*this)[j][i];
			}
		}
		return result;
	}
	static Matrix<T> GetIdentity(int r) {
		Matrix<T> identity(r, r);
		for (int i = 0; i < r; i++) {
			identity[i][i] = 1;
		}
		return identity;
	}
	static bool Solution(Matrix<T> A, Matrix<T>& x, Matrix<T> b) {
		x = Matrix<T>(A.nRow, b.nColumn);
		if ((A.nColumn != x.nRow) && (x.nRow != b.nRow)) {
			cout << "matrix 크기 에러" << endl;
			return false;
		}
		Matrix<T> L, U;
		if (!A.LUDecomposition(L, U)) {
			return false;
		}
		x = Matrix<T>(A.nColumn, 1);
		Matrix<T> y(x.nRow, x.nColumn);
		for (int i = 0; i < y.nRow; i++) {
			T mid = b[i][0];
			for (int j = 0; j < i; j++) {
				mid -= L[i][j] * y[j][0];
			}
			y[i][0] = mid / L[i][i];
			/*
			y[0][0] = b[0][0] / L[0][0];
			y[1][0] = (b[1][0] - (L[1][0] * y[0][0])) / L[1][1];
			y[2][0] = (b[2][0] - (L[2][0] * y[0][0]) - (L[2][1] * y[1][0])) / L[2][2];
			*/
		}
		for (int i = x.nRow - 1; i >= 0; i--) {
			T mid = y[i][0];
			for (int j = x.nRow - 1; j > i; j--) {
				mid -= U[i][j] * x[j][0];
			}
			x[i][0] = mid;
			/*
			x[2][0] = y[2][0]
			x[1][0] = (y[1][0] - U[1][2] * x[2][0])
			x[0][0] = (y[0][0] - U[0][1] * x[1][0] - U[0][2] * x[2][0]);
			*/
		}
		return true;
	}

};