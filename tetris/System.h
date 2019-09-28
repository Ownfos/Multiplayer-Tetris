#pragma once

#include "..//library/directx11 playground/Window.h"
#include "..//library/directx11 playground/BasicInputListener.h"

#include <thread>
#include <atomic>

#include "Object.h"
#include "MyApplication.h"

// handles connection with server and creation of objects
class System : public Object
{
private:
	MyApplication*											application;
	std::shared_ptr<ownfos::network::Socket>				socket;
	std::shared_ptr<ownfos::graphic::Window>				window;
	std::shared_ptr<ownfos::graphic::BasicInputListener>	input;
	std::thread												recieveThread;
	ObjectID												nameInputID;
	ObjectID												scrollListViewerID;
	ObjectID												board;
	ObjectID												opponentBoard;
	std::wstring											userName;
	std::wstring											opponentName;
	std::atomic_bool										userNameValid = false;
	std::atomic_bool										matchStarted = false;
	std::atomic_bool										waitingMatchConfirmation = false; // prevents sending multiple match request at same time

	void DeactivateBoards();

public:
	System
	(
		ObjectID												id,
		MyApplication*											application,
		std::shared_ptr<ownfos::graphic::Window>				window,
		std::shared_ptr<ownfos::graphic::BasicInputListener>	input
	);

	void ConnectServer(std::wstring ip);

	// send name to server to check validity
	void SendName(std::wstring name);

	void SendMatchRequest(std::wstring name);

	void EndMatch();

	void OnCreate() override;

	void OnUpdate(float deltaTime) override;

	void OnRender() override;

	void OnDestroy() override;

	void OnDataRecieved(ownfos::network::Buffer buffer);
};