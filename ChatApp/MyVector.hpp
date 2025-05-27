#pragma once

#include <stdexcept>

template <typename T>

class MyVector {
private:
	T* items;
	int size;
	int capacity;

	void resize();
public:
	MyVector();

	~MyVector();

	MyVector(const MyVector& other);

	MyVector& operator=(const MyVector<T>& other);

	T& operator[](int index);

	const T& operator[](int index) const;

	void push_back(const T& element);
	void pop_back();
	void remove_at(int index);

	int getSize() const;
};

template <typename T>
void MyVector<T>::resize() {
	capacity *= 2;
	T* newItems = new T[capacity];

	for (size_t i = 0; i < size; i++)
	{
		newItems[i] = items[i];
	}

	delete[] items;
	items = newItems;
}

template <typename T>
MyVector<T>::MyVector() : size(0), capacity(4) {
	items = new T[capacity];
};

template <typename T>
MyVector<T>::~MyVector() {
	delete[] items;
	items = nullptr;
	size = 0;
	capacity = 0;
}

template <typename T>
MyVector<T>::MyVector(const MyVector& other) {
	size = other.size;
	capacity = other.capacity;
	items = new T[capacity];

	for (int i = 0; i < size; ++i)
	{
		items[i] = other.items[i];
	}
}

template <typename T>
MyVector<T>& MyVector<T>::operator=(const MyVector<T>& other) {
	if (this != &other)
	{
		delete[] items;
		size = other.size;
		capacity = other.capacity;
		items = new T[capacity];

		for (int i = 0; i < size; ++i)
		{
			items[i] = other.items[i];
		}
	}

	return *this;
}

template <typename T>
T& MyVector<T>::operator[](int index) {
	if (index < 0 || index >= size)
	{
		throw std::out_of_range("Index is out of bounds!");
	}

	return items[index];
}

template <typename T>
const T& MyVector<T>::operator[](int index) const {
	if (index < 0 || index >= size)
	{
		throw std::out_of_range("Index is out of bounds!");
	}

	return items[index];
}

template <typename T>
void MyVector<T>::push_back(const T & element) {
	if (size >= capacity)
	{
		resize();
	}

	items[size++] = element;
}

template <typename T>
void MyVector<T>::pop_back() {
	if (size == 0)
	{
		return;
	}

	size--;
}

template <typename T>
void MyVector<T>::remove_at(int index) {
	size--;
	for (size_t i = index; i < size; i++)
	{
		items[i] = items[i + 1];
	}
}

template <typename T>
int MyVector<T>::getSize() const {
	return size;
}