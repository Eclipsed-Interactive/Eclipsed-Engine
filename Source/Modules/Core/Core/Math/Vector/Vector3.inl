#include "Vector3.h"

#include <cmath>

namespace Eclipse::Math
{
    template <typename T>
    inline Vector3<T>::Vector3() : x(T()), y(T()), z(T())
    {
        // TODO: ERROR CHECK IF ITS NOT A NUMBER
    }

    template <typename T>
    inline Vector3<T>::Vector3(const T& aX, const T& aY, const T& aZ)
        : x(aX), y(aY), z(aZ)
    {
        // TODO: ERROR CHECK IF ITS NOT A NUMBER
    }

    template <typename T>
    inline Vector3<T>::Vector3(const std::initializer_list<T> initList)
    {
        memcpy(data, initList.begin(), sizeof(T) * 3);
    }

    template <typename T>
    inline std::type_index Vector3<T>::GetType() const
    {
        return typeid(T);
    }

    template <typename T>
    inline T Vector3<T>::Dot(const Vector3& anotherVec) const
    {
        return x * anotherVec.x + y * anotherVec.y + z * anotherVec.z;
    }

    template <typename T>
    inline T Vector3<T>::Dot(const Vector3& aVec0, const Vector3& aVec1)
    {
        return aVec0.Dot(aVec1);
    }

    template <typename T>
    inline Vector3<T> Vector3<T>::Cross(const Vector3& anotherVec) const
    {
        return Vector3(
            y * anotherVec.z - z * anotherVec.y,
            z * anotherVec.x - x * anotherVec.z,
            x * anotherVec.y - y * anotherVec.x
        );
    }

    template <typename T>
    inline Vector3<T> Vector3<T>::Cross(const Vector3& aVec0, const Vector3& aVec1)
    {
        return aVec0.Cross(aVec1);
    }

    template <typename T>
    inline T Vector3<T>::Distance(const Vector3& anotherVec) const
    {
        return std::sqrt(DistanceSqrd(anotherVec));
    }

    template <typename T>
    inline T Vector3<T>::Distance(const Vector3& aVec0, const Vector3& aVec1)
    {
        return aVec0.Distance(aVec1);
    }

    template <typename T>
    inline T Vector3<T>::DistanceSqrd(const Vector3& anotherVec) const
    {
        T dx = (x - anotherVec.x);
        T dy = (y - anotherVec.y);
        T dz = (z - anotherVec.z);

        return dx * dx + dy * dy + dz * dz;
    }

    template <typename T>
    inline T Vector3<T>::DistanceSqrd(const Vector3& aVec0, const Vector3& aVec1)
    {
        return aVec0.DistanceSqrd(aVec1);
    }

    template <typename T>
    inline T Vector3<T>::Length() const
    {
        return std::sqrt(LengthSqrd());
    }

    template <typename T>
    inline T Vector3<T>::LengthSqrd() const
    {
        return x * x + y * y + z * z;
    }

    template <typename T>
    inline Vector3<T> Vector3<T>::Normalized() const
    {
        T length = Length();
        if (!length) return *this;

        return Vector3(x / length, y / length, z / length);
    }

    template <typename T>
    inline void Vector3<T>::Normalize()
    {
        T length = Length();
        if (!length) return;

        x /= length;
        y /= length;
        z /= length;
    }

    template <typename T>
    inline T Vector3<T>::AngleBetween(const Vector3& anotherVec) const
    {
        T length = Length() * anotherVec.Length();
        if (length == 0)
            return T();

        T d = Dot(anotherVec);
        return std::acos(d / length);
    }

    template <typename T>
    inline T Vector3<T>::AngleBetween(const Vector3& aVec0, const Vector3& aVec1)
    {
        return aVec0.AngleBetween(aVec1);
    }

#pragma region ===== OPERATORS =====

    template <typename T>
    inline const Vector3<T> Vector3<T>::operator+(const Vector3& anotherVec) const
    {
        return Vector3(x + anotherVec.x, y + anotherVec.y, z + anotherVec.z);
    }

    template <typename T>
    inline const Vector3<T>& Vector3<T>::operator+=(const Vector3& anotherVec)
    {
        x += anotherVec.x;
        y += anotherVec.y;
        z += anotherVec.z;
        return *this;
    }

    template <typename T>
    inline const Vector3<T> Vector3<T>::operator-(const Vector3& anotherVec) const
    {
        return Vector3(x - anotherVec.x, y - anotherVec.y, z - anotherVec.z);
    }

    template <typename T>
    inline const Vector3<T>& Vector3<T>::operator-=(const Vector3& anotherVec)
    {
        x -= anotherVec.x;
        y -= anotherVec.y;
        z -= anotherVec.z;
        return *this;
    }

    template <typename T>
    inline const Vector3<T> Vector3<T>::operator*(const Vector3& anotherVec) const
    {
        return Vector3(x * anotherVec.x, y * anotherVec.y, z * anotherVec.z);
    }

    template <typename T>
    inline const Vector3<T>& Vector3<T>::operator*=(const Vector3& anotherVec)
    {
        x *= anotherVec.x;
        y *= anotherVec.y;
        z *= anotherVec.z;
        return *this;
    }

    template <typename T>
    inline Vector3<T> Vector3<T>::operator=(const Vector3& anotherVec)
    {
        if (this != &anotherVec)
        {
            x = anotherVec.x;
            y = anotherVec.y;
            z = anotherVec.z;
        }
        return *this;
    }

    template <typename T>
    inline Vector3<T> Vector3<T>::operator==(const Vector3& anotherVec)
    {
        return (x == anotherVec.x && y == anotherVec.y && z == anotherVec.z);
    }

#pragma endregion

}