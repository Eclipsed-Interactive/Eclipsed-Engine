#pragma once

#include <type_traits>
#include <string>
#include <vector>

template<typename Base, typename Derived>
concept HasOnDrawInspectorMethod =
std::derived_from<Derived, Base>&&
    requires {
    &Base::OnDrawInspector();
    &Derived::OnDrawInspector();
};
    
template<typename T>
struct Is_String : std::disjunction<
    std::is_same<std::decay_t<T>, std::string>,
    std::is_same<std::decay_t<T>, const char*>,
    std::is_same<std::decay_t<T>, char*>
> {
};

template<typename T>
struct Is_Std_String : std::is_same<std::decay_t<T>, std::string> {};

template<typename T>
struct Is_C_String : std::false_type {};

template<>
struct Is_C_String<char*> : std::true_type {};
template<>
struct Is_C_String<const char*> : std::true_type {};
template<size_t N>
struct Is_C_String<char[N]> : std::true_type {};
template<size_t N>
struct Is_C_String<const char[N]> : std::true_type {};

template<typename T>
struct Is_Vector : std::false_type {};
template<typename T, typename Alloc>
struct Is_Vector<std::vector<T, Alloc>> : std::true_type {};



template<typename T>
struct Is_Std_Array : std::false_type {};
template<typename T, size_t N>
struct Is_Std_Array<std::array<T, N>> : std::true_type {};

template<typename T>
struct Is_C_Array : std::false_type {};
template<typename T, size_t N>
struct Is_C_Array<T[N]> : std::true_type {};



template<typename T>
struct Is_Array : std::disjunction<Is_Std_Array<T>, Is_C_Array<T>> {};





template<typename, typename = void>
struct Is_Serializable : std::false_type {};

template<typename T>
struct Is_Serializable<T, std::void_t<decltype(T::serializable)>>
    : std::bool_constant<T::serializable> {
};










template<typename T, typename Enable = void>
struct ElementType {};

// Specialization for vector
template<typename T>
struct ElementType<std::vector<T>> {
    using type = T;
};

// Specialization for arrays
template<typename T, std::size_t N>
struct ElementType<T[N]> {
    using type = T;
};

// Helper alias
template<typename T>
using ElementType_t = typename ElementType<T>::type;