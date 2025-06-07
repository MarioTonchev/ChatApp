#include <iostream>

#include "GroupChat.h"
#include "User.h"

using namespace std;

GroupChat::GroupChat(int chatId, const MyString& chatName) {
	this->chatId = chatId;
	this->chatName = chatName;
	this->requiresApproval = false;
}

GroupChat::GroupChat(int chatId, const MyString& chatName, bool approval) {
	this->chatId = chatId;
	this->chatName = chatName;
	this->requiresApproval = approval;
}

const MyString& GroupChat::getChatName() const {
	return chatName;
}

bool GroupChat::getRequiresApproval() const {
	return requiresApproval;
}

MyVector<User*>& GroupChat::getAdmins() {
	return admins;
}

MyVector<User*>& GroupChat::getUsersAwaitingApproval() {
	return awaitingApproval;
}

void GroupChat::setApproval(bool approval) {
	requiresApproval = approval;
}

void GroupChat::addAdmin(User* user) {
	admins.push_back(user);
}

void GroupChat::removeAdmin(User* user) {
	for (size_t i = 0; i < admins.getSize(); i++)
	{
		if (admins[i]->getUsername() == user->getUsername())
		{
			admins.remove_at(i);
			break;
		}
	}
}

void GroupChat::removeUserFromApprovalList(User* user) {
	for (size_t i = 0; i < awaitingApproval.getSize(); i++)
	{
		if (awaitingApproval[i]->getUsername() == user->getUsername())
		{
			awaitingApproval.remove_at(i);
			break;
		}
	}
}

void GroupChat::printChat() const {
	cout << "Chat '" << chatName << "':" << endl;

	if (messages.getSize() == 0)
	{
		cout << "No messages yet." << endl;
		return;
	}

	for (size_t i = 0; i < messages.getSize(); i++)
	{
		cout << "[" << messages[i].getSender() << ", " << messages[i].getDate() << " " << messages[i].getTime()
			<< "] " << messages[i].getContent() << endl;
	}
}