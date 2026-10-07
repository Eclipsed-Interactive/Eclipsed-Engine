#pragma once

namespace Eclipse
{
	using I8 = signed char;
	using I16 = signed short;
	using I32 = signed int;
	using I64 = signed long long;

	using U8 = unsigned char;
	using U16 = unsigned short;
	using U32 = unsigned int;
	using U64 = unsigned long long;

	using F32 = float;
	using F64 = double;

	using Size = decltype(sizeof(0));

	// Byte
	enum class Byte : unsigned char {};

	constexpr Byte operator<<(Byte byte, Size shift) { return static_cast<Byte>( static_cast<U8>(byte) << shift ); }
	constexpr Byte operator>>(Byte byte, Size shift) { return static_cast<Byte>( static_cast<U8>(byte) >> shift ); }

	constexpr Byte operator|(Byte a, Byte b) { return static_cast<Byte>( static_cast<U8>(a) | static_cast<U8>(b) ); }
	constexpr Byte operator&(Byte a, Byte b) { return static_cast<Byte>( static_cast<U8>(a) & static_cast<U8>(b) ); }
	constexpr Byte operator^(Byte a, Byte b) { return static_cast<Byte>( static_cast<U8>(a) ^ static_cast<U8>(b) ); }
	constexpr Byte operator~(Byte byte) { return static_cast<Byte>( ~static_cast<U8>(byte) ); }

	constexpr Byte& operator|=(Byte& a, Byte b) { a = a | b; return a; }
	constexpr Byte& operator&=(Byte& a, Byte b) { a = a & b; return a; }
	constexpr Byte& operator^=(Byte& a, Byte b) { a = a ^ b; return a; }

	// Checks
	static_assert(sizeof(I8) == 1, "Requires the typ 'I8' to be 1 bytes.");
	static_assert(sizeof(I16) == 2, "Requires the typ 'I16' to be 2 bytes.");
	static_assert(sizeof(I32) == 4, "Requires the typ 'I32' to be 4 bytes.");
	static_assert(sizeof(I64) == 8, "Requires the typ 'I64' to be 8 bytes.");

	static_assert(sizeof(U8) == 1, "Requires the typ 'U8' to be 1 bytes.");
	static_assert(sizeof(U16) == 2, "Requires the typ 'U16' to be 2 bytes.");
	static_assert(sizeof(U32) == 4, "Requires the typ 'U32' to be 4 bytes.");
	static_assert(sizeof(U64) == 8, "Requires the typ 'U64' to be 8 bytes.");

	static_assert(sizeof(F32) == 4, "Requires the typ 'F32' to be 4 bytes.");
	static_assert(sizeof(F64) == 8, "Requires the typ 'F64' to be 8 bytes.");

	static_assert(sizeof(Byte) == 1, "Requires the typ 'Byte' to be 1 bytes.");
}