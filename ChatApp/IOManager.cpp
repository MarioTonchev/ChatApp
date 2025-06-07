#include "IOManager.h"

using namespace std;

void writeMyStringBin(ofstream& os, const MyString& str) {
	int len = str.len();
	const char* data = str.get();

	os.write((const char*)&len, sizeof(len));
	os.write(data, len);
}
MyString readMyStringBin(ifstream& is) {
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

void saveUserToFile(User* user, const MyString& userType) {
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
void saveUserToBinFile(User* user, const MyString& userType) {
	ofstream os(usersFileBin, ios::binary | ios::app);

	if (!os.is_open())
	{
		throw invalid_argument("File does not exist!");
	}

	writeMyStringBin(os, userType);
	writeMyStringBin(os, user->getUsername());
	writeMyStringBin(os, user->getPassword());

	if (userType == "Admin")
	{
		Admin* admin = dynamic_cast<Admin*>(user);
		int adminId = admin->getAdminId();
		os.write((const char*)&adminId, sizeof(adminId));
	}

	os.close();
}

void loadUsers(MyVector<User*>& users) {
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
			RegularUser* user = new RegularUser(tokens[1], tokens[2]);
			users.push_back(user);
		}
		else if (tokens[0] == "Admin")
		{
			Admin* admin = new Admin(tokens[3].toInt(), tokens[1], tokens[2]);
			users.push_back(admin);
		}
	}

	is.close();
}
void loadUsersBin(MyVector<User*>& users) {
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

			User* user = new Admin(adminId, username, password);
			users.push_back(user);
		}
		else if (userType == "RegularUser")
		{
			User* user = new RegularUser(username, password);
			users.push_back(user);
		}
	}

	is.close();
}

void saveChatToFile(Chat* chat, const MyString& chatType) {
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
void saveChatToBinFile(Chat* chat, const MyString& chatType) {
	if (!chat)
	{
		throw invalid_argument("Chat cannot be null!");
	}

	MyString file = chatsFile;
	file += chat->getChatId();
	file += ".bin";

	ofstream os(file.get(), ios::binary | ios::trunc);

	if (!os.is_open())
	{
		throw invalid_argument("Error: Could not open file for binary write");
	}

	writeMyStringBin(os, chatType);

	int chatId = chat->getChatId();
	os.write((const char*)&chatId, sizeof(chatId));

	if (chatType == "GroupChat")
	{
		GroupChat* groupChat = dynamic_cast<GroupChat*>(chat);

		writeMyStringBin(os, groupChat->getChatName());

		int flag = groupChat->getRequiresApproval() ? 1 : 0;
		os.write((char*)&flag, sizeof(flag));

		int adminCount = groupChat->getAdmins().getSize();
		os.write((char*)&adminCount, sizeof(adminCount));

		for (int i = 0; i < adminCount; ++i) {
			writeMyStringBin(os, groupChat->getAdmins()[i]->getUsername());
		}
	}

	int messagesCount = chat->getMessages().getSize();
	os.write((char*)&messagesCount, sizeof(messagesCount));

	for (int i = 0; i < messagesCount; ++i)
	{
		const Message& message = chat->getMessages()[i];

		writeMyStringBin(os, message.getSender());
		writeMyStringBin(os, message.getContent());
		writeMyStringBin(os, message.getDate());
		writeMyStringBin(os, message.getTime());
	}

	os.close();
}

void saveChatIdToFile(Chat* chat) {
	ofstream os(chatIdsFile, ios::app);

	if (!os.is_open())
	{
		throw invalid_argument("Error: Could not open file!");
	}

	os << chat->getChatId() << endl;
	os.close();
}
void saveChatIdToBinFile(Chat* chat) {
	ofstream os(chatIdsFileBin, ios::binary | ios::app);

	if (!os.is_open())
	{
		throw invalid_argument("Error: Could not open file!");
	}

	int chatId = chat->getChatId();
	os.write((const char*)&chatId, sizeof(chatId));

	os.close();
}

MyVector<int> getChatIds() {
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
MyVector<int> getChatIdsBin() {
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

void saveUserToChatApprovalList(const MyString& username, int chatId) {
	ofstream os(approvalListFile, ios::app);

	if (!os.is_open())
	{
		throw invalid_argument("Error: Could not open file!");
	}

	os << username << "|" << chatId << endl;
	os.close();
}
void saveUserToChatApprovalListBin(const MyString& username, int chatId) {
	ofstream os(approvalListFileBin, ios::binary | ios::app);

	if (!os.is_open())
	{
		throw invalid_argument("Error: Could not open file");
	}

	writeMyStringBin(os, username);
	os.write((char*)&chatId, sizeof(chatId));

	os.close();
}

void loadChatApprovalList(MyVector<Chat*>& chats, MyVector<User*>& users) {
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
void loadChatApprovalListBin(MyVector<Chat*>& chats, MyVector<User*>& users) {
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

void loadChats(MyVector<Chat*>& chats, MyVector<User*>& users) {
	MyVector<int> chatIds = getChatIds();

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

	loadChatApprovalList(chats, users);
}
void loadChatsBin(MyVector<Chat*>& chats, MyVector<User*>& users) {
	MyVector<int> chatIds = getChatIdsBin();

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

			int flag = 0;
			is.read((char*)&flag, sizeof(flag));

			int adminsCount = 0;
			is.read((char*)&adminsCount, sizeof(adminsCount));

			chat = new GroupChat(chatId, name, flag);

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

	loadChatApprovalListBin(chats, users);
}

void saveUserChatToFile(const MyString& username, int chatId) {
	ofstream os(usersChatsFile, ios::app);

	if (!os.is_open())
	{
		throw invalid_argument("Error: Could not open file!");
	}

	os << username << "|" << chatId << endl;
	os.close();
}
void saveUserChatToBinFile(const MyString& username, int chatId) {
	ofstream os(usersChatsFileBin, ios::binary | ios::app);

	if (!os.is_open())
	{
		throw invalid_argument("Error: Could not open file");
	}

	writeMyStringBin(os, username);
	os.write((char*)&chatId, sizeof(chatId));

	os.close();
}

void loadUserChats(MyVector<User*>& users, MyVector<Chat*>& chats) {
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
void loadUserChatsBin(MyVector<User*>& users, MyVector<Chat*>& chats) {
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

void deleteChatIdFromFile(int chatId) {
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
void deleteChatIdFromBinFile(int chatId) {
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

void deleteChatFromFile(int chatId) {
	MyString fileName = chatsFile;
	fileName += chatId;
	fileName += ".txt";

	remove(fileName.get());
}
void deleteChatFromBinFile(int chatId) {
	MyString fileName = chatsFile;
	fileName += chatId;
	fileName += ".bin";

	remove(fileName.get());
}

void deleteUserFromFile(const MyString& username) {
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
void deleteUserFromBinFile(const MyString& username) {
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

void deleteUserChatRelation(const MyString& username, int chatId) {
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
void deleteUserChatRelationBin(const MyString& username, int chatId) {
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

void deleteUserFromApprovaList(const MyString& username, int chatId) {
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
void deleteUserFromApprovaListBin(const MyString& username, int chatId) {
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