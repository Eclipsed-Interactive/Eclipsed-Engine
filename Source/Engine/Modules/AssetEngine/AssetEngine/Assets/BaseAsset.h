#pragma once

#include "CoreEngine/EventSystem/EventSystem.h"

#define BASE_ASSET(_Asset)                                      \
public:                                                          \
    _Asset##Data* data = nullptr;                                \
                                                                  \
    /* Constructor from data */                                   \
    _Asset(_Asset##Data* _data) : data(_data) {                 \
        isValid = true;                                          \
        if(data) data->refCount++;                               \
    }                                                             \
                                                                  \
    /* Default constructor */                                     \
    _Asset() = default;                                          \
                                                                  \
    /* Destructor */                                             \
    ~_Asset() {                                                  \
        if(!isValid || !data) return;                            \
        data->refCount--;                                        \
        if(data->refCount == 0)                                   \
            EventSystem::Trigger("UnloadAsset", data->assetID);  \
    }                                                             \
                                                                  \
    /* Copy constructor */                                       \
    _Asset(const _Asset& other) : data(other.data), isValid(other.isValid) { \
        if(data) data->refCount++;                               \
    }                                                             \
                                                                  \
    /* Copy assignment */                                        \
    _Asset& operator=(const _Asset& other) {                     \
        if(this != &other) {                                     \
            if(data && --data->refCount == 0)                    \
                EventSystem::Trigger("UnloadAsset", data->assetID); \
            data = other.data;                                   \
            isValid = other.isValid;                             \
            if(data) data->refCount++;                           \
        }                                                        \
        return *this;                                            \
    }                                                             \
                                                                  \
    /* Move constructor */                                       \
    _Asset(_Asset&& other) noexcept : data(other.data), isValid(other.isValid) { \
        other.data = nullptr;                                     \
        other.isValid = false;                                    \
    }                                                             \
                                                                  \
    /* Move assignment */                                        \
    _Asset& operator=(_Asset&& other) noexcept {                \
        if(this != &other) {                                     \
            if(data && --data->refCount == 0)                    \
                EventSystem::Trigger("UnloadAsset", data->assetID); \
            data = other.data;                                   \
            isValid = other.isValid;                             \
            other.data = nullptr;                                 \
            other.isValid = false;                                \
        }                                                        \
        return *this;                                            \
    }                                                             \
                                                                  \
    _Asset##Data* GetData() { return data; }                     \
    bool IsValid() const { return isValid; }                     \
    const std::string& GetAssetID() const { return data->assetID; } \
private:                                                          \
    bool isValid = false;
