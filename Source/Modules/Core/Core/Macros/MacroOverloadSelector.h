
#ifndef _IOJWAD
#define _IOJWAD
/*
	It cant have multiple implementations with the same amount of arguments. It must be different.
*/

#define _EXPAND(x) x

#define _GET_ARG_COUNT(...)  _EXPAND(_GET_ARG_COUNT_IMPL(__VA_ARGS__, 5, 4, 3, 2, 1))
#define _GET_ARG_COUNT_IMPL(_1, _2, _3, _4, _5, N, ...) N

#define _COMBINE(name, n) name##n
#define _GET_MACRO_NAME(name, n) _COMBINE(name, n)

#define _BASE_SELECTION_IMPL(name, count, ...) _GET_MACRO_NAME(name##_, count)

#define BASE_SELECTION(...) _EXPAND(_BASE_SELECTION_IMPL(_EXPAND(BASE_MACRO_NAME), _GET_ARG_COUNT(__VA_ARGS__))(__VA_ARGS__))


#ifndef BASE_MACRO_NAME
#error("The macro: 'BASE_MACRO_NAME' has to be defined and given a string.")
#endif

#endif