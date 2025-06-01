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

void Chat::addParticipant(User* user) {
	participants.push_back(user);
}

void Chat::addMessage(Message message) {
	messages.push_back(message);
}