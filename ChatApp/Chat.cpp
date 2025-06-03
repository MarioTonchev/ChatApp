#include "Chat.h"

Chat::~Chat() {}

int Chat::getChatId() const {
	return chatId;
}

MyVector<User*>& Chat::getParticipants() {
	return participants;
}

MyVector<Message>& Chat::getMessages() {
	return messages;
}

void Chat::addParticipant(User* user) {
	participants.push_back(user);
}

void Chat::addMessage(Message message) {
	messages.push_back(message);
}