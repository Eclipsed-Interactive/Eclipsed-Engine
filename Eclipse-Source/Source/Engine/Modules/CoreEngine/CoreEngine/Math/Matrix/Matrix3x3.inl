#include "Matrix3x3.h"

#include <assert.h>

#include <string>

namespace Eclipse::Math
{
#pragma region operations

	template<class T>
	Matrix3x3<T>::Matrix3x3()
	{
		memset(data, 0, sizeof(Matrix3x3<T>));
		arrData[0][0] = 1;
		arrData[1][1] = 1;
		arrData[2][2] = 1;
	}
	template<class T>
	Matrix3x3<T>::Matrix3x3(Matrix3x3<T>& aOther)
	{
		memcpy(data, aOther.data, sizeof(Matrix3x3<T>));
	}

#pragma endregion


#pragma region operations

	template<class T>
	Matrix3x3<T> operator*(const Matrix3x3<T>& aMat0, const Matrix3x3<T>& aMat1)
	{
		Matrix3x3<T> returnMatrix;

		for (int row = 1; row <= 3; row++)
		{
			for (int col = 1; col <= 3; col++)
			{
				float total = 0;
				for (int i = 1; i <= 3; i++)
				{
					float num0 = aMat0(row, i);
					float num1 = aMat1(i, col);
					total += num0 * num1;
				}
				returnMatrix(row, col) = total;
			}
		}

		return returnMatrix;
	}
	template<class T>
	Vector3<T> operator*(const Vector3<T>& aVec0, const Matrix3x3<T>& aMat0)
	{
		Vector3<T> returnVector;

		for (int col = 1; col <= 3; ++col)
		{
			returnVector.data[col - 1] = 0;
			for (int row = 1; row <= 3; ++row)
			{
				returnVector.data[col - 1] += aMat0(row, col) * aVec0.data[row - 1];
			}
		}

		return returnVector;
	}

	template<class T>
	void Matrix3x3<T>::operator*=(const Matrix3x3<T>& aMat0)
	{
		Matrix3x3<T> returnMatrix;
		for (int row = 1; row <= 3; row++)
		{
			for (int col = 1; col <= 3; col++)
			{
				float total = 0;
				for (int i = 1; i <= 3; i++)
				{
					float num0 = aMat0(row, i);
					float num1 = (*this)(i, col);
					total += num0 * num1;
				}

				returnMatrix(row, col) = total;
			}
		}
		(*this) = returnMatrix;
	}

	template<class T>
	void Matrix3x3<T>::operator=(const Matrix3x3<T>& aOther)
	{
		memcpy(data, aOther.data, sizeof(Matrix3x3<T>));
	}

#pragma endregion


#pragma region Other

	template<class T>
	T& Matrix3x3<T>::operator()(int column, int row)
	{
		assert(column > 0 && row > 0 && column <= 3 && row <= 3 && "Index out of range");

		return arrData[column - 1][row - 1];
	}

	template<class T>
	const T& Matrix3x3<T>::operator()(int column, int row) const
	{
		assert(column > 0 && row > 0 && column <= 3 && row <= 3 && "Index out of range");

		return arrData[column - 1][row - 1];
	}

#pragma endregion

	template<class T>
	Matrix3x3<T> Matrix3x3<T>::CreateTranslation(const Vector2<T>& aTranslation)
	{
		Matrix3x3<T> mat;

		mat(3, 1) = aTranslation.x;
		mat(3, 2) = aTranslation.y;

		return mat;
	}
	template<class T>
	Matrix3x3<T> Matrix3x3<T>::CreateRotation(float aRotation)
	{
		Matrix3x3<T> mat;

		mat(1, 1) = cos(aRotation);
		mat(1, 2) = -sin(aRotation);
		mat(2, 1) = sin(aRotation);
		mat(2, 2) = cos(aRotation);

		return mat;
	}
	template<class T>
	Matrix3x3<T> Matrix3x3<T>::CreateScale(const Vector2<T>& aScale)
	{
		Matrix3x3<T> mat;

		mat(1, 1) = aScale.x;
		mat(2, 2) = aScale.y;

		return mat;
	}

	template<class T>
	Vector2<T> Matrix3x3<T>::GetTranslation()
	{
		Vector2<T> position;

		position.x = (*this)(3, 1);
		position.y = (*this)(3, 2);

		return position;
	}

	template <class T>
	void Matrix3x3<T>::Transpose()
	{
		std::swap(arrData[0][1], arrData[1][0]);
		std::swap(arrData[0][2], arrData[2][0]);
		std::swap(arrData[1][2], arrData[2][1]);
	}
	template <class T>
	Matrix3x3<T> Matrix3x3<T>::GetTranspose()
	{
		Matrix3x3<T> mat(*this);
		std::swap(mat.arrData[0][1], mat.arrData[1][0]);
		std::swap(mat.arrData[0][2], mat.arrData[2][0]);
		std::swap(mat.arrData[1][2], mat.arrData[2][1]);
		return mat;
	}

	template <class T>
	void Matrix3x3<T>::Inverse()
	{
		std::swap(arrData[0][1], arrData[1][0]);

		arrData[2][0] *= -1;
		arrData[2][1] *= -1;
	}
	template <class T>
	Matrix3x3<T> Matrix3x3<T>::GetInverse()
	{
		Matrix3x3<T> mat(*this);

		std::swap(mat.arrData[0][1], mat.arrData[1][0]);

		mat.arrData[2][0] *= -1;
		mat.arrData[2][1] *= -1;

		return mat;
	}
}