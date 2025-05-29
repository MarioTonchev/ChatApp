#include "Chat.h"

Chat::~Chat() {}

int Chat::getChatId() const {
	return chatId;
}

const MyVector<User*>& Chat::getParticipants() const {
	return participants;
}

const MyVector<Message>& Chat::getMessages() const {
	return messages;
}