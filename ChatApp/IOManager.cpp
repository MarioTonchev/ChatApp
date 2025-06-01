#include "IOManager.h"

using namespace std;

void writeMyString(ofstream& os, const MyString& str) {
	int len = str.len();
	const char* data = str.get();

	os.write((const char*)&len, sizeof(len));
	os.write(data, sizeof(data));
}
void saveUserToFile(User* user, const MyString& userType) {
	ofstream os(usersFile, ios::app);

	if (!os.is_open())
	{
		throw invalid_argument("Error: Could not open file!");
	}

	os << userType << "|";

	if (userType == "Admin")
	{
		Admin* admin = dynamic_cast<Admin*>(user);
		os << admin->getAdminId() << "|";
	}

	os << user->getUsername() << "|" << user->getPassword() << endl;

	os.close();
}
void saveUserToBinFile(User* user, const MyString& userType) {
	//finish this
	ofstream os(usersFileBin, ios::binary | ios::app);

	if (!os.is_open())
	{
		throw invalid_argument("File does not exist!");
	}

	writeMyString(os, userType);

	if (userType == "Admin")
	{
		Admin* admin = dynamic_cast<Admin*>(user);
		int adminId = admin->getAdminId();
		os.write((const char*)&adminId, sizeof(adminId));
	}

	writeMyString(os, user->getUsername());
	writeMyString(os, user->getPassword());

	os.close();
}

void saveChatToFile(Chat* chat) {
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

	MyString chatType = getChatType(chat);

	if (chatType == "GroupChat")
	{
		GroupChat* groupChat = dynamic_cast<GroupChat*>(chat);

		os << "GroupChat" << "|" << groupChat->getChatName() << "|";
	}
	else
	{
		os << "IndividualChat" << "|";
	}

	os << chat->getChatId() << endl;

	for (size_t i = 0; i < chat->getMessages().getSize(); i++)
	{
		os << chat->getMessages()[i].getSender() << "|" << chat->getMessages()[i].getContent() << "|"
			<< chat->getMessages()[i].getDate() << "|" << chat->getMessages()[i].getTime() << endl;
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
void saveUserChatToFile(const MyString& username, int chatId) {
	ofstream os(usersChatsFile, ios::app);

	if (!os.is_open())
	{
		throw invalid_argument("Error: Could not open file!");
	}

	os << username << "|" << chatId << endl;
	os.close();
}

MyString readMyString(ifstream& is) {
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
			Admin* admin = new Admin(tokens[1].toInt(), tokens[2], tokens[3]);
			users.push_back(admin);
		}
	}

	is.close();
}
void loadUsersBin(MyVector<User*>& users) {
	//finish this
	ifstream is(usersFileBin, ios::binary);

	if (!is.is_open())
	{
		throw invalid_argument("File does not exist");
	}

	MyString buffer;
	while (is.peek() != EOF)
	{
		MyString userType = readMyString(is);

		int adminId = 0;
		if (userType == "Admin")
		{
			is.read((char*)adminId, sizeof(adminId));
		}

		MyString username = readMyString(is);
		MyString password = readMyString(is);

		size_t chatCount;
		is.read(reinterpret_cast<char*>(&chatCount), sizeof(chatCount));
		//LOAD CHATS LATER

		User* user;

		if (userType == "Admin")
		{
			user = new Admin(adminId, username, password);
		}
		else if (userType == "RegularUser")
		{
			user = new RegularUser(username, password);
		}

		users.push_back(user);
	}

	is.close();
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
void loadChats(MyVector<Chat*>& chats) {
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
			chat = new GroupChat(tokens[2].toInt(), tokens[1]);
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