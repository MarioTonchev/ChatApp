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

	void addParticipant(User* user);
	void removeParticipant(User* user);

	void addMessage(Message message);

	virtual void printChatInfo() const = 0;
	virtual void printChatMessages() const = 0;
};