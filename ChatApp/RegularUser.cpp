#include <iostream>

#include "RegularUser.h"

using namespace std;

RegularUser::RegularUser(const MyString& username, const MyString& password, FileHandler* fileHandler) : User(username, password, fileHandler) {

}

void RegularUser::printActions() const {
	cout << "view-chats" << endl;
	cout << "select-chat <chatId>" << endl;
	cout << "create-individual-chat <username>" << endl;
	cout << "create-group <groupName> <username1> <username2> ...." << endl;
	cout << "leave-group <chatId>" << endl;
	cout << "add-to-group <chatId> <username>" << endl;
	cout << "kick-from-group <chatId> <username>" << endl;
	cout << "set-group-admin <chatId> <username>" << endl;
	cout << "group-stats <chatId>" << endl << endl;
	cout << "toggle-group-add-approval <chatId>" << endl;
	cout << "view-group-approval-list <chatId>" << endl;
	cout << "approve <chatId> <username>" << endl << endl;

	cout << "logout" << endl;
	cout << "quit" << endl;
}