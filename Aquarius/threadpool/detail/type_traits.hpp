#pragma once
#include <functional>
//
//   std::function<void()>
//

template<class T>
struct function_rt
{
	using type = void;
};

template<class T>
struct function_rt<std::function<T>>
{
	using type = T;
};

template<class T>
struct function_rt<std::function<T()>>
{
	using type = T;
};

template<class T, class... Args>
struct function_rt<std::function<T(Args...)>>
{
	using type = T;
};


template<class T>
using function_rt_t = typename function_rt<T>::type;