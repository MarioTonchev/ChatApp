#include "BinaryFileHandler.h"

#include <iostream>
#include <fstream>
#include <stdexcept>

using namespace std;

void BinaryFileHandler::writeMyStringBin(ofstream& os, const MyString& str) {
	int len = str.len();
	const char* data = str.get();

	os.write((const char*)&len, sizeof(len));
	os.write(data, len);
}
MyString BinaryFileHandler::readMyStringBin(ifstream& is) {
	int len;
	is.read((char*)&len, sizeof(len));

	if (len == 0) {
		return MyString();
	}

	char* buffer = new char[len + 1];
	is.read(buffer, len);
	buffer[len] = '\0';

	MyString str(buffer);
	delete[] buffer;

	return str;
}

void BinaryFileHandler::saveUserToFile(RegularUser* user) {
	ofstream os(usersFileBin, ios::binary | ios::app);

	if (!os.is_open())
	{
		throw invalid_argument("File does not exist!");
	}

	writeMyStringBin(os, "RegularUser");
	writeMyStringBin(os, user->getUsername());
	writeMyStringBin(os, user->getPassword());

	os.close();
}
void BinaryFileHandler::saveUserToFile(Admin* user) {
	ofstream os(usersFileBin, ios::binary | ios::app);

	if (!os.is_open())
	{
		throw invalid_argument("File does not exist!");
	}

	writeMyStringBin(os, "Admin");
	writeMyStringBin(os, user->getUsername());
	writeMyStringBin(os, user->getPassword());

	int adminId = user->getAdminId();
	os.write((const char*)&adminId, sizeof(adminId));

	os.close();
}
void BinaryFileHandler::loadUsers(MyVector<User*>& users) {
	ifstream is(usersFileBin, ios::binary);

	if (!is.is_open())
	{
		throw invalid_argument("File does not exist");
	}

	MyString buffer;
	while (is.peek() != EOF)
	{
		MyString userType = readMyStringBin(is);

		MyString username = readMyStringBin(is);
		MyString password = readMyStringBin(is);

		if (userType == "Admin")
		{
			int adminId = 0;
			is.read((char*)&adminId, sizeof(adminId));

			User* user = new Admin(adminId, username, password, this);
			users.push_back(user);
		}
		else if (userType == "RegularUser")
		{
			User* user = new RegularUser(username, password, this);
			users.push_back(user);
		}
	}

	is.close();
}

void BinaryFileHandler::saveChatToFile(IndividualChat* individualChat) {
	if (!individualChat)
	{
		throw invalid_argument("Chat cannot be null!");
	}

	MyString file = chatsFile;
	file += individualChat->getChatId();
	file += ".bin";

	ofstream os(file.get(), ios::binary | ios::trunc);

	if (!os.is_open())
	{
		throw invalid_argument("Error: Could not open file for binary write");
	}

	writeMyStringBin(os, "IndividualChat");

	int chatId = individualChat->getChatId();
	os.write((const char*)&chatId, sizeof(chatId));

	int messagesCount = individualChat->getMessages().getSize();
	os.write((char*)&messagesCount, sizeof(messagesCount));

	for (int i = 0; i < messagesCount; ++i)
	{
		const Message& message = individualChat->getMessages()[i];

		writeMyStringBin(os, message.getSender());
		writeMyStringBin(os, message.getContent());
		writeMyStringBin(os, message.getDate());
		writeMyStringBin(os, message.getTime());
	}

	os.close();
}
void BinaryFileHandler::saveChatToFile(GroupChat* groupChat) {
	if (!groupChat)
	{
		throw invalid_argument("Chat cannot be null!");
	}

	MyString file = chatsFile;
	file += groupChat->getChatId();
	file += ".bin";

	ofstream os(file.get(), ios::binary | ios::trunc);

	if (!os.is_open())
	{
		throw invalid_argument("Error: Could not open file for binary write");
	}

	writeMyStringBin(os, "GroupChat");

	int chatId = groupChat->getChatId();
	os.write((const char*)&chatId, sizeof(chatId));

	writeMyStringBin(os, groupChat->getChatName());

	int requiresApproval = groupChat->getRequiresApproval() ? 1 : 0;
	os.write((const char*)&requiresApproval, sizeof(requiresApproval));

	int adminCount = groupChat->getAdmins().getSize();
	os.write((const char*)&adminCount, sizeof(adminCount));

	for (int i = 0; i < adminCount; ++i) {
		writeMyStringBin(os, groupChat->getAdmins()[i]->getUsername());
	}

	int messagesCount = groupChat->getMessages().getSize();
	os.write((const char*)&messagesCount, sizeof(messagesCount));

	for (int i = 0; i < messagesCount; ++i)
	{
		const Message& message = groupChat->getMessages()[i];

		writeMyStringBin(os, message.getSender());
		writeMyStringBin(os, message.getContent());
		writeMyStringBin(os, message.getDate());
		writeMyStringBin(os, message.getTime());
	}

	os.close();
}
void BinaryFileHandler::loadChats(MyVector<Chat*>& chats, MyVector<User*>& users) {
	MyVector<int> chatIds = this->getChatIds();

	for (size_t i = 0; i < chatIds.getSize(); i++)
	{
		MyString file = chatsFile;
		file += chatIds[i];
		file += ".bin";

		ifstream is(file.get(), ios::binary);

		if (!is.is_open())
		{
			throw std::invalid_argument("Error: Could not open file");
		}

		MyString chatType = readMyStringBin(is);

		int chatId;
		is.read((char*)&chatId, sizeof(chatId));

		Chat* chat = nullptr;

		if (chatType == "GroupChat")
		{
			MyString name = readMyStringBin(is);

			int requiresApproval = 0;
			is.read((char*)&requiresApproval, sizeof(requiresApproval));

			int adminsCount = 0;
			is.read((char*)&adminsCount, sizeof(adminsCount));

			chat = new GroupChat(chatId, name, requiresApproval);

			for (int i = 0; i < adminsCount; ++i) {
				MyString username = readMyStringBin(is);
				User* user = findUser(username, users);

				dynamic_cast<GroupChat*>(chat)->addAdmin(user);
			}
		}
		else
		{
			chat = new IndividualChat(chatId);
		}

		if (!chat)
		{
			throw runtime_error("Chat creation failed during load");
		}

		int messagesCount = 0;
		is.read((char*)&messagesCount, sizeof(messagesCount));

		for (int i = 0; i < messagesCount; ++i) {
			MyString sender = readMyStringBin(is);
			MyString content = readMyStringBin(is);
			MyString date = readMyStringBin(is);
			MyString time = readMyStringBin(is);

			Message message(sender, content, date, time);

			chat->addMessage(message);
		}

		chats.push_back(chat);
		is.close();
	}

	this->loadChatApprovalList(chats, users);
}

void BinaryFileHandler::saveChatIdToFile(Chat* chat) {
	ofstream os(chatIdsFileBin, ios::binary | ios::app);

	if (!os.is_open())
	{
		throw invalid_argument("Error: Could not open file!");
	}

	int chatId = chat->getChatId();
	os.write((const char*)&chatId, sizeof(chatId));

	os.close();
}
MyVector<int> BinaryFileHandler::getChatIds() {
	ifstream is(chatIdsFileBin, ios::binary);

	if (!is.is_open())
	{
		throw invalid_argument("Error: Could not open file!");
	}

	MyVector<int> result;

	int chatId = 0;

	while (is.read((char*)&chatId, sizeof(chatId)))
	{
		result.push_back(chatId);
	}

	is.close();

	return result;
}

void BinaryFileHandler::saveUserToChatApprovalList(const MyString& username, int chatId) {
	ofstream os(approvalListFileBin, ios::binary | ios::app);

	if (!os.is_open())
	{
		throw invalid_argument("Error: Could not open file");
	}

	writeMyStringBin(os, username);
	os.write((char*)&chatId, sizeof(chatId));

	os.close();
}
void BinaryFileHandler::loadChatApprovalList(MyVector<Chat*>& chats, MyVector<User*>& users) {
	ifstream is(approvalListFileBin, ios::binary);

	if (!is.is_open())
	{
		throw invalid_argument("Error: Could not open file");
	}

	while (is.peek() != EOF) {
		MyString username = readMyStringBin(is);

		int chatId = 0;
		is.read((char*)&chatId, sizeof(chatId));

		User* user = findUser(username, users);
		Chat* chat = findChatById(chatId, chats);

		if (!user)
		{
			throw runtime_error("User cannot be null!");
		}

		if (!chat)
		{
			throw runtime_error("Chat cannot be null!");
		}

		if (GroupChat* groupChat = dynamic_cast<GroupChat*>(chat)) {
			groupChat->getUsersAwaitingApproval().push_back(user);
		}
	}

	is.close();
}

void BinaryFileHandler::saveUserChatToFile(const MyString& username, int chatId) {
	ofstream os(usersChatsFileBin, ios::binary | ios::app);

	if (!os.is_open())
	{
		throw invalid_argument("Error: Could not open file");
	}

	writeMyStringBin(os, username);
	os.write((char*)&chatId, sizeof(chatId));

	os.close();
}
void BinaryFileHandler::loadUserChats(MyVector<User*>& users, MyVector<Chat*>& chats) {
	ifstream is(usersChatsFileBin, std::ios::binary);

	if (!is.is_open())
	{
		throw invalid_argument("Error: Could not open file");
	}

	while (is.peek() != EOF) {
		MyString username = readMyStringBin(is);

		int chatId = 0;
		is.read((char*)&chatId, sizeof(chatId));

		User* user = findUser(username, users);
		Chat* chat = findChatById(chatId, chats);

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

void BinaryFileHandler::deleteChatIdFromFile(int chatId) {
	ifstream is(chatIdsFileBin, std::ios::binary);

	if (!is.is_open())
	{
		throw invalid_argument("Error: Could not open file");
	}

	ofstream os("temp.bin", ios::binary | ios::trunc);

	if (!os.is_open())
	{
		throw std::invalid_argument("Error: Could not open temp file");
	}

	int id = 0;

	while (is.read((char*)&id, sizeof(id)))
	{
		if (id == chatId)
		{
			continue;
		}

		os.write((const char*)&id, sizeof(id));
	}

	is.close();
	os.close();

	remove(chatIdsFileBin);
	rename("temp.bin", chatIdsFileBin);
}
void BinaryFileHandler::deleteChatFromFile(int chatId) {
	MyString fileName = chatsFile;
	fileName += chatId;
	fileName += ".bin";

	remove(fileName.get());
}
void BinaryFileHandler::deleteUserFromFile(const MyString& username) {
	ifstream is(usersFileBin, ios::binary);

	if (!is.is_open())
	{
		throw invalid_argument("Error: Could not open file!");
	}

	ofstream os("temp.bin", ios::binary);

	if (!os.is_open())
	{
		throw invalid_argument("Error: Could not open file!");
	}

	while (is.peek() != EOF)
	{
		MyString userType = readMyStringBin(is);
		MyString readUsername = readMyStringBin(is);
		MyString password = readMyStringBin(is);

		int adminId = 0;

		if (userType == "Admin")
		{
			is.read((char*)&adminId, sizeof(adminId));
		}

		if (readUsername == username)
		{
			continue;
		}

		writeMyStringBin(os, userType);
		writeMyStringBin(os, readUsername);
		writeMyStringBin(os, password);

		if (userType == "Admin")
		{
			os.write((char*)&adminId, sizeof(adminId));
		}
	}

	is.close();
	os.close();

	remove(usersFileBin);
	rename("temp.bin", usersFileBin);
}
void BinaryFileHandler::deleteUserChatRelation(const MyString& username, int chatId) {
	ifstream is(usersChatsFileBin, ios::binary);

	if (!is.is_open())
	{
		throw invalid_argument("Error: Could not open file!");
	}

	ofstream os("temp.bin", ios::binary);

	if (!os.is_open())
	{
		throw invalid_argument("Error: Could not open file!");
	}

	while (is.peek() != EOF)
	{
		MyString readUsername = readMyStringBin(is);

		int readChatId = 0;
		is.read((char*)&readChatId, sizeof(readChatId));

		if (readUsername == username && readChatId == chatId)
		{
			continue;
		}

		writeMyStringBin(os, readUsername);
		os.write((char*)&readChatId, sizeof(readChatId));
	}

	is.close();
	os.close();

	remove(usersChatsFileBin);
	rename("temp.bin", usersChatsFileBin);
}
void BinaryFileHandler::deleteUserFromApprovaList(const MyString& username, int chatId) {
	ifstream is(approvalListFileBin, ios::binary);

	if (!is.is_open())
	{
		throw invalid_argument("Error: Could not open file!");
	}

	ofstream os("temp.bin", ios::binary);

	if (!os.is_open())
	{
		throw invalid_argument("Error: Could not open file!");
	}

	while (is.peek() != EOF)
	{
		MyString readUsername = readMyStringBin(is);

		int readChatId = 0;
		is.read((char*)&readChatId, sizeof(readChatId));

		if (readUsername == username && readChatId == chatId)
		{
			continue;
		}

		writeMyStringBin(os, readUsername);
		os.write((char*)&readChatId, sizeof(readChatId));
	}

	is.close();
	os.close();

	remove(approvalListFileBin);
	rename("temp.bin", approvalListFileBin);
}