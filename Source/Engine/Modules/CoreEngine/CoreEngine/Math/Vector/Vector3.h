#pragma once

#include <initializer_list>
#include <typeindex>

namespace Eclipse::Math
{
	template <typename T>
	class Vector3 final
	{
	public:
		Vector3();
		Vector3(const T& aX, const T& aY, const T& aZ);
		Vector3(const std::initializer_list<T> initList);
		~Vector3() = default;

	public:
		std::type_index GetType() const;
		static constexpr bool serializable = true;

	public:
		const Vector3 operator+(const Vector3& anotherVec) const;
		const Vector3& operator+=(const Vector3& anotherVec);

		const Vector3 operator-(const Vector3& anotherVec) const;
		const Vector3& operator-=(const Vector3& anotherVec);

		const Vector3 operator*(const Vector3& anotherVec) const;
		const Vector3& operator*=(const Vector3& anotherVec);

		Vector3 operator=(const Vector3& anotherVec);
		Vector3 operator==(const Vector3& anotherVec);

		friend Vector3 operator*(const Vector3& aVec, const T& aScalar)
		{
			return Vector3(aVec.x * aScalar, aVec.y * aScalar, aVec.z * aScalar);
		}

		friend Vector3 operator*(const T& aScalar, const Vector3& aVec)
		{
			return Vector3(aVec.x * aScalar, aVec.y * aScalar, aVec.z * aScalar);
		}

		friend Vector3 operator/(const Vector3& aVec, const T& aScalar)
		{
			return Vector3(aVec.x / aScalar, aVec.y / aScalar, aVec.z / aScalar);
		}

		friend Vector3 operator/(const T& aScalar, const Vector3& aVec)
		{
			return Vector3(aScalar / aVec.x, aScalar / aVec.y, aScalar / aVec.z);
		}

	public:
		T Dot(const Vector3& anotherVec) const;
		static T Dot(const Vector3& aVec0, const Vector3& aVec1);

		Vector3 Cross(const Vector3& anotherVec) const;
		static Vector3 Cross(const Vector3& aVec0, const Vector3& aVec1);

		T Distance(const Vector3& anotherVec) const;
		static T Distance(const Vector3& aVec0, const Vector3& aVec1);

		T DistanceSqrd(const Vector3& anotherVec) const;
		static T DistanceSqrd(const Vector3& aVec0, const Vector3& aVec1);

		T Length() const;
		T LengthSqrd() const;

	public:
		Vector3<T> Normalized() const;
		void Normalize();

		T AngleBetween(const Vector3& anotherVec) const;
		static T AngleBetween(const Vector3& aVec0, const Vector3& aVec1);

	public:
		union
		{
			struct { T x, y, z; };
			struct { T X, Y, Z; };
			struct { T myX, myY, myZ; };

			T data[3];
			T myData[3];
		};
	};

	typedef Vector3<float> Vector3f;
	typedef Vector3<double> Vector3d;
	typedef Vector3<unsigned> Vector3ui;
	typedef Vector3<int> Vector3i;
}

#include "Vector3.inl"