#ifndef MESSAGE_BOARD_H
#define MESSAGE_BOARD_H
#define MESSAGEDURATION 3
#define MAX_NUM_MESSAGE 6

#include <list>
#include <string>
using std::list;
using std::string;

struct Message
{
	Message(string sender, string receiver, string content)
	{
		this->sender = sender;
		this->receiver = receiver;
		this->content = content;
	}
	string content;
	string sender;
	string receiver;
};

struct MessageBoard
{
	MessageBoard();
	~MessageBoard();
	list<Message> Messages;
	void addMessage(string sender, string receiver, string content);
	void Update(double);

	float messageDuration;
	int yOffset;
};


#endif