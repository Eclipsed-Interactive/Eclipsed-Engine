#pragma once

#include "AssetEngine/Data/TextureData.h"
#include "BaseAsset.h"

#include "CoreEngine/Math/RectSizePos.h"

namespace Eclipse
{
	class Texture
	{
	public:
	public:
		TextureData* data = nullptr; Texture(TextureData* _data) : data(_data)
		{
			isValid = true; if (data) data->refCount++;
		}

		Texture() = default;
		
		
		
		~Texture()
		{
			if (!isValid || !data)
				return;
			data->refCount--;
			if (data->refCount == 0)
				EventSystem::Trigger("UnloadAsset", data->assetID);
		}

		Texture(const Texture& other) 
			: data(other.data), isValid(other.isValid) 
		{
			if (data) data->refCount++;
		}
		
		Texture& operator=(const Texture& other) 
		{
			if (this != &other) {
				if (data && --data->refCount == 0) EventSystem::Trigger("UnloadAsset", data->assetID); data = other.data; isValid = other.isValid; if (data) data->refCount++;
			} 
			return *this;
		} 

		Texture(Texture&& other) noexcept 
			: data(other.data), isValid(other.isValid) 
		{
			other.data = nullptr; other.isValid = false;
		} 
		
		Texture& operator=(Texture&& other) noexcept {
			if (this != &other) 
			{
				if (data && --data->refCount == 0) 
					EventSystem::Trigger("UnloadAsset", data->assetID); 
				
				data = other.data;
				isValid = other.isValid; 
				other.data = nullptr; 
				other.isValid = false;
			} 
			
			return *this;
		} TextureData* GetData() {
			return data;
		} bool IsValid() const {
			return isValid;
		} const std::string& GetAssetID() const {
			return data->assetID;
		} private: bool isValid = false;;

	public:
		int GetWidth() const;
		int GetHeight() const;

		float GetAspectRatio() const;

		unsigned GetTextureID() const;

		const Math::Vector2f& GetTextureSizeNormilized() const;
		Math::Vector2f GetDimDivOne() const;

		void Bind(int slot = 0) const;
		void Unbind(int slot = 0) const;

		// Temporary
		std::vector<Math::RectSizePos> GetRects() const;
	};
}