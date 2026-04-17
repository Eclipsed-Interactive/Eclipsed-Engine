#include "JsonSerializer.h"

#include "CoreEngine/Math/Vector/Vector2.h"

#define s(t) #t

namespace Eclipse
{
	template<>
	struct JsonSerializer<int>
	{
	public:
		static rapidjson::Value ToJsonImpl(const int& aValue, rapidjson::Document::AllocatorType& alloc)
		{
			return rapidjson::Value(aValue);
		}

		static int FromJsonImpl(const rapidjson::Value& aValue)
		{
			return aValue.GetInt();
		}
	};

	template<>
	struct JsonSerializer<float>
	{
	public:
		static rapidjson::Value ToJsonImpl(const float& aValue, rapidjson::Document::AllocatorType& alloc)
		{
			return rapidjson::Value(aValue);
		}

		static float FromJsonImpl(const rapidjson::Value& aValue)
		{
			return aValue.GetFloat();
		}
	};

	template<>
	struct JsonSerializer<double>
	{
	public:
		static rapidjson::Value ToJsonImpl(const double& aValue, rapidjson::Document::AllocatorType& alloc)
		{
			return rapidjson::Value(aValue);
		}

		static double FromJsonImpl(const rapidjson::Value& aValue)
		{
			return aValue.GetDouble();
		}
	};

	template<>
	struct JsonSerializer<unsigned>
	{
	public:
		static rapidjson::Value ToJsonImpl(const unsigned& aValue, rapidjson::Document::AllocatorType& alloc)
		{
			return rapidjson::Value(aValue);
		}

		static unsigned FromJsonImpl(const rapidjson::Value& aValue)
		{
			return aValue.GetUint();
		}
	};

	template<>
	struct JsonSerializer<bool>
	{
	public:
		static rapidjson::Value ToJsonImpl(const bool& aValue, rapidjson::Document::AllocatorType& alloc)
		{
			return rapidjson::Value(aValue);
		}

		static bool FromJsonImpl(const rapidjson::Value& aValue)
		{
			return aValue.GetBool();
		}
	};

	template<typename T>
	struct JsonSerializer <Math::Vector2<T>>
	{
	private:
		using CleanT = std::remove_cvref_t<T>;

	public:
		static rapidjson::Value ToJsonImpl(const Math::Vector2<T>& aValue, rapidjson::Document::AllocatorType& alloc)
		{
			rapidjson::Value val(rapidjson::kObjectType);

			val.AddMember("x", aValue.x, alloc);
			val.AddMember("y", aValue.y, alloc);

			return val;
		}

		static Math::Vector2<T> FromJsonImpl(const rapidjson::Value& aValue)
		{
			assert(aValue.IsObject());
			assert(aValue.HasMember("x"));
			assert(aValue.HasMember("y"));

			const rapidjson::Value& x = aValue["x"];
			const rapidjson::Value& y = aValue["y"];

			assert(x.IsNumber());
			assert(y.IsNumber());

			Math::Vector2<T> vec;
			vec.x = static_cast<T>(x.GetDouble());
			vec.y = static_cast<T>(x.GetDouble());

			return vec;
		}
	};
}