#include "Chat.h"
#include "User.h"

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

void Chat::removeParticipant(User* user) {
	for (size_t i = 0; i < participants.getSize(); i++)
	{
		if (participants[i]->getUsername() == user->getUsername())
		{
			participants.remove_at(i);
			break;
		}
	}
}

void Chat::addMessage(Message message) {
	messages.push_back(message);
}