#include <iostream>

#include "User.h"
#include "UserUtils.h"
#include "ChatUtils.h"

using namespace std;

User::User(const MyString& username, const MyString& password) {
	this->username = username;
	this->password = password;
}

User::~User() {}

const MyString& User::getUsername() const {
	return username;
}

const MyString& User::getPassword() const {
	return password;
}

const MyVector<Chat*>& User::getChats() const {
	return chats;
}

void User::addChat(Chat*& chat) {
	chats.push_back(chat);
}

void User::viewChats() const {
	cout << "Chats: ";

	if (chats.getSize() == 0)
	{
		cout << " No active chats";
	}

	for (size_t i = 0; i < chats.getSize(); i++)
	{
		if (IndividualChat* chat = dynamic_cast<IndividualChat*>(chats[i]))
		{
			MyString username = chat->getParticipants()[0]->getUsername() == this->username
				? chat->getParticipants()[1]->getUsername() : chat->getParticipants()[0]->getUsername();

			cout << "Id: " << chat->getChatId() << ", " << username << " (" 
				<< chat->getMessages().getSize() << " messages)" << " ";
		}
		else if (GroupChat* chat = dynamic_cast<GroupChat*>(chats[i]))
		{
			cout << "Id: " << chat->getChatId() << ", " << chat->getChatName() 
				<< " (" << chat->getMessages().getSize() << " messages)" << " ";
		}
	}

	cout << endl;
}

void User::createIndividualChat(const MyString& username, MyVector<User*>& users, MyVector<Chat*>& chats) {
	if (this->username == username)
	{
		cout << "You cannot create a chat with yourself..." << endl;
		return;
	}

	User* user = findUser(username, users);

	if (!user)
	{
		cout << "User with such username does not exist!" << endl;
		return;
	}

	Chat* chat = findIndividualChatByUsernames(this->username, username, chats);

	if (chat)
	{
		cout << "You already have an active chat with " << username << "!" << endl;
		return;
	}

	int id = 1;

	for (size_t i = 0; i < chats.getSize(); i++)
	{
		if (id <= chats[i]->getChatId())
		{
			id = chats[i]->getChatId() + 1;
		}
	}
	
	chat = new IndividualChat(id);
	chat->addParticipant(this);
	chat->addParticipant(user);

	this->addChat(chat);
	user->addChat(chat);
	chats.push_back(chat);

	saveChatToFile(chat);
	saveChatIdToFile(chat);
	saveUserChatToFile(this->username, chat->getChatId());
	saveUserChatToFile(username, chat->getChatId());

	cout << "Chat with id " << chat->getChatId() << " successfully created!" << endl;
}