#pragma once

#include "Chat.h"

class IndividualChat : public Chat {
public:
	IndividualChat(int chatId);
	void printChatInfo() const override;
	void printChatMessages() const override;
};