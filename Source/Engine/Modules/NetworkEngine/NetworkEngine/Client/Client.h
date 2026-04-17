#pragma once

#include <thread>
#include <functional>

#include "asio/asio.hpp"

#include "NetworkEngine/Shared/GarantiedMessageHandler.h"

#include "NetworkEngine/Shared/Message.h"


namespace Eclipse
{
	using asio::ip::udp;
	class Client
	{
	public:
		~Client()
		{
			recieveThread.join();
		}

		void RecieveThread()
		{
			asio::executor_work_guard<asio::io_context::executor_type> work = asio::make_work_guard(myIOContext);

			myIOContext.run();

			work.reset();
		}

		void ShutDown()
		{
			myIOContext.stop();

			recieveThread.join();
		}

		Client(asio::io_context& ioContext, const char* ip, std::function<void(const NetMessage& aNetMessage)> aHandleRecieveFunc) :
			myIOContext(ioContext),
			socket(ioContext),
			serverEndpoint(*(udp::resolver(ioContext)).resolve(udp::v4(), ip, "24434").begin()),
			recieveThread(&Client::RecieveThread, this),
			garantiedMessageHandler(&Client::SendDirectly_NoChecks, this),
			HandleRecieveFunc(aHandleRecieveFunc)
		{
			memset(recieveBuffer, 0, sizeof(recieveBuffer));
			socket.open(udp::v4());
			StartRecieve();
		}

		void HandleRecieve(const NetMessage& message);

		void Update()
		{
			garantiedMessageHandler.Update();
		}

		void Recieve(const asio::error_code& error, std::size_t bytes_transferred)
		{
			if (error)
			{
				StartRecieve();
				return;
			}

			NetMessage message;
			memcpy(&message, recieveBuffer, bytes_transferred);

			isConnected = true;

			if (message.MetaData.IsGarantied)
			{
				if (!message.MetaData.SentGarantied)
					garantiedMessageHandler.RecievedGarantied(message);

				if (message.MetaData.SentGarantied)
				{
					message.MetaData.SentGarantied = false;

					Send(&message, 8, recieveEndpoint);
				}

				message.MetaData.IsGarantied = false;
			}

			HandleRecieve(message);

			StartRecieve();
		}

		void StartRecieve()
		{
			socket.async_receive_from(asio::buffer(recieveBuffer), recieveEndpoint, std::bind(&Client::Recieve, this, asio::placeholders::error, asio::placeholders::bytes_transferred));
		}

		void SendManager()
		{

		}

		void Send(const void* value, size_t byteSize, const udp::endpoint& endpoint)
		{
			if (!isConnected)
				return;

			socket.async_send_to(asio::buffer(value, byteSize), endpoint, std::bind(&Client::SendManager, this));
		}

		void Send(NetMessage& message)
		{
			if (!isConnected)
				return;

			if (message.MetaData.IsGarantied && message.MetaData.SentGarantied)
				garantiedMessageHandler.Enqueue(message, serverEndpoint);
			else
				socket.async_send_to(asio::buffer(&message, message.MetaData.dataSize), serverEndpoint, std::bind(&Client::SendManager, this));
		}

		// Only for garantied messages
		void Send(NetMessage& message, const std::function<void()>& aLambdaToRunAfterRecieve)
		{
			garantiedMessageHandler.Enqueue(message, serverEndpoint, aLambdaToRunAfterRecieve);
		}

	private:
		template <class T>
		friend class GarantiedMessageHandler;

		// Does not check for garantied messages do not use this function
		void SendDirectly_NoChecks(NetMessage& message, const udp::endpoint& endpoint)
		{
			socket.async_send_to(asio::buffer(&message, message.MetaData.dataSize), endpoint, std::bind(&Client::SendManager, this));
		}

	private:
		udp::endpoint serverEndpoint;
		udp::socket socket;

		char recieveBuffer[512]{};
		udp::endpoint recieveEndpoint;

		bool isConnected = false;

		std::thread recieveThread;

		GarantiedMessageHandler<Client> garantiedMessageHandler;

		asio::io_context& myIOContext;

		std::function<void(const NetMessage& aNetMessage)> HandleRecieveFunc;
	};
}