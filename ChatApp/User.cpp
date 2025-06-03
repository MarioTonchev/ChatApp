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
	if (username == "")
	{
		cout << "Username cannot be empty!" << endl;
		return;
	}

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

void User::selectChat(int chatId, MyVector<Chat*>& chats) {
	Chat* chat = findChatById(chatId, chats);

	if (!chat)
	{
		cout << "Chat with such id does not exist!" << endl;
		return;
	}

	User* user = findUser(username, chat->getParticipants());

	if (!user)
	{
		cout << "You don't have access to this chat!" << endl;
		return;
	}

	chat->printChat();

	cout << "Enter message (type 'cancel' to stop):" << endl;

	MyString command;
	command.getline(cin);

	if (command != "cancel")
	{
		Message message(username, command);

		chat->addMessage(message);
		saveChatToFile(chat);

		cout << "Message sent successfully!" << endl;
	}
}

void User::createGroupChat(const MyString& groupName, MyVector<MyString>& usernames, MyVector<User*>& users, MyVector<Chat*>& chats) {
	Chat* chat = findGroupChatByName(groupName, chats);

	if (chat)
	{
		cout << "Chat with this name already exists!" << endl;
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

	chat = new GroupChat(id, groupName);

	for (size_t i = 2; i < usernames.getSize(); i++)
	{
		if (usernames[i] == username)
		{
			cout << "You cannot add yourself!" << endl;
			return;
		}

		User* user = findUser(usernames[i], users);

		if (!user)
		{
			cout << "One of these users does not exist!" << endl;
			return;
		}

		user->addChat(chat);
		chat->addParticipant(user);
	}

	this->addChat(chat);
	chat->addParticipant(this);

	dynamic_cast<GroupChat*>(chat)->addAdmin(this);
	chats.push_back(chat);

	saveChatToFile(chat);
	saveChatIdToFile(chat);

	saveUserChatToFile(username, chat->getChatId());

	for (size_t i = 2; i < usernames.getSize(); i++)
	{
		saveUserChatToFile(usernames[i], chat->getChatId());
	}

	cout << "Group chat with name '" << dynamic_cast<GroupChat*>(chat)->getChatName() << "' successfully created!" << endl;
}

void User::leaveGroupChat(int chatId, MyVector<Chat*>& chats) {
	Chat* chat = findChatById(chatId, this->chats);

	if (!chat)
	{
		cout << "You aren't in a group chat with such id!" << endl;
		return;
	}

	MyString chatType = getChatType(chat);

	if (chatType != "GroupChat")
	{
		cout << "You aren't in a group chat with such id!" << endl;
		return;
	}

	for (size_t i = 0; i < this->chats.getSize(); i++)
	{
		if (this->chats[i]->getChatId() == chatId)
		{
			this->chats.remove_at(i);
			break;
		}
	}

	for (size_t i = 0; i < chat->getParticipants().getSize(); i++)
	{
		if (chat->getParticipants()[i]->getUsername() == username)
		{
			chat->getParticipants().remove_at(i);
		}
	}

	if (chat->getParticipants().getSize() == 0)
	{
		deleteUserChatRelation(username, chatId);
		
		cout << "You successfully left chat with id " << chatId << "." << endl;
		
		deleteChat(chat, chats);

		return;
	}

	GroupChat* groupChat = dynamic_cast<GroupChat*>(chat);

	for (size_t i = 0; i < groupChat->getAdmins().getSize(); i++)
	{
		if (groupChat->getAdmins()[i]->getUsername() == username)
		{
			groupChat->getAdmins().remove_at(i);
		}
	}

	if (groupChat->getAdmins().getSize() == 0)
	{
		groupChat->setApproval(false);
	}

	saveChatToFile(chat);
	deleteUserChatRelation(username, chatId);

	cout << "You successfully left chat with id " << chatId << "." << endl;
}