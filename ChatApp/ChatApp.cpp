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
	MyVector<Chat*> chats;

	loadUsers(users);
	loadChats(chats, users);
	loadUserChats(users, chats);

	User* loggedUser = nullptr;
	MyString command;
	MyString userType;
	MyVector<MyString> tokens;

	cout << "Welcome to BlahBlah! To login or register use the following commands:" << endl;

	while (true)
	{
		if (!loggedUser)
		{
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

			if (tokens[0] == "view-chats")
			{
				loggedUser->viewChats();
			}
			else if (tokens[0] == "create-individual-chat" && tokens.getSize() >= 2)
			{
				loggedUser->createIndividualChat(tokens[1], users, chats);
			}
			else if (tokens[0] == "select-chat" && tokens.getSize() == 2)
			{
				if (tokens[1] == "")
				{
					cout << "Chat id cannot be empty!" << endl;
					continue;
				}

				loggedUser->selectChat(tokens[1].toInt(), chats);
			}
			else if (tokens[0] == "create-group" && tokens.getSize() >= 3)
			{
				loggedUser->createGroupChat(tokens[1], tokens, users, chats);
			}
			else if (tokens[0] == "leave-group" && tokens.getSize() == 2)
			{
				if (tokens[1] == "")
				{
					cout << "Chat id cannot be empty!" << endl;
					continue;
				}

				loggedUser->leaveGroupChat(tokens[1].toInt(), chats);
			}
			else if (tokens[0] == "logout")
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