#pragma once
#include <cstring>

#include "CoreEngine/Math/Vector/Vector2.h"
#include "CoreEngine/Math/Vector/Vector3.h"

namespace Eclipse::Math
{
    template <class T>
    class Matrix3x3
    {
    public:
        Matrix3x3();
        ~Matrix3x3() = default;

        Matrix3x3(Matrix3x3<T>& aOther);
        void operator=(const Matrix3x3<T>& aOther);
        void operator*=(const Matrix3x3<T>& aMat0);

        T& operator()(int column, int row);
        const T& operator()(int column, int row) const;

        static Matrix3x3<T> CreateTranslation(const Vector2<T>& aTranslation);
        static Matrix3x3<T> CreateRotation(float aRotation);
        static Matrix3x3<T> CreateScale(const Vector2<T>& aScale);

        Vector2<T> GetTranslation();

        void Transpose();
        Matrix3x3<T> GetTranspose();
        
        void Inverse();
        Matrix3x3<T> GetInverse();

        union
        {
            T arrData[3][3];
            T data[9];
        };

    };

    typedef Matrix3x3<float> Matrix3x3f;
    typedef Matrix3x3<double> Matrix3x3d;
    typedef Matrix3x3<unsigned> Matrix3x3ui;
    typedef Matrix3x3<int> Matrix3x3i;

    typedef Matrix3x3f Mat3x3f;
    typedef Matrix3x3d Mat3x3d;
    typedef Matrix3x3ui Mat3x3ui;
    typedef Matrix3x3i Mat3x3i;
}

#include "Matrix3x3.inl"