#include "TextFileHandler.h"

#include <iostream>
#include <fstream>
#include <stdexcept>

using namespace std;

void TextFileHandler::saveUserToFile(User* user, const MyString& userType) {
	ofstream os(usersFile, ios::app);

	if (!os.is_open())
	{
		throw invalid_argument("Error: Could not open file!");
	}

	os << userType << "|";

	os << user->getUsername() << "|" << user->getPassword();

	if (userType == "Admin")
	{
		Admin* admin = dynamic_cast<Admin*>(user);
		os << "|" << admin->getAdminId();
	}

	os << endl;

	os.close();
}
void TextFileHandler::loadUsers(MyVector<User*>& users) {
	ifstream is(usersFile);

	if (!is.is_open())
	{
		throw invalid_argument("Error: Could not open file!");
	}

	MyString buffer;
	while (is.peek() != EOF)
	{
		buffer.getline(is);
		MyVector<MyString> tokens = buffer.split('|');

		if (tokens[0] == "RegularUser")
		{
			RegularUser* user = new RegularUser(tokens[1], tokens[2], this);
			users.push_back(user);
		}
		else if (tokens[0] == "Admin")
		{
			Admin* admin = new Admin(tokens[3].toInt(), tokens[1], tokens[2], this);
			users.push_back(admin);
		}
	}

	is.close();
}

void TextFileHandler::saveChatToFile(Chat* chat, const MyString& chatType) {
	if (!chat)
	{
		throw invalid_argument("Chat cannot be null!");
	}

	MyString fileName = chatsFile;
	fileName += chat->getChatId();
	fileName += ".txt";

	ofstream os(fileName.get());

	if (!os.is_open())
	{
		throw invalid_argument("Error: Could not open file!");
	}

	if (chatType == "GroupChat")
	{
		GroupChat* groupChat = dynamic_cast<GroupChat*>(chat);

		os << "GroupChat" << "|" << chat->getChatId() << "|"
			<< groupChat->getChatName() << "|" << groupChat->getRequiresApproval() << "|";

		if (groupChat->getAdmins().getSize() == 0)
		{
			os << "noadmins" << endl;
		}

		for (size_t i = 0; i < groupChat->getAdmins().getSize(); i++)
		{
			if (i < groupChat->getAdmins().getSize() - 1)
			{
				os << groupChat->getAdmins()[i]->getUsername() << ",";
			}
			else
			{
				os << groupChat->getAdmins()[i]->getUsername() << endl;
			}
		}
	}
	else
	{
		os << "IndividualChat" << "|" << chat->getChatId() << endl;
	}

	for (size_t i = 0; i < chat->getMessages().getSize(); i++)
	{
		os << chat->getMessages()[i].getSender() << "|" << chat->getMessages()[i].getContent() << "|"
			<< chat->getMessages()[i].getDate() << "|" << chat->getMessages()[i].getTime() << endl;
	}

	os.close();
}
void TextFileHandler::loadChats(MyVector<Chat*>& chats, MyVector<User*>& users) {
	MyVector<int> chatIds = this->getChatIds();

	for (size_t i = 0; i < chatIds.getSize(); i++)
	{
		MyString file = chatsFile;
		file += chatIds[i];
		file += ".txt";

		ifstream is(file.get());

		if (!is.is_open())
		{
			throw invalid_argument("Error: Could not open file!");
		}

		MyString buffer;
		buffer.getline(is);
		MyVector<MyString> tokens = buffer.split('|');

		Chat* chat = nullptr;

		if (tokens[0] == "IndividualChat")
		{
			chat = new IndividualChat(tokens[1].toInt());
		}
		else if (tokens[0] == "GroupChat")
		{
			chat = new GroupChat(tokens[1].toInt(), tokens[2], tokens[3].toInt());

			if (tokens[4] != "noadmins")
			{
				MyVector<MyString> adminsUsernames = tokens[4].split(',');

				for (size_t i = 0; i < adminsUsernames.getSize(); i++)
				{
					User* user = findUser(adminsUsernames[i], users);

					dynamic_cast<GroupChat*>(chat)->addAdmin(user);
				}
			}
		}

		if (!chat)
		{
			throw runtime_error("Chat cannot be null!");
		}

		while (is.peek() != EOF)
		{
			buffer.getline(is);
			tokens = buffer.split('|');

			Message message(tokens[0], tokens[1], tokens[2], tokens[3]);
			chat->addMessage(message);
		}

		chats.push_back(chat);

		is.close();
	}

	this->loadChatApprovalList(chats, users);
}

void TextFileHandler::saveChatIdToFile(Chat* chat) {
	ofstream os(chatIdsFile, ios::app);

	if (!os.is_open())
	{
		throw invalid_argument("Error: Could not open file!");
	}

	os << chat->getChatId() << endl;
	os.close();
}
MyVector<int> TextFileHandler::getChatIds() {
	ifstream is(chatIdsFile);

	if (!is.is_open())
	{
		throw invalid_argument("Error: Could not open file!");
	}

	MyVector<int> result;

	MyString buffer;
	while (is.peek() != EOF)
	{
		buffer.getline(is);

		result.push_back(buffer.toInt());
	}

	is.close();

	return result;
}

void TextFileHandler::saveUserToChatApprovalList(const MyString& username, int chatId) {
	ofstream os(approvalListFile, ios::app);

	if (!os.is_open())
	{
		throw invalid_argument("Error: Could not open file!");
	}

	os << username << "|" << chatId << endl;
	os.close();
}
void TextFileHandler::loadChatApprovalList(MyVector<Chat*>& chats, MyVector<User*>& users) {
	ifstream is(approvalListFile);

	if (!is.is_open())
	{
		throw invalid_argument("Error: Could not open file!");
	}

	MyString buffer;
	while (is.peek() != EOF)
	{
		buffer.getline(is);
		MyVector<MyString> tokens = buffer.split('|');

		User* user = findUser(tokens[0], users);
		Chat* chat = findChatById(tokens[1].toInt(), chats);

		if (!user)
		{
			throw runtime_error("User cannot be null!");
		}

		if (!chat)
		{
			throw runtime_error("Chat cannot be null!");
		}

		if (GroupChat* groupChat = dynamic_cast<GroupChat*>(chat))
		{
			groupChat->getUsersAwaitingApproval().push_back(user);
		}
	}

	is.close();
}

void TextFileHandler::saveUserChatToFile(const MyString& username, int chatId) {
	ofstream os(usersChatsFile, ios::app);

	if (!os.is_open())
	{
		throw invalid_argument("Error: Could not open file!");
	}

	os << username << "|" << chatId << endl;
	os.close();
}
void TextFileHandler::loadUserChats(MyVector<User*>& users, MyVector<Chat*>& chats) {
	ifstream is(usersChatsFile);

	if (!is.is_open())
	{
		throw invalid_argument("Error: Could not open file!");
	}

	MyString buffer;
	while (is.peek() != EOF)
	{
		buffer.getline(is);
		MyVector<MyString> tokens = buffer.split('|');

		User* user = findUser(tokens[0], users);
		Chat* chat = findChatById(tokens[1].toInt(), chats);

		if (!user)
		{
			throw runtime_error("User cannot be null!");
		}

		if (!chat)
		{
			throw runtime_error("Chat cannot be null!");
		}

		user->addChat(chat);
		chat->addParticipant(user);
	}

	is.close();
}

void TextFileHandler::deleteChatIdFromFile(int chatId) {
	ifstream is(chatIdsFile);

	if (!is.is_open())
	{
		throw invalid_argument("Error: Could not open fil!");
	}

	ofstream os("temp.txt");

	if (!os.is_open())
	{
		throw invalid_argument("Error: Could not open fil!");
	}

	MyString buffer;
	while (is.peek() != EOF)
	{
		buffer.getline(is);

		if (buffer.toInt() == chatId)
		{
			continue;
		}

		os << buffer << endl;
	}

	is.close();
	os.close();

	remove(chatIdsFile);
	rename("temp.txt", chatIdsFile);
}
void TextFileHandler::deleteChatFromFile(int chatId) {
	MyString fileName = chatsFile;
	fileName += chatId;
	fileName += ".txt";

	remove(fileName.get());
}
void TextFileHandler::deleteUserFromFile(const MyString& username) {
	ifstream is(usersFile);

	if (!is.is_open())
	{
		throw invalid_argument("Error: Could not open fil!");
	}

	ofstream os("temp.txt");

	if (!os.is_open())
	{
		throw invalid_argument("Error: Could not open fil!");
	}

	MyString buffer;
	while (is.peek() != EOF)
	{
		buffer.getline(is);
		MyVector<MyString> tokens = buffer.split('|');

		if (tokens[1] == username)
		{
			continue;
		}

		os << buffer << endl;
	}

	is.close();
	os.close();

	remove(usersFile);
	rename("temp.txt", usersFile);
}
void TextFileHandler::deleteUserChatRelation(const MyString& username, int chatId) {
	ifstream is(usersChatsFile);

	if (!is.is_open())
	{
		throw invalid_argument("Error: Could not open fil!");
	}

	ofstream os("temp.txt");

	if (!os.is_open())
	{
		throw invalid_argument("Error: Could not open fil!");
	}

	MyString buffer;
	while (is.peek() != EOF)
	{
		buffer.getline(is);
		MyVector<MyString> tokens = buffer.split('|');

		if (tokens[0] == username && tokens[1].toInt() == chatId)
		{
			continue;
		}

		os << tokens[0] << "|" << tokens[1] << endl;
	}

	is.close();
	os.close();

	remove(usersChatsFile);
	rename("temp.txt", usersChatsFile);
}
void TextFileHandler::deleteUserFromApprovaList(const MyString& username, int chatId) {
	ifstream is(approvalListFile);

	if (!is.is_open())
	{
		throw invalid_argument("Error: Could not open fil!");
	}

	ofstream os("temp.txt");

	if (!os.is_open())
	{
		throw invalid_argument("Error: Could not open fil!");
	}

	MyString buffer;
	while (is.peek() != EOF)
	{
		buffer.getline(is);
		MyVector<MyString> tokens = buffer.split('|');

		if (tokens[0] == username && tokens[1].toInt() == chatId)
		{
			continue;
		}

		os << tokens[0] << "|" << tokens[1] << endl;
	}

	is.close();
	os.close();

	remove(approvalListFile);
	rename("temp.txt", approvalListFile);
}