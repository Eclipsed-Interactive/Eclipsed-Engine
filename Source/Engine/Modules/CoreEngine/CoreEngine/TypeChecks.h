#pragma once

#include <string>
#include <type_traits>

template <typename T>
concept is_numerical = std::is_arithmetic<T>::value;

template <typename T>
concept is_integer = std::is_same<T, int>::value;

template <typename T>
concept is_float = std::is_same<T, float>::value;

template <typename T>
concept is_signed = std::is_signed<T>::value;


template<typename T>
concept is_string = requires(T t) { std::string{ t }; };


template <typename T>
concept IsStdString = std::is_same_v<std::decay_t<T>, std::string>;

template<typename T>
concept string_like = requires(T t) { std::string{ t }; };

template<typename T>
concept test = true;