#pragma once

#include "Chat.h"

class GroupChat : public Chat {
private:
	MyString chatName;
public:
	GroupChat(int chatId, const MyString& chatName);
	const MyString& getChatName() const;
	void printChat() const override;
}; 