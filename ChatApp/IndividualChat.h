#pragma once

#include "Chat.h"

class IndividualChat : public Chat {
public:
	IndividualChat(int chatId, User& participant1, User& participant2);
	void printChat() const override;
};