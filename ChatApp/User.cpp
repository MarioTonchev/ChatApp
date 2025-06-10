#include <iostream>

#include "User.h"
#include "UserUtils.h"
#include "ChatUtils.h"
#include "FileHandler.h"

using namespace std;

User::User(const MyString& username, const MyString& password, FileHandler* fileHandler) {
	this->username = username;
	this->password = password;

	if (fileHandler != nullptr)
	{
		this->fileHandler = fileHandler;
	}
	else
	{
		throw invalid_argument("File handler cannot be null!");
	}
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

void User::addChat(Chat* chat) {
	chats.push_back(chat);
}

void User::removeChat(Chat* chat) {
	for (size_t i = 0; i < chats.getSize(); i++)
	{
		if (chats[i]->getChatId() == chat->getChatId())
		{
			chats.remove_at(i);
			break;
		}
	}
}

void User::viewChats() const {
	cout << "Chats: ";

	if (chats.getSize() == 0)
	{
		cout << " No active chats";
	}
	else
	{
		cout << endl;
	}

	for (size_t i = 0; i < chats.getSize(); i++)
	{
		chats[i]->printChatInfo();
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

	IndividualChat* chat = findIndividualChatByUsernames(this->username, username, chats);

	if (chat)
	{
		cout << "You already have an active chat with " << username << "!" << endl;
		return;
	}

	chat = new IndividualChat(Chat::createChatId(chats));
	chat->addParticipant(this);
	chat->addParticipant(user);

	this->addChat(chat);
	user->addChat(chat);
	chats.push_back(chat);

	fileHandler->saveChatToFile(chat);
	fileHandler->saveChatIdToFile(chat);
	fileHandler->saveUserChatToFile(this->username, chat->getChatId());
	fileHandler->saveUserChatToFile(username, chat->getChatId());

	cout << "Chat with id " << chat->getChatId() << " successfully created!" << endl;
}

void User::selectChat(int chatId) {
	Chat* chat = findChatById(chatId, this->chats);

	if (!chat)
	{
		cout << "You aren't in any chat with id " << chatId << "!" << endl;
		return;
	}

	chat->printChatMessages();
	
	cout << endl << "Enter message (type 'cancel' to stop):" << endl;

	MyString input;
	input.getline(cin);
	
	if (input != "cancel")
	{
		Message message(this->username, input);
		chat->addMessage(message);

		MyString chatType = getChatType(chat);
		
		if (chatType == "IndividualChat")
		{
			fileHandler->saveChatToFile((IndividualChat*)chat);
		}
		else if (chatType == "GroupChat")
		{
			fileHandler->saveChatToFile((GroupChat*)chat);
		}

		cout << "Message sent successfully!" << endl;
	}
	else
	{
		clearConsole();
		this->printActions();
	}
}

void User::createGroupChat(const MyString& groupName, MyVector<MyString>& usernames, MyVector<User*>& users, MyVector<Chat*>& chats) {
	GroupChat* chat = findGroupChatByName(groupName, chats);

	if (chat)
	{
		cout << "Group chat with this name already exists!" << endl;
		return;
	}

	chat = new GroupChat(Chat::createChatId(chats), groupName);

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
			for (size_t j = 2; j < i; j++)
			{
				User* user = findUser(usernames[j], users);

				user->removeChat(chat);
				chat->removeParticipant(user);
			}

			cout << "One of these users does not exist!" << endl;

			return;
		}

		user->addChat(chat);
		chat->addParticipant(user);
	}

	this->addChat(chat);
	chat->addParticipant(this);

	chat->addAdmin(this);
	chats.push_back(chat);

	fileHandler->saveChatToFile(chat);
	fileHandler->saveChatIdToFile(chat);
	fileHandler->saveUserChatToFile(username, chat->getChatId());

	for (size_t i = 2; i < usernames.getSize(); i++)
	{
		fileHandler->saveUserChatToFile(usernames[i], chat->getChatId());
	}

	cout << "Group chat with name '" << chat->getChatName() << "' successfully created!" << endl;
}

void User::leaveGroupChat(int chatId, MyVector<Chat*>& chats) {
	GroupChat* groupChat = findGroupChatById(chatId, this->chats);

	if (!groupChat)
	{
		cout << "You aren't in a group chat with such id!" << endl;
		return;
	}

	this->removeChat(groupChat);
	groupChat->removeParticipant(this);

	if (groupChat->getParticipants().getSize() == 0)
	{
		fileHandler->deleteUserChatRelation(username, chatId);

		cout << "You successfully left chat with id " << chatId << "." << endl;

		deleteChat(groupChat, chats, fileHandler);

		cout << "Group chat with id " << groupChat->getChatId()
			<< " was deleted because it had 0 users left!" << endl;

		return;
	}

	groupChat->removeAdmin(this);

	if (groupChat->getAdmins().getSize() == 0)
	{
		groupChat->setApproval(false);
	}

	fileHandler->saveChatToFile(groupChat);
	fileHandler->deleteUserChatRelation(username, chatId);

	cout << "You successfully left chat with id " << chatId << "." << endl;
}

void User::addToGroup(int chatId, const MyString& username, MyVector<User*>& users) {
	if (username == "")
	{
		cout << "Username field cannot be empty!" << endl;
		return;
	}

	if (this->username == username)
	{
		cout << "You can't add yourself to a group chat!" << endl;
		return;
	}

	GroupChat* groupChat = findGroupChatById(chatId, this->chats);

	if (!groupChat)
	{
		cout << "You aren't in any group chat with id " << chatId << "." << endl;
		return;
	}

	User* user = findUser(username, groupChat->getParticipants());

	if (user)
	{
		cout << "User " << username << " is already part of this group chat!" << endl;
		return;
	}

	user = findUser(username, users);

	if (!user)
	{
		cout << "User " << username << " does not exist!" << endl;
		return;
	}

	if (groupChat->getRequiresApproval() == false)
	{
		user->addChat(groupChat);
		groupChat->addParticipant(user);
		fileHandler->saveUserChatToFile(username, chatId);

		cout << "User " << username << " has been successfully added to the group chat!" << endl;
	}
	else
	{
		groupChat->addUserToApprovalList(user);
		fileHandler->saveUserToChatApprovalList(username, chatId);

		cout << "User " << username << " has been added to the waiting for join approval list!" << endl;
	}
}

void User::kickFromGroup(int chatId, const MyString& username) {
	GroupChat* groupChat = findGroupChatById(chatId, this->chats);

	if (!groupChat)
	{
		cout << "You aren't in any group chat with id " << chatId << "." << endl;
		return;
	}

	User* user = findUser(this->username, groupChat->getAdmins());

	if (!user)
	{
		cout << "You are not an admin of group chat "
			<< groupChat->getChatName() << "! Only group admins can kick other participants." << endl;
		return;
	}

	if (this->username == username)
	{
		cout << "You can't kick yourself!" << endl;
		return;
	}

	user = findUser(username, groupChat->getParticipants());

	if (!user)
	{
		cout << "User " << username << " is not part of group chat '" << groupChat->getChatName() << "'." << endl;
		return;
	}

	user->removeChat(groupChat);
	groupChat->removeParticipant(user);
	groupChat->removeAdmin(user);

	fileHandler->saveChatToFile(groupChat);
	fileHandler->deleteUserChatRelation(username, chatId);

	cout << "Successfully kicked user " << username << " from group chat '" << groupChat->getChatName() << "'!" << endl;
}

void User::setGroupAdmin(int chatId, const MyString& username) {
	GroupChat* groupChat = findGroupChatById(chatId, this->chats);

	if (!groupChat)
	{
		cout << "You aren't in any group chat with id " << chatId << "." << endl;
		return;
	}

	User* user = findUser(this->username, groupChat->getAdmins());

	if (!user)
	{
		cout << "You are not an admin of group chat '"
			<< groupChat->getChatName() << "'! Only group admins can make other participants admins." << endl;
		return;
	}

	if (this->username == username)
	{
		cout << "You can't make yourself admin!" << endl;
		return;
	}

	user = findUser(username, groupChat->getAdmins());

	if (user)
	{
		cout << "User " << username << " is already an admin of group chat '" << groupChat->getChatName() << "'." << endl;
		return;
	}

	user = findUser(username, groupChat->getParticipants());

	if (!user)
	{
		cout << "User " << username << " is not part of group chat '" << groupChat->getChatName() << "'." << endl;
		return;
	}

	groupChat->addAdmin(user);
	fileHandler->saveChatToFile(groupChat);

	cout << "Successfully made user " << username << " admin of '" << groupChat->getChatName() << "'!" << endl;
}

void User::toggleAddApproval(int chatId) {
	GroupChat* groupChat = findGroupChatById(chatId, this->chats);

	if (!groupChat)
	{
		cout << "You aren't in any group chat with id " << chatId << "." << endl;
		return;
	}

	User* user = findUser(this->username, groupChat->getAdmins());

	if (!user)
	{
		cout << "You are not an admin of group chat '"
			<< groupChat->getChatName() << "'! Only group admins can turn on and off adding approval." << endl;
		return;
	}

	groupChat->setApproval(!(groupChat->getRequiresApproval()));
	fileHandler->saveChatToFile(groupChat);

	if (groupChat->getRequiresApproval())
	{
		cout << "Group chat '" << groupChat->getChatName() << "' has enabled adding approval!" << endl;
	}
	else
	{
		cout << "Group chat '" << groupChat->getChatName() << "' has disabled adding approval!" << endl;
	}
}

void User::viewAddRequests(int chatId) {
	GroupChat* groupChat = findGroupChatById(chatId, chats);

	if (!groupChat)
	{
		cout << "You aren't in any group chat with id " << chatId << "." << endl;
		return;
	}

	cout << "Users awaiting approval in group chat '" << groupChat->getChatName() << "': ";

	if (groupChat->getUsersAwaitingApproval().getSize() == 0)
	{
		cout << " None";
	}

	for (size_t i = 0; i < groupChat->getUsersAwaitingApproval().getSize(); i++)
	{
		cout << groupChat->getUsersAwaitingApproval()[i]->getUsername() << " ";
	}

	cout << endl;
}

void User::approveAddRequest(int chatId, const MyString& username) {
	GroupChat* groupChat = findGroupChatById(chatId, this->chats);

	if (!groupChat)
	{
		cout << "You aren't in any group chat with id " << chatId << "." << endl;
		return;
	}

	User* user = findUser(this->username, groupChat->getAdmins());

	if (!user)
	{
		cout << "You are not an admin of group chat '"
			<< groupChat->getChatName() << "'! Only group admins can approve add requests." << endl;
		return;
	}

	user = findUser(username, groupChat->getUsersAwaitingApproval());

	if (!user)
	{
		cout << "User " << username << " is not in the group chat's awaiting approval list." << endl;
		return;
	}

	groupChat->removeUserFromApprovalList(user);
	user->addChat(groupChat);
	groupChat->addParticipant(user);

	fileHandler->deleteUserFromApprovaList(username, chatId);
	fileHandler->saveUserChatToFile(user->getUsername(), chatId);

	cout << "User " << username << " has been approved and was successfully added to the group chat!" << endl;
}

void User::checkGroupStats(int chatId) {
	GroupChat* groupChat = findGroupChatById(chatId, this->chats);

	if (!groupChat)
	{
		cout << "You aren't in any group chat with id " << chatId << "." << endl;
		return;
	}

	cout << groupChat->getChatName() << ": " << groupChat->getParticipants().getSize()
		<< " members, " << groupChat->getMessages().getSize() << " messages";

	if (groupChat->getMessages().getSize() > 0)
	{
		MyString topUser = groupChat->getParticipants()[0]->getUsername();
		int topMessagesCount = groupChat->getMessageCount(topUser);

		for (size_t i = 1; i < groupChat->getParticipants().getSize(); i++)
		{
			MyString currentUser = groupChat->getParticipants()[i]->getUsername();
			int currMessagesCount = groupChat->getMessageCount(currentUser);

			if (currMessagesCount > topMessagesCount)
			{
				topMessagesCount = currMessagesCount;
				topUser = currentUser;
			}
		}

		cout << ", Top: " << topUser << " (" << topMessagesCount << " msg)" << endl;
	}
	else
	{
		cout << endl;
	}
}