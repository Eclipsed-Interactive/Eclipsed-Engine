#pragma once

#include <unordered_map>
#include <thread>
#include <mutex>

#include "asio/asio.hpp"

#include "Message.h"

namespace Eclipse
{
	using asio::ip::udp;

	class Client;

	template <class T>
	class GarantiedMessageHandler
	{
	public:
		GarantiedMessageHandler(void (T::* aSendDirectlyFunc)(NetMessage&, const udp::endpoint&), T* owner) : FunctionOwner(owner), SendDirectlyFunc(aSendDirectlyFunc)
		{

		}

		~GarantiedMessageHandler()
		{

		}

		void Update();
		void Enqueue(const NetMessage& message, const udp::endpoint& anEndpoint);
		void Enqueue(const NetMessage& message, const udp::endpoint& anEndpoint, const std::function<void()>& aLambdaFunctionToRunOnRecieve);
		void RecievedGarantied(const NetMessage& aMessage);

	private:
		float TimeBetweenTryAgains = 0.1f;

		std::mutex mapChangeMutex;


		T* FunctionOwner;
		void (T::* SendDirectlyFunc)(NetMessage&, const udp::endpoint&);

	public:
		struct GarantiedMessage
		{
			GarantiedMessage(const NetMessage& aMessage, const udp::endpoint& anEndpoint) : endpoint(anEndpoint), HasLambda(false)
			{
				memcpy(&message, &aMessage, 512);
			}
			GarantiedMessage(const NetMessage& aMessage, const udp::endpoint& anEndpoint, const std::function<void()>& aLambdaToRunOnRecieve) : endpoint(anEndpoint), HasLambda(true), LambdaToRunOnRecieve(aLambdaToRunOnRecieve)
			{
				memcpy(&message, &aMessage, 512);
			}

			udp::endpoint endpoint;
			NetMessage message;

			float TryAgainTimer = 0;
			float TimeAtFirstSend = 0;

			bool HasLambda;
			std::function<void()> LambdaToRunOnRecieve;
		};

		//MessageQueue GarantiedMsgs;
		std::unordered_map<unsigned, GarantiedMessage> GarantiedMsgs;

		int toatlFailed;
	};
}