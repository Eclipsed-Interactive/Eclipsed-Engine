#pragma once

#include <thread>

#include "CoreEngine/MainSingleton.h"

#include "NetworkEngine/Shared/Message.h"
#include "NetworkEngine/Shared/GarantiedMessageHandler.h"

#include "asio/asio.hpp"

#include <unordered_set>

namespace Eclipse
{
	using asio::ip::udp;
	class Server
	{
	public:
		~Server()
		{
			recieveThread.join();
		}
		void RecieveThread()
		{
			asio::executor_work_guard<asio::io_context::executor_type> work = asio::make_work_guard(myIOContext);

			myIOContext.run();
		}

		void ShutDown()
		{
			myIOContext.stop();
			recieveThread.join();
		}

		Server(asio::io_context& ioContext, std::function<void(const NetMessage& aNetMessage)> aHandleRecieveFunc) :
			myIOContext(ioContext),
			socket(ioContext, udp::endpoint(udp::v4(), asio::ip::port_type(24434))),
			recieveThread(&Server::RecieveThread, this),
			garantiedMessageHandler(&Server::SendDirectly_NoChecks, this),
			HandleRecieveFunc(aHandleRecieveFunc)
		{
			//StartRecieve();
			memset(recieveBuffer, 0, sizeof(recieveBuffer));

						
		}

		void Update()
		{
			garantiedMessageHandler.Update();
		}

		void StartRecieve()
		{
			socket.async_receive_from(asio::buffer(recieveBuffer), recieveEndpoint, std::bind(&Server::Recieve, this, asio::placeholders::error, asio::placeholders::bytes_transferred));
		}

		void HandleRecieve(const NetMessage& aMessage)
		{
			HandleRecieveFunc(aMessage);
		}

		void Recieve(const asio::error_code& error, std::size_t bytes_transferred)
		{
			if (error)
				return;

			bool endpointFound = false;

			for (auto& endpoint : endpoints)
				if (endpoint.port() == recieveEndpoint.port())
					endpointFound = true;

			if (!endpointFound)
			{
				endpoints.emplace_back(recieveEndpoint);
				perClientAmountRecievedBack.emplace_back(0);
			}


			NetMessage message;
			memcpy(&message, recieveBuffer, bytes_transferred);

			// if (bytes_transferred == 24)
			// {
			// 	int y = 0;
			// }
			

			HandleRecieve(message);

			hasClients = true;

			if (message.MetaData.IsGarantied)
			{
				if (!message.MetaData.SentGarantied)
					garantiedMessageHandler.RecievedGarantied(message);

				if (message.MetaData.SentGarantied)
				{
					message.MetaData.SentGarantied = false;
					Send(&message, 8, recieveEndpoint);
					message.MetaData.SentGarantied = true;
					
					for (auto& endpoint : endpoints)
					{
						if (recieveEndpoint.port() == endpoint.port())
							continue;
						
						message.MetaData.messageID = ++message.messageIDIncrementor;

						SendGarantied(message, endpoint);
					}

					message.MetaData.IsGarantied = false;
				}
			}
			else
			{
				for (auto& endpoint : endpoints)
				{
					if (recieveEndpoint.port() == endpoint.port())
						continue;

					Send(&message, message.MetaData.dataSize, endpoint);
				}
			}


			StartRecieve();
		}

		void SendManager()
		{
			
		}

		void Send(const void* value, size_t byteSize, const udp::endpoint& endpoint)
		{
			if (!hasClients)
				return;

			socket.async_send_to(asio::buffer(value, byteSize), endpoint, std::bind(&Server::SendManager, this));
		}

		void Send(const NetMessage& message)
		{
			for (auto& endpoint : endpoints)
				Send(&message, message.MetaData.dataSize, endpoint);
		}

		void SendToPrev(const NetMessage& message)
		{
			Send(&message, message.MetaData.dataSize, recieveEndpoint);
		}

		void Send(const NetMessage& message, const udp::endpoint& endpoint)
		{
			Send(&message, message.MetaData.dataSize, endpoint);
		}

		void SendGarantied(const NetMessage& message, const udp::endpoint& endpoint)
		{
			garantiedMessageHandler.Enqueue(message, endpoint);
		}

		void SendToPrev(const NetMessage& message, const std::function<void()>& aLambdaToRunAfterRecieve)
		{
			garantiedMessageHandler.Enqueue(message, recieveEndpoint, aLambdaToRunAfterRecieve);
		}
		void Send(const NetMessage& message, const udp::endpoint& endpoint, const std::function<void()>& aLambdaToRunAfterRecieve)
		{
			garantiedMessageHandler.Enqueue(message, endpoint, aLambdaToRunAfterRecieve);
		}

		std::vector<udp::endpoint>& GetEndpoints()
		{
			return endpoints;
		}

		std::vector<unsigned> perClientAmountRecievedBack;
	private:
		template <class T>
		friend class GarantiedMessageHandler;

		void SendDirectly_NoChecks(NetMessage& message, const udp::endpoint& endpoint)
		{
			socket.async_send_to(asio::buffer(&message, message.MetaData.dataSize), endpoint, std::bind(&Server::SendManager, this));
		}

	private:
		std::function<void(const NetMessage& aNetMessage)> HandleRecieveFunc;

		udp::socket socket;

		char recieveBuffer[512]{};
		udp::endpoint recieveEndpoint;

		std::vector<udp::endpoint> endpoints;

		bool hasClients;

		std::thread recieveThread;

		GarantiedMessageHandler<Server> garantiedMessageHandler;

		asio::io_context& myIOContext;
	};
}