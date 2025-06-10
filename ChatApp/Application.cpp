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

	loggedUser = nullptr;
}

Application::~Application() {
	delete fileHandler;
	freeMemory(users, chats);
}

void Application::run() {
	fileHandler->loadUsers(users);
	fileHandler->loadChats(chats, users);
	fileHandler->loadUserChats(users, chats);

	MyString userType;
	MyString command;
	MyVector<MyString> tokens;

	printLoginScreen();

	while (true)
	{
		command.getline(cin);
		tokens = command.split(' ');
		MyString action = tokens[0];

		if (!loggedUser)
		{
			if (action == "login" && tokens.getSize() == 3)
			{
				login(tokens[1], tokens[2], users, loggedUser, userType, fileHandler);
			}
			else if (action == "register" && tokens.getSize() == 3)
			{
				createRegularAccount(tokens[1], tokens[2], users, fileHandler);
			}
			else if (action == "register-admin" && tokens.getSize() == 3)
			{
				createAdminAccout(tokens[1], tokens[2], users, fileHandler);
			}
			else if (action == "quit")
			{
				break;
			}
		}
		else
		{
			if (action == "view-chats")
			{
				loggedUser->viewChats();
			}
			else if (action == "create-individual-chat" && tokens.getSize() == 2)
			{
				loggedUser->createIndividualChat(tokens[1], users, chats);
			}
			else if (action == "select-chat" && tokens.getSize() == 2)
			{
				if (tokens[1] == "")
				{
					cout << "Chat id cannot be empty!" << endl;
					continue;
				}

				loggedUser->selectChat(tokens[1].toInt());
			}
			else if (action == "create-group" && tokens.getSize() >= 3)
			{
				loggedUser->createGroupChat(tokens[1], tokens, users, chats);
			}
			else if (action == "leave-group" && tokens.getSize() == 2)
			{
				if (tokens[1] == "")
				{
					cout << "Chat id cannot be empty!" << endl;
					continue;
				}

				loggedUser->leaveGroupChat(tokens[1].toInt(), chats);
			}
			else if (action == "set-group-admin" && tokens.getSize() == 3)
			{
				if (tokens[1] == "")
				{
					cout << "Chat id cannot be empty!" << endl;
					continue;
				}

				loggedUser->setGroupAdmin(tokens[1].toInt(), tokens[2]);
			}
			else if (action == "add-to-group" && tokens.getSize() == 3)
			{
				if (tokens[1] == "")
				{
					cout << "Chat id cannot be empty!" << endl;
					continue;
				}

				loggedUser->addToGroup(tokens[1].toInt(), tokens[2], users);
			}
			else if (action == "kick-from-group" && tokens.getSize() == 3)
			{
				if (tokens[1] == "")
				{
					cout << "Chat id cannot be empty!" << endl;
					continue;
				}

				loggedUser->kickFromGroup(tokens[1].toInt(), tokens[2]);
			}
			else if (action == "group-stats" && tokens.getSize() == 2)
			{
				if (tokens[1] == "")
				{
					cout << "Chat id cannot be empty!" << endl;
					continue;
				}

				loggedUser->checkGroupStats(tokens[1].toInt());
			}
			else if (action == "toggle-group-add-approval" && tokens.getSize() == 2)
			{
				if (tokens[1] == "")
				{
					cout << "Chat id cannot be empty!" << endl;
					continue;
				}

				loggedUser->toggleAddApproval(tokens[1].toInt());
			}
			else if (action == "view-group-approval-list" && tokens.getSize() == 2)
			{
				if (tokens[1] == "")
				{
					cout << "Chat id cannot be empty!" << endl;
					continue;
				}

				loggedUser->viewAddRequests(tokens[1].toInt());
			}
			else if (action == "approve" && tokens.getSize() == 3)
			{
				if (tokens[1] == "")
				{
					cout << "Chat id cannot be empty!" << endl;
					continue;
				}

				loggedUser->approveAddRequest(tokens[1].toInt(), tokens[2]);
			}
			else if (action == "delete-user" && userType == "Admin" && tokens.getSize() == 2)
			{
				Admin* admin = dynamic_cast<Admin*>(loggedUser);
				admin->deleteUser(tokens[1], users, chats);
			}
			else if (action == "delete-group" && userType == "Admin" && tokens.getSize() == 2)
			{
				if (tokens[1] == "")
				{
					cout << "Chat id cannot be empty!" << endl;
					continue;
				}

				Admin* admin = dynamic_cast<Admin*>(loggedUser);
				admin->deleteGroupChat(tokens[1].toInt(), chats);
			}
			else if (action == "view-all-chats" && userType == "Admin")
			{
				Admin* admin = dynamic_cast<Admin*>(loggedUser);
				admin->viewAllChats(chats);
			}
			else if (action == "logout")
			{
				loggedUser = nullptr;
				userType = "";
				clearConsole();
				printLoginScreen();
			}
			else if (action == "quit")
			{
				break;
			}
		}
	}
}