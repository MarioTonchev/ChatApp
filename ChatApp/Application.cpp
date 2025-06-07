#include "Application.h"

Application::Application(const MyString& fileMode) {
	if (fileMode == "txt")
	{
		fileHandler = new TextFileHandler();
	}
	else if (fileMode == "bin")
	{
		fileHandler = new BinaryFileHandler();
	}
	else
	{
		throw std::invalid_argument("Invalid file mode: choose between 'txt' and 'bin'");
	}
}

void Application::run() {
	MyVector<User*> users;
	MyVector<Chat*> chats;

	fileHandler->loadUsers(users);
	fileHandler->loadChats(chats, users);
	fileHandler->loadUserChats(users, chats);

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
				login(tokens[1], tokens[2], users, loggedUser, userType, fileHandler);
			}
			else if (tokens[0] == "register" && tokens.getSize() >= 3)
			{
				createRegularAccount(tokens[1], tokens[2], users, fileHandler);
			}
			else if (tokens[0] == "register-admin")
			{
				createAdminAccout(tokens[1], tokens[2], users, fileHandler);
			}
			else if (tokens[0] == "quit")
			{
				break;
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
			else if (tokens[0] == "set-group-admin" && tokens.getSize() >= 3)
			{
				if (tokens[1] == "")
				{
					cout << "Chat id cannot be empty!" << endl;
					continue;
				}

				loggedUser->setGroupAdmin(tokens[1].toInt(), tokens[2]);
			}
			else if (tokens[0] == "add-to-group" && tokens.getSize() >= 3)
			{
				if (tokens[1] == "")
				{
					cout << "Chat id cannot be empty!" << endl;
					continue;
				}

				loggedUser->addToGroup(tokens[1].toInt(), tokens[2], users);
			}
			else if (tokens[0] == "kick-from-group" && tokens.getSize() >= 3)
			{
				if (tokens[1] == "")
				{
					cout << "Chat id cannot be empty!" << endl;
					continue;
				}

				loggedUser->kickFromGroup(tokens[1].toInt(), tokens[2]);
			}
			else if (tokens[0] == "group-stats" && tokens.getSize() == 2)
			{
				if (tokens[1] == "")
				{
					cout << "Chat id cannot be empty!" << endl;
					continue;
				}

				loggedUser->checkGroupStats(tokens[1].toInt());
			}
			else if (tokens[0] == "delete-user" && userType == "Admin" && tokens.getSize() >= 2)
			{
				Admin* admin = dynamic_cast<Admin*>(loggedUser);
				admin->deleteUser(tokens[1], users, chats);
			}
			else if (tokens[0] == "delete-group" && userType == "Admin" && tokens.getSize() == 2)
			{
				if (tokens[1] == "")
				{
					cout << "Chat id cannot be empty!" << endl;
					continue;
				}

				Admin* admin = dynamic_cast<Admin*>(loggedUser);
				admin->deleteGroupChat(tokens[1].toInt(), chats);
			}
			else if (tokens[0] == "view-all-chats" && userType == "Admin")
			{
				Admin* admin = dynamic_cast<Admin*>(loggedUser);
				admin->viewAllChats(chats);
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

	freeMemory(users, chats);
}