#pragma once

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

namespace Eclipse::Math
{
#pragma region VECTOR2

	template<typename T>
	inline Vector2<T> ToVec2(const Vector3<T>& vec)
	{
		return Vector2<T>(vec.x, vec.y);
	}

	template<typename T>
	inline Vector2<T> ToVec2(const Vector4<T>& vec)
	{
		return Vector2<T>(vec.x, vec.y);
	}

	template<typename T>
	Vector2<T>::operator Vector3<T>() const
	{
		return Vector3<T>{x, y, T{ 0 }};
	}

	template<typename T>
	Vector2<T>::operator Vector4<T>() const
	{
		return Vector4<T>{x, y, T{ 0 }, T{ 0 }};
	}

#pragma endregion


#pragma region VECTOR3

	template<typename T>
	inline Vector3<T> ToVec3(const Vector2<T>& vec)
	{
		return Vector3<T>(vec.x, vec.y, T{ 0 });
	}

	template<typename T>
	inline Vector3<T> ToVec3(const Vector4<T>& vec)
	{
		return Vector3<T>(vec.x, vec.y, vec.z);
	}

	template<typename T>
	Vector3<T>::operator Vector2<T>() const
	{
		return Vector3<T>{x, y, T{ 0 }};
	}

	template<typename T>
	Vector3<T>::operator Vector4<T>() const
	{
		return Vector4<T>{x, y, z, T{ 0 }};
	}

#pragma endregion



#pragma region VECTOR4

	template<typename T>
	inline Vector4<T> ToVec4(const Vector2<T>& vec)
	{
		return Vector4<T>(vec.x, vec.y, T{ 0 }, T{ 0 });
	}

	template<typename T>
	inline Vector4<T> ToVec4(const Vector3<T>& vec)
	{
		return Vector4<T>(vec.x, vec.y, vec.z, T{ 0 });
	}

	template<typename T>
	Vector4<T>::operator Vector2<T>() const
	{
		return Vector4<T>{x, y, T{ 0 }};
	}

	template<typename T>
	Vector4<T>::operator Vector3<T>() const
	{
		return Vector4<T>{x, y, z, T{ 0 }};
	}

#pragma endregion
}