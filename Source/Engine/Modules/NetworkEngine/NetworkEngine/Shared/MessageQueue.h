//#pragma once
//
//#include "Message.h"
//
//#include <new>
//
//#define MAXGARANTIEDMESSAGESATONCE 1000000
//
//class MessageQueue
//{
//public:
//	MessageQueue()
//	{
//		messageHolder = new char[MAXGARANTIEDMESSAGESATONCE];
//		//memset(messageHolder, 0, MAXGARANTIEDMESSAGESATONCE);
//	}
//	~MessageQueue() = default;
//
//	void Enqueue(const Message& message)
//	{
//		Message* messageHolderptr = reinterpret_cast<Message*>(messageHolder + frontCursor);
//		frontCursor += sizeof(Message);
//		::new(messageHolderptr) Message();
//
//		memcpy(messageHolderptr, &message, 512);
//
//		size++;
//	}
//
//	void DeQueue()
//	{
//		Message* messageHolderptr = reinterpret_cast<Message*>(messageHolder + backCursor);
//		backCursor += sizeof(Message);
//		messageHolderptr->~Message();
//		memset(messageHolderptr, 0, 512);
//	
//		size--;
//	}
//
//	Message* Front()
//	{
//		size_t front = frontCursor - sizeof(Message);
//		Message* messageHolderptr = reinterpret_cast<Message*>(messageHolder + front);
//
//		return messageHolderptr;
//	}
//
//	bool Empty()
//	{
//		return !(bool)size;
//	}
//	bool NotEmpty()
//	{
//		return (bool)size;
//	}
//
//	size_t Size()
//	{
//		return size;
//	}
//
//private:
//	char* messageHolder = nullptr;
//
//	size_t frontCursor = 0;
//	size_t backCursor = 0;
//
//	size_t size = 0;
//};