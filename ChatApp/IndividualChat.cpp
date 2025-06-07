#include <iostream>

#include "IndividualChat.h"

using namespace std;

IndividualChat::IndividualChat(int chatId) {
	this->chatId = chatId;
}

void IndividualChat::printChat() const {
	cout << "Chat " << chatId <<":" << endl;

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

void IndividualChat::removeAdmin(User* user) {

}

void IndividualChat::removeUserFromApprovalList(User* user) {

}