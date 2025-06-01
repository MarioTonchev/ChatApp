#include <iostream>

#include "HelperFunctions.h"

using namespace std;

void clearConsole()
{
	cout << "\033[;H";
	cout << "\033[J";
}

void printActions(const MyString& userType) {
	cout << "You are able to use the following commands:" << endl << endl;

	if (userType == "Admin")
	{
		cout << "delete-user <username>" << endl;
		cout << "delete-group <chatId>" << endl;
		cout << "view-all-chats" << endl << endl;
	}

	cout << "view-chats" << endl;
	cout << "select-chat <chatId>" << endl;
	cout << "create-individual-chat <username>" << endl;
	cout << "create-group <groupName> <username1> <username2> ...." << endl;
	cout << "leave-group <chatId>" << endl;
	cout << "add-to-group <chatId> <username>" << endl;
	cout << "kick-from-group <chatId> <username>" << endl;
	cout << "set-group-admin <chatId> <username>" << endl;
	cout << "group-stats <chatId>" << endl << endl;

	cout << "logout" << endl;
	cout << "quit" << endl;
}