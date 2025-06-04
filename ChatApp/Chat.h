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
	MyVector<User*>& getParticipants();
	MyVector<Message>& getMessages();
	int getMessageCount(const MyString& username);

	virtual void printChat() const = 0;

	void addParticipant(User* user);
	void removeParticipant(User* user);

	void addMessage(Message message);
};