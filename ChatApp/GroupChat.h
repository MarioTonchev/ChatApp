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
	MyVector<User*>& getAdmins();

	void setApproval(bool approval);
	void addAdmin(User* user);
	void removeAdmin(User* user);

	void printChat() const override;
}; 