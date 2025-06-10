#include <iostream>

#include "IndividualChat.h"

using namespace std;

IndividualChat::IndividualChat(int chatId) {
	this->chatId = chatId;
}

void IndividualChat::printChatInfo() const {
	cout << "Individual chat with id " << chatId << " ("
		<< messages.getSize() << " messages)" << endl;
}

void IndividualChat::printChatMessages() const {
	cout << "Individual chat with id " << chatId <<":" << endl;

	if (messages.getSize() == 0)
	{
		cout << "No messages yet." << endl;
		return;
	}

	for (size_t i = 0; i < messages.getSize(); i++)
	{
		cout << "[" << messages[i].getSender() << ", " << messages[i].getDate() << " " << messages[i].getTime()
			<< "] " << messages[i].getContent() << endl;
	}
}