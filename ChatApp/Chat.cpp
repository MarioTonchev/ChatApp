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

int Chat::getMessageCount(const MyString& username) {
	int count = 0;

	for (size_t i = 0; i < messages.getSize(); i++)
	{
		if (messages[i].getSender() == username)
		{
			count++;
		}
	}

	return count;
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