#include "Chat.h"

Chat::~Chat() {}

const MyVector<User*>& Chat::getParticipants() const {
	return participants;
}

const MyVector<Message>& Chat::getMessages() const {
	return messages;
}