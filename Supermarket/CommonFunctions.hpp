#include<iostream>


namespace COMMON
{
	template <typename T>
	void resize(T* arr, size_t size, size_t newCapacity);

	template <typename T>
	void copyArr(T* dest, const T* src, size_t size);
}

template <typename T>
void COMMON::resize(T* arr, size_t size, size_t newCapacity)
{
	T* newArr = new T[newCapacity];
	for (size_t i = 0; i < size; i++)
	{
		newArr[i] = arr[i];
	}
	delete arr;
	arr = newArr;
}

template <typename T>
void COMMON::copyArr<T>(T* dest, const T* src, size_t size)
{
	for (size_t i = 0; i < size; i++)
	{
		dest[i] = src[i];
	}
}
