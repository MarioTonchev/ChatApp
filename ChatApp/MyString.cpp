#include <stdexcept>
#include <cstring>

#include "MyString.h"

using namespace std;

void MyString::setData(const char* str) {
	if (!str)
	{
		throw invalid_argument("String cannot be a nullptr!");
	}

	size = strlen(str);

	data = new char[size + 1];
	strcpy_s(data, size + 1, str);
}

void MyString::free() {
	delete[] data;
	data = nullptr;
	size = 0;
}

const char* MyString::get() const {
	return data;
}

int MyString::len() const {
	return size;
}

MyString::MyString(const char* str) {
	setData(str);
}

MyString::~MyString() {
	free();
}

MyString::MyString(const MyString& other) {
	setData(other.data);
}

MyString& MyString::operator=(const MyString& other) {
	if (this != &other)
	{
		free();
		setData(other.data);
	}

	return *this;
}

MyString& MyString::operator+=(const MyString& other) {
	int newSize = size + other.size;
	char* result = new char[newSize + 1];

	strcpy_s(result, newSize + 1, data);
	strcat_s(result, newSize + 1, other.data);

	delete[] data;
	data = result;
	size = newSize;

	return *this;
}

MyString& MyString::operator+=(const char* str) {
	int newSize = size + strlen(str);
	char* result = new char[newSize + 1];

	strcpy_s(result, newSize + 1, data);
	strcat_s(result, newSize + 1, str);

	delete[] data;
	data = result;
	size = newSize;

	return *this;
}

MyString& MyString::operator+=(const char ch) {
	int newSize = size + 1;
	char* result = new char[newSize + 1];

	strcpy_s(result, size + 1, data);

	result[size] = ch;
	result[size + 1] = '\0';

	delete[] data;
	data = result;
	size = newSize;

	return *this;
}

MyString& MyString::operator+=(int num) {
	int numCopy = num;
	int numSize = 0;

	while (numCopy > 0)
	{
		numCopy /= 10;
		numSize++;
	}

	int newSize = size + numSize;
	char* result = new char[newSize + 1];
	strcpy_s(result, size + 1, data);

	int index = newSize - 1;

	while (num > 0)
	{
		char c = (num % 10) + '0';
		num /= 10;

		result[index--] = c;
	}

	result[newSize] = '\0';

	delete[] data;
	data = result;
	size = newSize;

	return *this;
}

char& MyString::operator[](int index) {
	if (index < 0 || index >= size)
	{
		throw out_of_range("Index is out of bounds!");
	}

	return data[index];
}

char MyString::operator[](int index) const {
	if (index < 0 || index >= size)
	{
		throw out_of_range("Index is out of bounds!");
	}

	return data[index];
}

bool operator==(const MyString& left, const MyString& right) {
	return strcmp(left.get(), right.get()) == 0;
}

bool operator!=(const MyString& left, const MyString& right) {
	return !(left == right);
}

ostream& operator<<(ostream& os, const MyString& str) {
	os << str.data;
	return os;
}

istream& operator>>(istream& is, MyString& str) {
	char buffer[256];
	is >> buffer;

	delete[] str.data;

	str.size = strlen(buffer);
	str.data = new char[str.size + 1];
	strcpy_s(str.data, str.size + 1, buffer);

	return is;
}

void MyString::getline(istream& is) {
	char buffer[256];

	is.getline(buffer, sizeof(buffer));

	free();
	int newSize = strlen(buffer);
	data = new char[newSize + 1];
	strcpy_s(data, newSize + 1, buffer);
	size = newSize;
}

int MyString::toInt() const {
	int result = 0;
	int sign = 1;
	int start = 0;

	if (data[0] == '-')
	{
		start++;
		sign = -1;
	}
	else if (data[0] == '+')
	{
		start++;
	}

	for (size_t i = start; i < size; i++)
	{
		if (data[i] < '0' || data[i] > '9')
		{
			return 0;
		}

		result *= 10;
		result += data[i] - '0';
	}

	return result * sign;
}

double MyString::toDouble() const {
	double result = 0.0;
	int sign = 1;
	double fractionalPart = 0.0;
	double divisor = 1.0;

	int start = 0;
	int dotIndex = -1;

	if (data[0] == '-')
	{
		start++;
		sign = -1;
	}
	else if (data[0] == '+')
	{
		start++;
	}

	for (size_t i = start; i < size; i++)
	{
		if (data[i] == '.')
		{
			dotIndex = i;
			break;
		}

		result *= 10;
		result += data[i] - '0';
	}

	if (dotIndex != -1)
	{
		for (size_t i = dotIndex + 1; i < size; i++)
		{
			fractionalPart = fractionalPart * 10 + (data[i] - '0');
			divisor *= 10;
		}
	}

	result += fractionalPart / divisor;

	return result * sign;
}