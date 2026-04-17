#include "Client.h"

#include "NetworkEngine/Shared/Message.h"

namespace Eclipse
{
	void Client::HandleRecieve(const NetMessage& message)
	{
		HandleRecieveFunc(message);
	}
}