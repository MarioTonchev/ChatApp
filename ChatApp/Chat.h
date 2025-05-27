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
	virtual void printChat() const = 0;
	virtual ~Chat();
	const MyVector<User*>& getParticipants() const;
	const MyVector<Message>& getMessages() const;
};