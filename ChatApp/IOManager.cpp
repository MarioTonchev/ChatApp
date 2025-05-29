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
		throw invalid_argument("File does not exist!");
	}

	os << userType << "|";

	if (userType == "Admin")
	{
		Admin* admin = dynamic_cast<Admin*>(user);
		os << admin->getAdminId() << "|";
	}

	os << user->getUsername() << "|" << user->getPassword() << "|";

	if (user->getChats().getSize() == 0)
	{
		os << endl;
	}

	for (size_t i = 0; i < user->getChats().getSize(); i++)
	{
		os << user->getChats()[i]->getChatId();

		if (i < user->getChats().getSize() - 1)
		{
			os << ",";
		}
		else
		{
			os << endl;
		}
	}

	os.close();
}
void saveUserToBinFile(User* user, const MyString& userType) {
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

	int chatCount = user->getChats().getSize();
	os.write((const char*)&chatCount, sizeof(chatCount));

	for (size_t i = 0; i < chatCount; i++)
	{
		int chatId = user->getChats()[i]->getChatId();
		os.write((const char*)&chatId, sizeof(chatId));
	}

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
		throw invalid_argument("File does not exist");
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