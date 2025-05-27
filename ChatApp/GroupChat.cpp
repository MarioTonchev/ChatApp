#include <iostream>

#include "GroupChat.h"

using namespace std;

GroupChat::GroupChat(int chatId, const MyString& chatName, MyVector<User*> participants) {
	this->chatId = chatId;
	this->chatName = chatName;
	this->participants = participants;
}

const MyString& GroupChat::getChatName() const {
	return chatName;
}

void GroupChat::printChat() const {
	for (size_t i = 0; i < messages.getSize(); i++)
	{
		cout << messages[i].getDate() << ", " << messages[i].getTime() << " | "
			<< messages[i].getSender() << ": " << messages[i].getContent() << endl;
	}
}