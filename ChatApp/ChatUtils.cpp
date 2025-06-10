#include <stdexcept>

#include "ChatUtils.h"
#include "Chat.h"
#include "GroupChat.h"
#include "IndividualChat.h"

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

GroupChat* findGroupChatById(int id, MyVector<Chat*>& chats) {
	for (size_t i = 0; i < chats.getSize(); i++)
	{
		if (GroupChat* groupChat = dynamic_cast<GroupChat*>(chats[i]))
		{
			if (groupChat->getChatId() == id)
			{
				return groupChat;
			}
		}
	}

	return nullptr;
}

IndividualChat* findIndividualChatByUsernames(const MyString& username1, const MyString& username2, MyVector<Chat*>& chats) {
	for (size_t i = 0; i < chats.getSize(); i++)
	{
		if (IndividualChat* chat = dynamic_cast<IndividualChat*>(chats[i]))
		{
			MyString participant1Username = chat->getParticipants()[0]->getUsername();
			MyString participant2Username = chat->getParticipants()[1]->getUsername();

			if ((participant1Username == username1 && participant2Username == username2)
				|| (participant1Username == username2 && participant2Username == username1))
			{
				return chat;
			}
		}
	}

	return nullptr;
}

GroupChat* findGroupChatByName(const MyString& groupName, MyVector<Chat*>& chats) {
	for (size_t i = 0; i < chats.getSize(); i++)
	{
		if (GroupChat* chat = dynamic_cast<GroupChat*>(chats[i]))
		{
			if (chat->getChatName() == groupName)
			{
				return chat;
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

void deleteChat(Chat* chat, MyVector<Chat*>& chats, FileHandler* fileHandler) {
	if (!chat)
	{
		throw invalid_argument("Chat cannot be null!");
	}

	fileHandler->deleteChatIdFromFile(chat->getChatId());
	fileHandler->deleteChatFromFile(chat->getChatId());

	for (size_t i = 0; i < chat->getParticipants().getSize(); i++)
	{
		chat->getParticipants()[i]->removeChat(chat);
		fileHandler->deleteUserChatRelation(chat->getParticipants()[i]->getUsername(), chat->getChatId());
	}

	for (size_t i = 0; i < chats.getSize(); i++)
	{
		if (chats[i]->getChatId() == chat->getChatId())
		{
			chats.remove_at(i);
			break;
		}
	}

	delete chat;
}