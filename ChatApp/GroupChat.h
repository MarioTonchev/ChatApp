#pragma once

#include "Chat.h"

class GroupChat : public Chat {
private:
	MyString chatName;
	bool requiresApproval;
	MyVector<User*> admins;
	MyVector<User*> awaitingApproval;
public:
	GroupChat(int chatId, const MyString& chatName);
	GroupChat(int chatId, const MyString& chatName, bool approval);

	const MyString& getChatName() const;
	bool getRequiresApproval() const;
	MyVector<User*>& getAdmins();
	MyVector<User*>& getUsersAwaitingApproval();

	void setApproval(bool approval);
	void addAdmin(User* user);
	void removeAdmin(User* user) override;
	void removeUserFromApprovalList(User* u) override;

	void printChat() const override;
}; 