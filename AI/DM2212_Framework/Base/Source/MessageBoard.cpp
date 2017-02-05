#include "MessageBoard.h"

MessageBoard::MessageBoard()
{
	messageDuration = MESSAGEDURATION;
	yOffset = 3;
}
MessageBoard::~MessageBoard()
{

}
void MessageBoard::addMessage(string sender, string receiver, string content)
{
	Messages.push_back(Message(sender,receiver,content));
}

void MessageBoard::Update(double dt)
{
	if (Messages.size() > 0)
	{
		messageDuration -= dt;
		if (messageDuration <= 0.f || Messages.size() > MAX_NUM_MESSAGE)
		{
			Messages.pop_front();
			if (messageDuration <= 0.f)
				messageDuration = MESSAGEDURATION;
		}

	}
}

