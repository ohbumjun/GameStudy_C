template<typename T>
T&& forward(typename remove_reference<T>::&type param)
{
	return static_cast<T&&>(param);
}

template<typename T>
T& && forward(typename remove_reference<T&>::& type param)
{
	return static_cast<T& &&>(param);
}


template<typename T>
T& forward(T& type param)
{
	return static_cast<T&>(param);
}

