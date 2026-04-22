#include "Matrix2x2.h"

#include <assert.h>

namespace Eclipse::Math
{
#pragma region operations

	template<class T>
	Matrix2x2<T>::Matrix2x2()
	{
		memset(data, 0, sizeof(Matrix2x2<T>));
		arrData[0][0] = 1;
		arrData[1][1] = 1;
	}
	template<class T>
	Matrix2x2<T>::Matrix2x2(Matrix2x2<T>& aOther)
	{
		memcpy(data, aOther, sizeof(Matrix2x2<T>));
	}

#pragma endregion


#pragma region operations

	template<class T>
	Matrix2x2<T> operator*(const Matrix2x2<T>& aMat0, const Matrix2x2<T>& aMat1)
	{
		Matrix2x2<T> returnMatrix;

		for (int row = 1; row <= 2; row++)
		{
			for (int col = 1; col <= 2; col++)
			{
				float& valToAddMatrixNum = returnMatrix(row, col);
				for (int i = 1; i <= 2; i++)
				{
					float num0 = aMat0(row, i);
					float num1 = aMat1(i, col);
					valToAddMatrixNum += num0 * num1;
				}
			}
		}

		return returnMatrix;
	}
	template<class T>
	Vector2<T> operator*(const Vector2<T>& aVec0, const Matrix2x2<T>& aMat0)
	{
		Vector2<T> returnVector;

		for (int col = 1; col <= 2; col++)
		{
			float& returnVectorVal = returnVector[col];
			for (int row = 1; row <= 2; row++)
			{
				float num0 = aMat0(row, col);
				float num1 = aVec0[row - 1];
				returnVectorVal += num0 * num1;
			}
		}

		return returnVector;
	}

	template<class T>
	void Matrix2x2<T>::operator*=(const Matrix2x2<T>& aMat0)
	{
		Matrix2x2<T> returnMatrix;
		for (int row = 1; row <= 2; row++)
		{
			for (int col = 1; col <= 2; col++)
			{
				float& valToAddMatrixNum = returnMatrix(row, col);
				for (int i = 1; i <= 2; i++)
				{
					float num0 = aMat0(row, i);
					float num1 = (*this)(i, col);
					valToAddMatrixNum += num0 * num1;
				}
			}
		}
		(*this) = returnMatrix;
	}

	template<class T>
	void Matrix2x2<T>::operator=(const Matrix2x2<T>& aOther)
	{
		memcpy(data, aOther, sizeof(Matrix2x2<T>));
	}

#pragma endregion


#pragma region Other

	template<class T>
	T& Matrix2x2<T>::operator()(int row, int column)
	{
		assert(column > 0 && row > 0 && column <= 2 && row <= 2 && "Index out of range");

		return arrData[row - 1][column - 1];
	}

#pragma endregion

}