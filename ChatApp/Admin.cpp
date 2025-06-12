#include <iostream>

#include "Admin.h"

using namespace std;

Admin::Admin(int adminId, const MyString& username, const MyString& password, FileHandler* fileHandler) : User(username, password, fileHandler) {
	this->adminId = adminId;
}

int Admin::getAdminId() const {
	return adminId;
}

int Admin::createAdminId(MyVector<User*>& users) {
	int id = 1;

	for (size_t i = 0; i < users.getSize(); i++)
	{
		if (Admin* admin = dynamic_cast<Admin*>(users[i]))
		{
			if (id <= admin->getAdminId())
			{
				id = admin->getAdminId() + 1;
			}
		}
	}

	return id;
}

void Admin::deleteUser(const MyString& username, MyVector<User*>& users, MyVector<Chat*>& chats) {
	if (this->username == username)
	{
		cout << "You can't delete yourself..." << endl;
		return;
	}

	User* user = findUser(username, users);

	if (!user)
	{
		cout << "User " << username << " does not exist!" << endl;
		return;
	}

	for (size_t i = 0; i < user->getChats().getSize(); i++)
	{
		Chat* currChat = user->getChats()[i];
		MyString chatType = getChatType(currChat);

		currChat->removeParticipant(user);

		if (GroupChat* groupChat = dynamic_cast<GroupChat*>(currChat))
		{
			groupChat->removeAdmin(user);
			groupChat->removeUserFromApprovalList(user);
		}

		fileHandler->deleteUserChatRelation(username, currChat->getChatId());

		if (currChat->getParticipants().getSize() == 0)
		{
			cout << "Group chat with id " << currChat->getChatId()
				<< " was deleted because it had 0 users left!" << endl;

			deleteChat(currChat, chats, fileHandler);
		}
		else if (chatType == "IndividualChat" && currChat->getParticipants().getSize() == 1)
		{
			cout << "Individual chat with id " << currChat->getChatId()
				<< " was deleted because it had only 1 user left!" << endl;

			deleteChat(currChat, chats, fileHandler);
		}
		else
		{
			if (chatType == "IndividualChat")
			{
				fileHandler->saveChatToFile((IndividualChat*)currChat);
			}
			else if (chatType == "GroupChat")
			{
				fileHandler->saveChatToFile((GroupChat*)currChat);
			}
		}
	}

	for (size_t i = 0; i < users.getSize(); i++)
	{
		if (users[i]->getUsername() == username)
		{
			users.remove_at(i);
			break;
		}
	}

	fileHandler->deleteUserFromFile(username);

	delete user;

	cout << "User " << username << " has been successfully deleted!" << endl;
}

void Admin::deleteGroupChat(int chatId, MyVector<Chat*>& chats) {
	GroupChat* groupChat = findGroupChatById(chatId, chats);

	if (!groupChat)
	{
		cout << "Group chat with id " << chatId << " does not exist!" << endl;
		return;
	}

	cout << "Group chat with id " << chatId << " has been successfully deleted!" << endl;

	deleteChat(groupChat, chats, fileHandler);
}

void Admin::viewAllChats(MyVector<Chat*>& chats) const {
	if (chats.getSize() == 0)
	{
		cout << "There are currently 0 active chats." << endl;
		return;
	}

	for (size_t i = 0; i < chats.getSize(); i++)
	{
		chats[i]->printChatMessages();
		cout << endl;
	}
}

void Admin::printActions() const {
	cout << "You are able to use the following commands:" << endl << endl;

	cout << "delete-user <username>" << endl;
	cout << "delete-group <chatId>" << endl;
	cout << "view-all-chats" << endl << endl;

	cout << "view-chats" << endl;
	cout << "select-chat <chatId>" << endl;
	cout << "create-individual-chat <username>" << endl;
	cout << "create-group <groupName> <username1> <username2> ...." << endl;
	cout << "leave-group <chatId>" << endl;
	cout << "add-to-group <chatId> <username>" << endl;
	cout << "kick-from-group <chatId> <username>" << endl;
	cout << "set-group-admin <chatId> <username>" << endl;
	cout << "group-stats <chatId>" << endl << endl;
	cout << "toggle-group-add-approval <chatId>" << endl;
	cout << "view-group-approval-list <chatId>" << endl;
	cout << "approve <chatId> <username>" << endl << endl;

	cout << "logout" << endl;
	cout << "quit" << endl;
}