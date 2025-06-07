#pragma once

#include "MyVector.hpp"
#include "Message.h"

class User;

class Chat {
protected:
	int chatId;
	MyVector<User*> participants;
	MyVector<Message> messages;

public:
	virtual ~Chat();

	int getChatId() const;
	static int createChatId(MyVector<Chat*>& chats);

	MyVector<User*>& getParticipants();
	MyVector<Message>& getMessages();
	int getMessageCount(const MyString& username);

	virtual void printChat() const = 0;

	void addParticipant(User* user);

	void removeParticipant(User* user);
	virtual void removeAdmin(User* user) = 0;
	virtual void removeUserFromApprovalList(User* user) = 0;

	void addMessage(Message message);
};