#include <iostream>

#include "HelperFunctions.h"

using namespace std;

#pragma warning(disable:4996)

void clearConsole()
{
	cout << "\033[;H";
	cout << "\033[J";
}

void printLoginScreen() {
	cout << "Welcome to BlahBlah! To login or register use the following commands:" << endl;
	cout << "login <username> <password>" << endl;
	cout << "register <username> <password>" << endl;
	cout << "register-admin <username> <password>" << endl << endl;
}

MyString getCurrentDate() {
	MyString result;

	time_t t = time(nullptr);
	tm* now = localtime(&t);

	result += now->tm_mday;
	result += '.';
	result += (now->tm_mon + 1);
	result += '.';
	result += (now->tm_year + 1900);

	return result;
}

MyString getCurrentTime() {
	MyString result;

	time_t t = time(nullptr);
	tm* now = localtime(&t);

	result += now->tm_hour;
	result += ':';

	if (now->tm_min < 10) {
		result += '0';
	}

	result += now->tm_min;

	return result;
}

void freeMemory(MyVector<User*>& users, MyVector<Chat*>& chats) {
	for (size_t i = 0; i < users.getSize(); i++)
	{
		delete users[i];
	}

	for (size_t i = 0; i < chats.getSize(); i++)
	{
		delete chats[i];
	}
}