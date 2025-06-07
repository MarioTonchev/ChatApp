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
	
	chat = new IndividualChat(Chat::createChatId(chats));
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

	cout << endl << "You can choose one of the following commands (type 'cancel' to stop):" << endl;

	GroupChat* groupChat = dynamic_cast<GroupChat*>(chat);

	if (groupChat)
	{
		user = findUser(username, groupChat->getAdmins());

		if (user)
		{
			cout << "toggle-add-approval" << endl;
			cout << "view-add-requests" << endl;
			cout << "approve <username>" << endl;
		}
	}
	
	cout << "send-message" << endl;


	MyString command;
	command.getline(cin);

	MyVector<MyString> tokens = command.split(' ');

	if (command == "toggle-add-approval")
	{
		groupChat->setApproval(!(groupChat->getRequiresApproval()));
		saveChatToFile(groupChat);

		groupChat->getRequiresApproval() ? cout << "Adding approval has been turned on!" << endl 
			: cout << "Adding approval has been turned off!";
	}
	else if (command == "view-add-requests")
	{
		cout << "Users awaiting approval: ";

		if (groupChat->getUsersAwaitingApproval().getSize() == 0)
		{
			cout << "none";
		}

		for (size_t i = 0; i < groupChat->getUsersAwaitingApproval().getSize(); i++)
		{
			cout << groupChat->getUsersAwaitingApproval()[i]->getUsername() << " ";
		}

		cout << endl;
	}
	else if (tokens[0] == "approve" && tokens.getSize() >= 2)
	{
		user = findUser(tokens[1], groupChat->getUsersAwaitingApproval());

		if (!user)
		{
			cout << "User " << tokens[1] << " does not exist in the approval list!" << endl;
			return;
		}

		for (size_t i = 0; i < groupChat->getUsersAwaitingApproval().getSize(); i++)
		{
			if (user->getUsername() == groupChat->getUsersAwaitingApproval()[i]->getUsername())
			{
				groupChat->getUsersAwaitingApproval().remove_at(i);
				break;
			}
		}

		deleteUserFromApprovaList(tokens[1], chatId);

		user->addChat(groupChat);
		groupChat->addParticipant(user);

		saveUserChatToFile(user->getUsername(), chatId);

		cout << "User " << user->getUsername() << " has been approved and was successfully added to the group chat!" << endl;
	}
	else if (command == "send-message")
	{
		cout << "Enter message (type 'cancel' to stop):" << endl;
		command.getline(cin);

		Message message(username, command);

		chat->addMessage(message);
		saveChatToFile(chat);

		cout << "Message sent successfully!" << endl;
	}
	else if (command == "cancel")
	{
		clearConsole();
		user = findUser(this->username, groupChat->getParticipants());
		MyString userType = getUserType(user);
		printActions(userType);
	}
}

void User::createGroupChat(const MyString& groupName, MyVector<MyString>& usernames, MyVector<User*>& users, MyVector<Chat*>& chats) {
	Chat* chat = findGroupChatByName(groupName, chats);

	if (chat)
	{
		cout << "Chat with this name already exists!" << endl;
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

	GroupChat* groupChat = dynamic_cast<GroupChat*>(chat);

	if (!groupChat)
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

	groupChat->removeParticipant(this);

	if (groupChat->getParticipants().getSize() == 0)
	{
		deleteUserChatRelation(username, chatId);
		
		cout << "You successfully left chat with id " << chatId << "." << endl;
		
		deleteChat(groupChat, chats);
		cout << "Group chat with id " << groupChat->getChatId()
			<< " was deleted because it had 0 users left!" << endl;

		return;
	}

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

	saveChatToFile(groupChat);
	deleteUserChatRelation(username, chatId);

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

	Chat* chat = findChatById(chatId, this->chats);

	if (!chat)
	{
		cout << "You aren't in any group chat with id " << chatId << "." << endl;
		return;
	}

	GroupChat* groupChat = dynamic_cast<GroupChat*>(chat);

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
		saveUserChatToFile(username, chatId);

		cout << "User " << username << " has been successfully added to the group chat!" << endl;
	}
	else
	{
		groupChat->getUsersAwaitingApproval().push_back(user);
		saveUserToApprovalList(username, chatId);

		cout << "User " << username << " has been added to the waiting for join approval list!" << endl;
	}
}

void User::setGroupAdmin(int chatId, const MyString& username) {
	Chat* chat = findChatById(chatId, this->chats);

	if (!chat)
	{
		cout << "You aren't in any group chat with id " << chatId << "." << endl;
		return;
	}

	GroupChat* groupChat = dynamic_cast<GroupChat*>(chat);

	if (!groupChat)
	{
		cout << "You aren't in any group chat with id " << chatId << "." << endl;
		return;
	}

	User* user = findUser(this->username, groupChat->getAdmins());

	if (!user)
	{
		cout << "You are not an admin of group chat "
			<< groupChat->getChatName() << "! Only group admins can make other participants admins." << endl;
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
	saveChatToFile(groupChat);

	cout << "Successfully made user " << username << " admin of '" << groupChat->getChatName() << "'!" << endl;
}

void User::kickFromGroup(int chatId, const MyString& username) {
	Chat* chat = findChatById(chatId, this->chats);

	if (!chat)
	{
		cout << "You aren't in any group chat with id " << chatId << "." << endl;
		return;
	}

	GroupChat* groupChat = dynamic_cast<GroupChat*>(chat);

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

	saveChatToFile(chat);
	deleteUserChatRelation(username, chatId);

	cout << "Successfully kicked user " << username << " from group chat '" << groupChat->getChatName() << "'!" << endl;
}

void User::checkGroupStats(int chatId) {
	Chat* chat = findChatById(chatId, this->chats);

	if (!chat)
	{
		cout << "You aren't in any group chat with id " << chatId << "." << endl;
		return;
	}

	GroupChat* groupChat = dynamic_cast<GroupChat*>(chat);

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

		for (size_t i = 0; i < groupChat->getParticipants().getSize(); i++)
		{
			int curr = groupChat->getMessageCount(groupChat->getParticipants()[i]->getUsername());

			if (curr > topMessagesCount)
			{
				topMessagesCount = curr;
				topUser = groupChat->getParticipants()[i]->getUsername();
			}
		}

		cout << ", Top: " << topUser << " (" << topMessagesCount << " msg)" << endl;
	}
	else
	{
		cout << endl;
	}
}