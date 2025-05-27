#include <iostream>

#include "IndividualChat.h"

using namespace std;

IndividualChat::IndividualChat(int chatId, User& participant1, User& participant2) {
	this->chatId = chatId;
	participants.push_back(&participant1);
	participants.push_back(&participant2);
}

void IndividualChat::printChat() const {
	for (size_t i = 0; i < messages.getSize(); i++)
	{
		cout << messages[i].getDate() << ", " << messages[i].getTime() << " | "
			<< messages[i].getSender() << ": " << messages[i].getContent() << endl;
	}
}