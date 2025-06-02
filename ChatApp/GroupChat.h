#pragma once

#include "Chat.h"

class GroupChat : public Chat {
private:
	MyString chatName;
	bool requiresApproval;
	MyVector<User*> admins;
public:
	GroupChat(int chatId, const MyString& chatName);
	GroupChat(int chatId, const MyString& chatName, bool approval);

	const MyString& getChatName() const;
	bool getRequiresApproval() const;
	const MyVector<User*>& getAdmins() const;

	void setApproval(bool approval);
	void addAdmin(User* user);

	void printChat() const override;
}; 