#include <stdexcept>

#include "ChatUtils.h"
#include "Chat.h"
#include "GroupChat.h"
#include "IndividualChat.h"
#include "IOManager.h"

using namespace std;

Chat* findChatById(int id, MyVector<Chat*>& chats) {
	for (size_t i = 0; i < chats.getSize(); i++)
	{
		if (chats[i]->getChatId() == id)
		{
			return chats[i];
		}
	}

	return nullptr;
}

Chat* findIndividualChatByUsernames(const MyString& username1, const MyString& username2, MyVector<Chat*>& chats) {
	for (size_t i = 0; i < chats.getSize(); i++)
	{
		if (IndividualChat* chat = dynamic_cast<IndividualChat*>(chats[i]))
		{
			MyString participant1Username = chat->getParticipants()[0]->getUsername();
			MyString participant2Username = chat->getParticipants()[1]->getUsername();

			if ((participant1Username == username1 && participant2Username == username2)
				|| (participant1Username == username2 && participant2Username == username1))
			{
				return chats[i];
			}
		}
	}

	return nullptr;
}

Chat* findGroupChatByName(const MyString& groupName, MyVector<Chat*>& chats) {
	for (size_t i = 0; i < chats.getSize(); i++)
	{
		if (GroupChat* chat = dynamic_cast<GroupChat*>(chats[i]))
		{
			if (chat->getChatName() == groupName)
			{
				return chats[i];
			}
		}
	}

	return nullptr;
}

const MyString getChatType(Chat* chat) {
	MyString classType = typeid(*chat).name();
	MyString chatType = classType.split(' ')[1];

	return chatType;
}

void deleteChat(Chat* chat, MyVector<Chat*>& chats) {
	if (!chat)
	{
		throw invalid_argument("Chat cannot be null!");
	}

	deleteChatIdFromFile(chat->getChatId());
	deleteChatFromFile(chat->getChatId());

	for (size_t i = 0; i < chats.getSize(); i++)
	{
		if (chats[i]->getChatId() == chat->getChatId())
		{
			chats.remove_at(i);
			break;
		}
	}
}