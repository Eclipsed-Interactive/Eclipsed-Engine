#pragma once


#define ASSET_OPERATORS_DEF(TYPE, HANDLE, MANAGER)			\
friend class MANAGER;										\
public:														\
    size_t GetAssetID() const;                              \
	TYPE() = default;										\
	explicit TYPE(HANDLE* handle);							\
	~TYPE();												\
	TYPE(const TYPE& other);								\
	TYPE& operator=(const TYPE& other);						\
	TYPE(TYPE&& other) noexcept;							\
	TYPE& operator=(TYPE&& other) noexcept;					\
    bool IsValid() const;                                   \
    void IsAsset() {}                                       \
    HANDLE* GetHandle() { return dataHandle; }                \
private:													\
	HANDLE* dataHandle = nullptr;







#define ASSET_OPERATORS_IMPL(TYPE, HANDLE)                                                          \
TYPE::TYPE(HANDLE* handle)                                                                          \
    : dataHandle(handle)                                                                            \
{                                                                                                   \
    if (dataHandle)                                                                                 \
        dataHandle->refCount++;                                                                     \
}                                                                                                   \
																							        \
size_t TYPE::GetAssetID() const                                                                     \
{                                                                                                   \
    return dataHandle->assetID;                                                                     \
}                                                                                                   \
																							        \
TYPE::~TYPE()                                                                                       \
{                                                                                                   \
    if (dataHandle)                                                                                 \
        dataHandle->refCount--;                                                                     \
}                                                                                                   \
                                                                                                    \
TYPE::TYPE(const TYPE& other)                                                                       \
    : dataHandle(other.dataHandle)                                                                  \
{                                                                                                   \
    if (dataHandle)                                                                                 \
        dataHandle->refCount++;                                                                     \
}                                                                                                   \
                                                                                                    \
TYPE& TYPE::operator=(const TYPE& other)                                                            \
{                                                                                                   \
    if (this == &other)                                                                             \
        return *this;                                                                               \
                                                                                                    \
    if (dataHandle)                                                                                 \
        dataHandle->refCount--;                                                                     \
                                                                                                    \
    dataHandle = other.dataHandle;                                                                  \
                                                                                                    \
    if (dataHandle)                                                                                 \
        dataHandle->refCount++;                                                                     \
                                                                                                    \
    return *this;                                                                                   \
}                                                                                                   \
                                                                                                    \
TYPE::TYPE(TYPE&& other) noexcept                                                                   \
    : dataHandle(other.dataHandle)                                                                  \
{                                                                                                   \
    other.dataHandle = nullptr;                                                                     \
}                                                                                                   \
                                                                                                    \
TYPE& TYPE::operator=(TYPE&& other) noexcept                                                        \
{                                                                                                   \
    if (this != &other)                                                                             \
    {                                                                                               \
        if (dataHandle)                                                                             \
            dataHandle->refCount--;                                                                 \
                                                                                                    \
        dataHandle = other.dataHandle;                                                              \
        other.dataHandle = nullptr;                                                                 \
    }                                                                                               \
    return *this;                                                                                   \
}                                                                                                   \
                                                                                                    \
bool TYPE::IsValid() const                                                                          \
{                                                                                                   \
    return dataHandle != nullptr;                                                                   \
}

















#define ASSET_IMPL(TYPE, HANDLE, MANAGER)																	\
friend class MANAGER;																						\
public:																										\
TYPE() = default;																							\
TYPE(HANDLE* handle) : dataHandle(handle) { if (dataHandle) dataHandle->refCount++; }						\
~TYPE() { if (dataHandle) dataHandle->refCount--; }															\
TYPE(const TYPE& other) : dataHandle(other.dataHandle) { if (dataHandle)dataHandle->refCount++; }			\
TYPE operator=(const TYPE& other)																	\
{																											\
	if (this == &other)																						\
		return *this;																						\
																											\
	if (dataHandle)																							\
		dataHandle->refCount--;																				\
																											\
	dataHandle = other.dataHandle;																			\
																											\
	if (dataHandle)																							\
		dataHandle->refCount++;																				\
																											\
	return *this;																							\
}																											\
TYPE(TYPE&& other) noexcept																					\
	: dataHandle(other.dataHandle)																			\
{																											\
	other.dataHandle = nullptr;																				\
}																											\
                                                                                                            \
TYPE& operator=(TYPE&& other) noexcept																		\
{																											\
	if (this != &other)																						\
	{																										\
		if (dataHandle)																						\
			dataHandle->refCount--;																			\
																											\
		dataHandle = other.dataHandle;																		\
		other.dataHandle = nullptr;																			\
	}																										\
	return *this;																							\
}																											\
																											\
	private:																								\
		HANDLE* dataHandle = nullptr;