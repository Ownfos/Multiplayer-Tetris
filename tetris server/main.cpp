#pragma comment(lib, "../library/winsock playground.lib")

#include "../library/winsock playground/Socket.h"
#include "../library/winsock playground/Buffer.h"
#include "../library/winsock playground/Utility.h"
#include "../library/winsock playground/NetworkExceptions.h"

#include <iostream>
#include <thread>
#include <map>
#include <list>
#include <mutex>
#include <string>
#include <algorithm>
#include <random>

using namespace ownfos::network;

struct Client
{
	std::shared_ptr<Socket> socket;
	std::wstring opponentName; // if this is L"", client is whether in match or waiting for match confirmation.
};

void SendClientList(std::map<std::wstring, Client>& clients)
{
	for (auto [currentName, client] : clients)
	{
		Buffer buffer;
		buffer.Write<int>(2);
		auto count = std::count_if(clients.begin(), clients.end(), [](std::pair<const std::wstring, Client>& client) { return client.second.opponentName == L""; }) - 1;
		buffer.Write<int>(count);
		for (auto [name, client] : clients)
			if (name != currentName && client.opponentName == L"")
				buffer.WriteWideString(name);
		client.socket->SendData(buffer);
	}
}

int main()
{
	try
	{
		Initialize();
		{
			std::default_random_engine engine(time(NULL));
			std::uniform_int_distribution<int> distribution(0, 6);

			std::map<std::wstring, Client> clients;
			std::mutex clientsMutex;
			
			auto listener = std::make_shared<Socket>();
			listener->InitializeServer(12345);

			while (true)
			{
				auto client = listener->AcceptClient();
				std::thread recieveThread([&clients, &clientsMutex, client, &engine, &distribution]
					{
						std::wstring name = L"";
						std::wstring opponentName = L"";
						while (true)
						{
							try
							{
								auto buffer = client->RecieveData();

								int initialType;
								int opponentInitialType;

								int type = buffer.Read<int>();

								if (type == 0) // -Register name
								{
									name = buffer.ReadWideString();
									std::wcout << L"someone is trying to register as name : " << name << std::endl;
									{
										std::lock_guard guard(clientsMutex);
										if (clients.find(name) == clients.end() && name != L"") // -If name is valid
										{
											Buffer buffer;
											buffer.Write<int>(0);
											client->SendData(buffer);

											clients[name] = { client, L"" };
											SendClientList(clients);
										}
										else
										{
											Buffer buffer;
											buffer.Write<int>(1);
											client->SendData(buffer);
										}
									}
								}
								else if (type == 1) // -Match request
								{
									opponentName = buffer.ReadWideString();
									{
										std::lock_guard guard(clientsMutex);
										if (clients.find(opponentName) != clients.end() && clients[opponentName].opponentName == L"")
										{
											clients[name].opponentName = opponentName;
											clients[opponentName].opponentName = name;

											Buffer buffer;
											buffer.Write<int>(3);
											buffer.WriteWideString(name);
											clients[opponentName].socket->SendData(buffer);
										}
									}
								}
								else if (type == 2) // -Match accepted
								{
									opponentName = buffer.ReadWideString();
									{
										initialType = distribution(engine);
										opponentInitialType = distribution(engine);

										std::lock_guard guard(clientsMutex);
										{
											Buffer buffer;
											buffer.Write<int>(4);
											buffer.Write<int>(initialType);
											buffer.Write<int>(opponentInitialType);
											clients[name].socket->SendData(buffer);
										}
										{
											Buffer buffer;
											buffer.Write<int>(4);
											buffer.Write<int>(opponentInitialType);
											buffer.Write<int>(initialType);
											clients[opponentName].socket->SendData(buffer);
										}
										SendClientList(clients);
									}
								}
								else if (type == 3) // -Match rejected
								{
									opponentName = buffer.ReadWideString();
									{
										std::lock_guard guard(clientsMutex);
										clients[name].opponentName = L"";
										clients[opponentName].opponentName = L"";

										Buffer buffer;
										buffer.Write<int>(5);
										buffer.WriteWideString(name);
										clients[opponentName].socket->SendData(buffer);
									}
								}
								else if (type == 4) // -Match ended
								{
									std::lock_guard guard(clientsMutex);
									
									clients[name].opponentName = L"";
									SendClientList(clients);
								}
								else // -Pass buffer to opponent
								{
									std::lock_guard guard(clientsMutex);
									clients[clients[name].opponentName].socket->SendData(buffer);
								}
							}
							catch (DisconnectionDetectedException& exception)
							{
								if (name != L"")
								{
									std::wcout << name << L" disconnection detected!" << std::endl;
									{
										std::lock_guard guard(clientsMutex);

										// notify peer opponent
										if (auto opponent = clients.find(clients[name].opponentName); opponent!=clients.end())
										{
											Buffer buffer;
											buffer.Write<int>(6);
											opponent->second.opponentName = L"";

											try
											{
												opponent->second.socket->SendData(buffer);
											}
											catch (DisconnectionDetectedException& e)
											{

											}
											catch (std::exception& e)
											{

											}
										}

										clients.erase(name);
										SendClientList(clients);
									}
								}
								return;
							}
							catch (std::exception& exception)
							{
								std::cout << "exception! : " << exception.what() << std::endl;
								return;
							}
						}
					});
				recieveThread.detach();
			}
		}
		Uninitialize();
	}
	catch (std::exception& exception)
	{
		MessageBox(NULL, exception.what(), "Error", MB_OK);
		return -1;
	}
}