#include "GarantiedMessageHandler.h"

#include "NetworkEngine/Client/Client.h"
#include "NetworkEngine/Server/Server.h"

#include "CoreEngine/MainSingleton.h"
#include "CoreEngine/Timer.h"

namespace Eclipse
{
	template <class T>
	void GarantiedMessageHandler<T>::Update()
	{
		mapChangeMutex.lock();
		for (auto& [_, message] : GarantiedMsgs)
		{
			float& TryAgainTimer = message.TryAgainTimer;

			TryAgainTimer -= Time::GetDeltaTime();

			if (TryAgainTimer > 0.f)
				continue;

			(FunctionOwner->*SendDirectlyFunc)(message.message, message.endpoint);

			TryAgainTimer = TimeBetweenTryAgains;
		}
		mapChangeMutex.unlock();
	}

	template <class T>
	void GarantiedMessageHandler<T>::Enqueue(const NetMessage& message, const udp::endpoint& anEndpoint)
	{
		NetMessage cpyMessage;
		std::memcpy(&cpyMessage, &message, message.MetaData.dataSize);

		GarantiedMessage grntMessage(cpyMessage, anEndpoint);

		grntMessage.TimeAtFirstSend = Time::GetTotalTime();
		
		mapChangeMutex.lock();
		GarantiedMsgs.emplace(message.MetaData.messageID, grntMessage);
		mapChangeMutex.unlock();
	}

	template <class T>
	void GarantiedMessageHandler<T>::Enqueue(const NetMessage& message, const udp::endpoint& anEndpoint, const std::function<void()>& aLambdaFunctionToRunOnRecieve)
	{
		NetMessage cpyMessage;
		std::memcpy(&cpyMessage, &message, message.MetaData.dataSize);

		GarantiedMessage grntMessage(message, anEndpoint, aLambdaFunctionToRunOnRecieve);

		grntMessage.TimeAtFirstSend = Time::GetTotalTime();
		
		mapChangeMutex.lock();
		GarantiedMsgs.emplace(message.MetaData.messageID, grntMessage);
		mapChangeMutex.unlock();
	}

	template <class T>
	void GarantiedMessageHandler<T>::RecievedGarantied(const NetMessage& aMessage)
	{
		if (aMessage.MetaData.SentGarantied)
			return;
		if (GarantiedMsgs.empty())
			return;
		if (GarantiedMsgs.find(aMessage.MetaData.messageID) == GarantiedMsgs.end())
			return;
		
		auto& garantiedMessage = GarantiedMsgs.at(aMessage.MetaData.messageID);

		float currenttime = Time::GetTotalTime();
		float secondsPing = currenttime - garantiedMessage.TimeAtFirstSend;

		//TemporarySettingsSingleton::Get().ping = (int)(secondsPing * 1000.f);
		
		if (garantiedMessage.HasLambda)
			garantiedMessage.LambdaToRunOnRecieve();
		

		mapChangeMutex.lock();
		GarantiedMsgs.erase(aMessage.MetaData.messageID);
		mapChangeMutex.unlock();
	}
}

template class Eclipse::GarantiedMessageHandler<Eclipse::Server>;
template class Eclipse::GarantiedMessageHandler<Eclipse::Client>;