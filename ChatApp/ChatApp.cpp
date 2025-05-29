#include <iostream>
#include "MyVector.hpp"
#include "MyString.h"
#include "User.h"
#include "UserUtils.h"
#include "HelperFunctions.h"

using namespace std;

int main()
{
	MyVector<User*> users;

	loadUsers(users);

	User* loggedUser = nullptr;
	MyString command;
	MyString userType;
	MyVector<MyString> tokens;

	while (true)
	{
		if (!loggedUser)
		{
			cout << "Welcome to BlahBlah! To login or register use the following commands:" << endl;
			cout << "login <username> <password>" << endl;
			cout << "register <username> <password>" << endl;

			command.getline(cin);
			tokens = command.split(' ');

			if (tokens[0] == "login" && tokens.getSize() >= 3)
			{
				login(tokens[1], tokens[2], users, loggedUser, userType);
			}
			else if (tokens[0] == "register" && tokens.getSize() >= 3)
			{
				createAccount(tokens[1], tokens[2], users);
			}
		}
		else
		{
			command.getline(cin);
			tokens = command.split(' ');

			if (tokens[0] == "logout")
			{
				loggedUser = nullptr;
				userType = "";
				clearConsole();
			}
			else if (tokens[0] == "quit")
			{
				break;
			}
		}
	}
}