#include "Vector2.h"

#include <cmath>

namespace Eclipse::Math
{
	template <typename T>
	inline Vector2<T>::Vector2() : x(T()), y(T())
	{
		// TODO: ERROR CHECK IF ITS NOT A NUMBER
	}

	template <typename T>
	inline Vector2<T>::Vector2(const T &aX, const T &aY) : x(aX), y(aY)
	{
		// TODO: ERROR CHECK IF ITS NOT A NUMBER
	}

	template <typename T>
	inline Vector2<T>::Vector2(const std::initializer_list<T> initList)
	{
		memcpy(data, initList.begin(), sizeof(T) * 2);
	}

	template <typename T>
	inline std::type_index Vector2<T>::GetType() const
	{
		return typeid(T);
	}

	template <typename T>
	inline T Vector2<T>::Dot(const Vector2 &anotherVec) const
	{
		return x * anotherVec.x + y * anotherVec.y;
	}

	template <typename T>
	inline T Vector2<T>::Dot(const Vector2 &aVec0, const Vector2 &aVec1)
	{
		return aVec0.Dot(aVec1);
	}

	template <typename T>
	inline T Vector2<T>::Distance(const Vector2 &anotherVec) const
	{
		return std::sqrt(DistanceSqrd(anotherVec));
	}

	template <typename T>
	inline T Vector2<T>::Distance(const Vector2 &aVec0, const Vector2 &aVec1)
	{
		return aVec0.Distance(aVec1);
	}

	template <typename T>
	inline T Vector2<T>::DistanceSqrd(const Vector2 &anotherVec) const
	{
		T dx = (x - anotherVec.x);
		T dy = (y - anotherVec.y);

		return dx * dx + dy * dy;
	}

	template <typename T>
	inline T Vector2<T>::DistanceSqrd(const Vector2 &aVec0, const Vector2 &aVec1)
	{
		return aVec0.DistanceSqrd(aVec1);
	}

	template <typename T>
	inline T Vector2<T>::Length() const
	{
		return std::sqrt(LengthSqrd());
	}

	template <typename T>
	inline T Vector2<T>::LengthSqrd() const
	{
		return x * x + y * y;
	}

	template <typename T>
	inline Vector2<T> Vector2<T>::Normalized() const
	{
		T length = Length();

		if (!length) *this;

		return Vector2(x / length, y / length);
	}

	template <typename T>
	inline void Vector2<T>::Normalize()
	{
		T length = Length();

		if (!length) return;

		x /= length;
		y /= length;
	}

	template <typename T>
	inline Vector2<T> Vector2<T>::Perpendicular() const
	{
		return Vector2<T>(-y, x);
	}

	template <typename T>
	inline Vector2<T> Vector2<T>::Perpendicular(const Vector2 &aVec)
	{
		return aVec.Perpendicular();
	}

	template <typename T>
	inline T Vector2<T>::AngleBetween(const Vector2 &anotherVec) const
	{
		T length = Length() * anotherVec.Length();
		if (length == 0)
			return T();

		T d = Dot(anotherVec);
		return std::acos(d / length);
	}

	template <typename T>
	inline T Vector2<T>::AngleBetween(const Vector2 &aVec0, const Vector2 &aVec1)
	{
		return aVec0.AngleBetween(aVec1);
	}

	template<typename T>
	inline rapidjson::Value Vector2<T>::Serialize(const Vector2<T>& aVec, rapidjson::Document::AllocatorType& alloc)
	{
		rapidjson::Value val(rapidjson::kObjectType);
		val.AddMember("x", aVec.x, alloc);
		val.AddMember("y", aVec.y, alloc);
		return val;
	}

	template<typename T>
	inline Vector2<T> Vector2<T>::Deserialize(const rapidjson::Value& aValue)
	{
		Vector2<T> vec;
		vec.x = aValue["x"].GetFloat();
		vec.y = aValue["y"].GetFloat();

		return vec;
	}

#pragma region ===== OPERATORS =====

	template <typename T>
	inline const Vector2<T> Vector2<T>::operator+(const Vector2 &anotherVec) const
	{
		return Vector2(x + anotherVec.x, y + anotherVec.y);
	}

	template <typename T>
	inline const Vector2<T> &Vector2<T>::operator+=(const Vector2 &anotherVec)
	{
		x += anotherVec.x;
		y += anotherVec.y;
		return *this;
	}

	template <typename T>
	inline const Vector2<T> Vector2<T>::operator-(const Vector2 &anotherVec) const
	{
		return Vector2(x - anotherVec.x, y - anotherVec.y);
	}

	template <typename T>
	inline const Vector2<T> &Vector2<T>::operator-=(const Vector2 &anotherVec)
	{
		x -= anotherVec.x;
		y -= anotherVec.y;
		return *this;
	}

	template <typename T>
	inline const Vector2<T> Vector2<T>::operator*(const Vector2 &anotherVec) const
	{
		return Vector2(x * anotherVec.x, y * anotherVec.y);
	}

	template <typename T>
	inline const Vector2<T> &Vector2<T>::operator*=(const Vector2 &anotherVec)
	{
		x *= anotherVec.x;
		y *= anotherVec.y;
		return *this;
	}

	template <typename T>
	inline Vector2<T> Vector2<T>::operator=(const Vector2 &anotherVec)
	{
		if (this != &anotherVec)
		{
			x = anotherVec.x;
			y = anotherVec.y;
		}
		return *this;
	}

	template <typename T>
	inline Vector2<T> Vector2<T>::operator==(const Vector2 &anotherVec)
	{
		return (x == anotherVec.x && y == anotherVec.y);
	}
#pragma endregion


	template<typename T>
	inline Vector2<T> operator*(const Vector2<T>& aVec, const T& aScalar)
	{
		return Vector2<T>(aVec.x * aScalar, aVec.y * aScalar);
	}

	template<typename T>
	inline Vector2<T> operator*(const T& aScalar, const Vector2<T>& aVec)
	{
		return Vector2<T>(aVec.x * aScalar, aVec.y * aScalar);
	}
}

template<typename T>
inline Eclipse::Math::Vector2<T> Vec2(const T& aScalarX, const T& aScalarY) {
	Eclipse::Math::Vector2<T> vec = { aScalarX, aScalarY };
	return vec;
}