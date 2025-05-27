#pragma once

#include <iostream>
#include <cstring>   
#include "MyVector.hpp"

class MyString {
private:
	char* data;
	int size;

	void setData(const char* str);
	void free();
public:
	MyString(const char* str = "");
	~MyString();
	MyString(const MyString& other);
	MyString& operator=(const MyString& other);

	MyString& operator+=(const MyString& other);
	MyString& operator+=(const char* str);
	MyString& operator+=(const char ch);
	MyString& operator+=(int num);
	char& operator[](int index);
	char operator[](int index) const;
	friend std::ostream& operator<<(std::ostream& os, const MyString& str);
	friend std::istream& operator>>(std::istream& is, MyString& str);

	MyVector<MyString> split(const char delimeter) {
		MyVector<MyString> result;

		MyString str;

		for (size_t i = 0; i < size; i++)
		{
			if (data[i] != delimeter)
			{
				str += data[i];
			}
			else
			{
				result.push_back(str);

				str = "";
			}
		}

		result.push_back(str);

		return result;
	}
	void getline(std::istream& is);
	int toInt() const;
	double toDouble() const;

	const char* get() const;
	int len() const;
};

bool operator==(const MyString& left, const MyString& right);
bool operator!=(const MyString& left, const MyString& right);