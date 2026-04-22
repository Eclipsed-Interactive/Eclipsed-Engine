#pragma once

#include <initializer_list>
#include <typeindex>

namespace Eclipse::Math
{
	template <typename T>
	class Vector4 final
	{
	public:
		Vector4();
		Vector4(const T& aX, const T& aY, const T& aZ, const T& aW);
		Vector4(const std::initializer_list<T> initList);
		~Vector4() = default;

	public:
		std::type_index GetType() const;
		static constexpr bool serializable = true;

	public:
		const Vector4 operator+(const Vector4& anotherVec) const;
		const Vector4& operator+=(const Vector4& anotherVec);

		const Vector4 operator-(const Vector4& anotherVec) const;
		const Vector4& operator-=(const Vector4& anotherVec);

		const Vector4 operator*(const Vector4& anotherVec) const;
		const Vector4& operator*=(const Vector4& anotherVec);

		Vector4 operator=(const Vector4& anotherVec);
		Vector4 operator==(const Vector4& anotherVec);

		friend Vector4 operator*(const Vector4& aVec, const T& aScalar)
		{
			return Vector4(aVec.x * aScalar, aVec.y * aScalar, aVec.z * aScalar, aVec.w * aScalar);
		}

		friend Vector4 operator*(const T& aScalar, const Vector4& aVec)
		{
			return Vector4(aVec.x * aScalar, aVec.y * aScalar, aVec.z * aScalar, aVec.w * aScalar);
		}

		friend Vector4 operator/(const Vector4& aVec, const T& aScalar)
		{
			return Vector4(aVec.x / aScalar, aVec.y / aScalar, aVec.z / aScalar, aVec.w / aScalar);
		}

		friend Vector4 operator/(const T& aScalar, const Vector4& aVec)
		{
			return Vector4(aScalar / aVec.x, aScalar / aVec.y, aScalar / aVec.z, aScalar / aVec.w);
		}

	public:
		T Dot(const Vector4& anotherVec) const;
		static T Dot(const Vector4& aVec0, const Vector4& aVec1);

		T Distance(const Vector4& anotherVec) const;
		static T Distance(const Vector4& aVec0, const Vector4& aVec1);

		T DistanceSqrd(const Vector4& anotherVec) const;
		static T DistanceSqrd(const Vector4& aVec0, const Vector4& aVec1);

		T Length() const;
		T LengthSqrd() const;

	public:
		Vector4<T> Normalized() const;
		void Normalize();

	public:
		union
		{
			struct { T x, y, z, w; };
			struct { T X, Y, Z, W; };
			struct { T myX, myY, myZ, myW; };

			T data[4];
			T myData[4];
		};
	};

	typedef Vector4<float> Vector4f;
	typedef Vector4<double> Vector4d;
	typedef Vector4<unsigned> Vector4ui;
	typedef Vector4<int> Vector4i;
}

#include "Vector4.inl"