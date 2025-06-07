#pragma once

#include "Chat.h"

class IndividualChat : public Chat {
public:
	IndividualChat(int chatId);
	void printChat() const override;
	void removeAdmin(User* user) override;
	void removeUserFromApprovalList(User* user) override;
};