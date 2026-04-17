#include "Vector4.h"

#include <cmath>

namespace Eclipse::Math
{
    template <typename T>
    inline Vector4<T>::Vector4() : x(T()), y(T()), z(T()), w(T())
    {
        // TODO: ERROR CHECK IF ITS NOT A NUMBER
    }

    template <typename T>
    inline Vector4<T>::Vector4(const T& aX, const T& aY, const T& aZ, const T& aW) : x(aX), y(aY), z(aZ), w(aW)
    {
        // TODO: ERROR CHECK IF ITS NOT A NUMBER
    }

    template <typename T>
    inline Vector4<T>::Vector4(const std::initializer_list<T> initList)
    {
        memcpy(data, initList.begin(), sizeof(T) * 4);
    }

    template <typename T>
    inline std::type_index Vector4<T>::GetType() const
    {
        return typeid(T);
    }

    template <typename T>
    inline T Vector4<T>::Dot(const Vector4& anotherVec) const
    {
        return x * anotherVec.x + y * anotherVec.y + z * anotherVec.z + w * anotherVec.w;
    }

    template <typename T>
    inline T Vector4<T>::Dot(const Vector4& aVec0, const Vector4& aVec1)
    {
        return aVec0.Dot(aVec1);
    }

    template <typename T>
    inline T Vector4<T>::Distance(const Vector4& anotherVec) const
    {
        return std::sqrt(DistanceSqrd(anotherVec));
    }

    template <typename T>
    inline T Vector4<T>::Distance(const Vector4& aVec0, const Vector4& aVec1)
    {
        return aVec0.Distance(aVec1);
    }

    template <typename T>
    inline T Vector4<T>::DistanceSqrd(const Vector4& anotherVec) const
    {
        T dx = (x - anotherVec.x);
        T dy = (y - anotherVec.y);
        T dz = (z - anotherVec.z);
        T dw = (w - anotherVec.w);

        return dx * dx + dy * dy + dz * dz + dw * dw;
    }

    template <typename T>
    inline T Vector4<T>::DistanceSqrd(const Vector4& aVec0, const Vector4& aVec1)
    {
        return aVec0.DistanceSqrd(aVec1);
    }

    template <typename T>
    inline T Vector4<T>::Length() const
    {
        return std::sqrt(LengthSqrd());
    }

    template <typename T>
    inline T Vector4<T>::LengthSqrd() const
    {
        return x * x + y * y + z * z + w * w;
    }

    template <typename T>
    inline Vector4<T> Vector4<T>::Normalized() const
    {
        T length = Length();

        if (!length) *this;

        return Vector4(x / length, y / length, z / length, w / length);
    }

    template <typename T>
    inline void Vector4<T>::Normalize()
    {
        T length = Length();

        if (!length) return;

        x /= length;
        y /= length;
        z /= length;
        w /= length;
    }

#pragma region ===== OPERATORS =====

    template <typename T>
    inline const Vector4<T> Vector4<T>::operator+(const Vector4& anotherVec) const
    {
        return Vector4(x + anotherVec.x, y + anotherVec.y, z + anotherVec.z, w + anotherVec.w);
    }

    template <typename T>
    inline const Vector4<T>& Vector4<T>::operator+=(const Vector4& anotherVec)
    {
        x += anotherVec.x;
        y += anotherVec.y;
        z += anotherVec.z;
        w += anotherVec.w;
        return *this;
    }

    template <typename T>
    inline const Vector4<T> Vector4<T>::operator-(const Vector4& anotherVec) const
    {
        return Vector4(x - anotherVec.x, y - anotherVec.y, z - anotherVec.z, w - anotherVec.w);
    }

    template <typename T>
    inline const Vector4<T>& Vector4<T>::operator-=(const Vector4& anotherVec)
    {
        x -= anotherVec.x;
        y -= anotherVec.y;
        z -= anotherVec.z;
        w -= anotherVec.w;
        return *this;
    }

    template <typename T>
    inline const Vector4<T> Vector4<T>::operator*(const Vector4& anotherVec) const
    {
        return Vector4(x * anotherVec.x, y * anotherVec.y, z * anotherVec.z, w * anotherVec.w);
    }

    template <typename T>
    inline const Vector4<T>& Vector4<T>::operator*=(const Vector4& anotherVec)
    {
        x *= anotherVec.x;
        y *= anotherVec.y;
        z *= anotherVec.z;
        w *= anotherVec.w;
        return *this;
    }

    template <typename T>
    inline Vector4<T> Vector4<T>::operator=(const Vector4& anotherVec)
    {
        if (this != &anotherVec)
        {
            x = anotherVec.x;
            y = anotherVec.y;
            z = anotherVec.z;
            w = anotherVec.w;
        }
        return *this;
    }

    template <typename T>
    inline Vector4<T> Vector4<T>::operator==(const Vector4& anotherVec)
    {
        return (x == anotherVec.x && y == anotherVec.y && z == anotherVec.z && w == anotherVec.w);
    }

#pragma endregion

}