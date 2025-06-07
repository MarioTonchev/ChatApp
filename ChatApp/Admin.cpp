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

		currChat->removeParticipant(user);
		currChat->removeAdmin(user);
		currChat->removeUserFromApprovalList(user);

		fileHandler->deleteUserChatRelation(username, currChat->getChatId());

		if (currChat->getParticipants().getSize() == 0)
		{
			deleteChat(currChat, chats, fileHandler);
			cout << "Group chat with id " << currChat->getChatId()
				<< " was deleted because it had 0 users left!" << endl;
		}
		else if (getChatType(currChat) == "IndividualChat" && currChat->getParticipants().getSize() == 1)
		{
			deleteChat(currChat, chats, fileHandler);

			MyString otherParticipantUsername = currChat->getParticipants()[0]->getUsername() == username ?
				currChat->getParticipants()[1]->getUsername() : currChat->getParticipants()[0]->getUsername();

			User* otherParticipant = findUser(otherParticipantUsername, users);
			otherParticipant->removeChat(currChat);

			fileHandler->deleteUserChatRelation(otherParticipantUsername, currChat->getChatId());

			cout << "Individual chat with id " << currChat->getChatId()
				<< " was deleted because it had only 1 user left!" << endl;
		}
		else
		{
			fileHandler->saveChatToFile(user->getChats()[i], getChatType(user->getChats()[i]));
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

	cout << "User " << username << " has been successfully deleted!" << endl;
}

void Admin::deleteGroupChat(int chatId, MyVector<Chat*>& chats) {
	Chat* chat = findChatById(chatId, chats);

	if (!chat)
	{
		cout << "Group chat with id " << chatId << " does not exist!" << endl;
		return;
	}

	GroupChat* groupChat = dynamic_cast<GroupChat*>(chat);

	if (!groupChat)
	{
		cout << "Group chat with id " << chatId << " does not exist!" << endl;
		return;
	}

	for (size_t i = 0; i < chat->getParticipants().getSize(); i++)
	{
		chat->getParticipants()[i]->removeChat(chat);
		fileHandler->deleteUserChatRelation(chat->getParticipants()[i]->getUsername(), chat->getChatId());
	}

	deleteChat(chat, chats, fileHandler);
	cout << "Group chat with id " << chatId << " has been successfully deleted!" << endl;
}

void Admin::viewAllChats(MyVector<Chat*>& chats) {
	if (chats.getSize() == 0)
	{
		cout << "There are currently 0 active chats." << endl;
		return;
	}

	for (size_t i = 0; i < chats.getSize(); i++)
	{
		chats[i]->printChat();
	}
}