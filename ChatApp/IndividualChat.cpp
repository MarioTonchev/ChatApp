#include <iostream>

#include "IndividualChat.h"

using namespace std;

IndividualChat::IndividualChat(int chatId) {
	this->chatId = chatId;
}

void IndividualChat::printChat() const {
	for (size_t i = 0; i < messages.getSize(); i++)
	{
		cout << messages[i].getDate() << ", " << messages[i].getTime() << " | "
			<< messages[i].getSender() << ": " << messages[i].getContent() << endl;
	}
}