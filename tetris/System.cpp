#include "System.h"

#include "..//library/winsock playground/NetworkExceptions.h"

#include "InputBox.h"
#include "ScrollListViewer.h"
#include "TetrisBoard.h"

using namespace ownfos::graphic;
using namespace ownfos::network;

System::System
(
	ObjectID												id,
	MyApplication*											application,
	std::shared_ptr<ownfos::graphic::Window>				window,
	std::shared_ptr<ownfos::graphic::BasicInputListener>	input
)
	: Object(id), application(application), window(window), input(input)
{
	socket = std::make_shared<Socket>();

	ConnectServer(L"127.0.0.1");
}

void System::DeactivateBoards()
{
	if (auto pointer = application->FindObjectWithID(board); pointer != nullptr)
		dynamic_cast<TetrisBoard*>(pointer)->Deactivate();
	if (auto pointer = application->FindObjectWithID(opponentBoard); pointer != nullptr)
		dynamic_cast<TetrisBoard*>(pointer)->Deactivate();
}

void System::ConnectServer(std::wstring ip)
{
	socket->ConnectServer(ip, 12345);
	recieveThread = std::thread([this]
		{
			while (true)
			{
				try
				{
					this->OnDataRecieved(socket->RecieveData());
				}
				catch (DisconnectionDetectedException& exception)
				{
					//MessageBox(NULL, exception.what().c_str(), "client recieve thread disconnection", NULL);
					break;
				}
				catch (std::exception& exception)
				{
					MessageBox(NULL, exception.what(), "client recieve thread error", MB_OK);
					break;
				}
			}
		});
}

void System::SendName(std::wstring name)
{
	Buffer b;
	b.Write<int>(0);
	b.WriteWideString(name);
	socket->SendData(b);

	userName = name;
}

void System::SendMatchRequest(std::wstring opponentName)
{
	if (!waitingMatchConfirmation)
	{
		this->opponentName = opponentName;

		Buffer buffer;
		buffer.Write<int>(1);
		buffer.WriteWideString(opponentName);
		socket->SendData(buffer);

		waitingMatchConfirmation = true;
	}
}

void System::EndMatch()
{
	matchStarted = false;
	application->DestroyObject(board);
	application->DestroyObject(opponentBoard);
	scrollListViewerID = application->CreateObject<ScrollListViewer>(application, this, window, input);

	Buffer buffer;
	buffer.Write<int>(4);
	socket->SendData(buffer);
}

void System::OnCreate()
{
	try
	{
		nameInputID = application->CreateObject<InputBox>
		(
			application,
			this,
			window,
			input,
			0,
			0,
			L"your name"
		);
	}
	catch (std::exception& e)
	{
		MessageBox(NULL, e.what(), "System::OnCreate() error", MB_OK);
	}
}

void System::OnUpdate(float deltaTime)
{
	input->UpdateFrameState();

	if (input->FrameState(KeyCode::Esc) == InputState::PressedNow)
	{
		PostQuitMessage(0);
	}
	if (input->FrameState(KeyCode::Enter) == InputState::PressedNow)
	{
		if (auto pointer = application->FindObjectWithID(nameInputID); pointer != nullptr)
		{
			userName = dynamic_cast<InputBox*>(pointer)->GetBuffer();

			Buffer b;
			b.Write<int>(0);
			b.WriteWideString(userName);
			socket->SendData(b);
		}
	}
}

void System::OnRender()
{
	if(userNameValid && !matchStarted)
		application->RenderText(L"User name: " + userName, 0xffffffff, 0, 0, 0.0f);
	//if(matchStarted)
	//	application->RenderText(L"Opponent name: " + opponentName, 0xffffffff, 0, -20, 0.0f);
}

void System::OnDestroy()
{
	try
	{
		socket->Disconnect();
		if(recieveThread.joinable())
			recieveThread.join();
	}
	catch(std::exception& e)
	{
		MessageBox(NULL, e.what(), "error on System::OnDestroy()", MB_OK);
	}
}

void System::OnDataRecieved(ownfos::network::Buffer buffer)
{
	try
	{
		int						size;
		std::list<std::wstring> clientList;
		int						newType;
		int						linesToSend;
		int						dummyLinesRemoved;
		int						dx, dy;
		int						initialType;
		int						opponentInitialType;

		auto messageType = buffer.Read<int>();
		switch (messageType)
		{
		// destroy input box
		case 0:
			application->DestroyObject(nameInputID);

			scrollListViewerID = application->CreateObject<ScrollListViewer>(application, this, window, input);
			userNameValid = true;
			break;

		// invalid name
		case 1:
			MessageBox(NULL, "Invalid user name!", "", MB_OK);
			break;

		// update client list
		case 2:
			size = buffer.Read<int>();
			for (int i = 0; i < size; i++)
				clientList.push_back(buffer.ReadWideString());

			if (auto pointer = application->FindObjectWithID(scrollListViewerID); pointer != nullptr)
				dynamic_cast<ScrollListViewer*>(pointer)->UpdateData(clientList);

			break;
		
		// asks for match confirmation
		case 3:
			opponentName = buffer.ReadWideString();
			waitingMatchConfirmation = true;
			if
			(
				MessageBoxW
				(
					NULL,
					(opponentName + L" is willing to have match with you(" + userName + L").").c_str(),
					L"accept match?",
					MB_YESNO
				) == IDYES
			)
			{
				Buffer buffer;
				buffer.Write<int>(2);
				buffer.WriteWideString(opponentName);
				socket->SendData(buffer);
			}
			else
			{
				Buffer buffer;
				buffer.Write<int>(3);
				buffer.WriteWideString(opponentName);
				socket->SendData(buffer);
			}
			waitingMatchConfirmation = false;
			break;

		// begin match
		case 4:
			application->DestroyObject(scrollListViewerID);
			matchStarted				= true;
			waitingMatchConfirmation	= false;
			initialType					= buffer.Read<int>();
			opponentInitialType			= buffer.Read<int>();
			
			board = application->CreateObject<TetrisBoard>
				(
					application,
					this,
					socket,
					input,
					true,
					-150,
					0,
					initialType
				);
			
			opponentBoard = application->CreateObject<TetrisBoard>
				(
					application,
					this,
					socket,
					input,
					false,
					150,
					0,
					opponentInitialType
				);
			break;

		// match rejected
		case 5:
			opponentName = buffer.ReadWideString();
			waitingMatchConfirmation = false;
			MessageBoxW(NULL, (opponentName + L" refused to have match with you").c_str(), L"", MB_OK);
			break;

		// opponent disconnected
		case 6:
			DeactivateBoards();
			MessageBox(NULL, "Oppponent disconnected", "", MB_OK);
			EndMatch();
			break;

		// set block type
		case 7:
			newType = buffer.Read<int>();
			if (auto pointer = application->FindObjectWithID(opponentBoard); pointer != nullptr)
				dynamic_cast<TetrisBoard*>(pointer)->SetBlockType(newType);
			break;

		// rotate block
		case 8:
			if (auto pointer = application->FindObjectWithID(opponentBoard); pointer != nullptr)
				dynamic_cast<TetrisBoard*>(pointer)->RotateBlock();
			break;

		// opponent lost the match
		case 9:
			DeactivateBoards();
			MessageBox(NULL, "You won!", "Game Over", MB_OK);
			EndMatch();
			break;

		// hard drop
		case 10:
			newType = buffer.Read<int>();
			linesToSend = buffer.Read<int>();
			dummyLinesRemoved = buffer.Read<int>();
			{
				if (auto pointer = application->FindObjectWithID(opponentBoard); pointer != nullptr)
				{
					dynamic_cast<TetrisBoard*>(pointer)->HardDrop(newType);
					dynamic_cast<TetrisBoard*>(pointer)->RemoveDummyLines(dummyLinesRemoved);
				}
				if (auto pointer = application->FindObjectWithID(board); pointer != nullptr)
					dynamic_cast<TetrisBoard*>(pointer)->AddDummyLines(linesToSend);
			}
			{
				Buffer buffer;
				buffer.Write<int>(12);
				buffer.Write<int>(linesToSend);
				socket->SendData(buffer);
			}
			break;

		// move block
		case 11:
			dx = buffer.Read<int>();
			dy = buffer.Read<int>();
			if (auto pointer = application->FindObjectWithID(opponentBoard); pointer != nullptr)
				dynamic_cast<TetrisBoard*>(pointer)->MoveBlock(dx, dy);
			break;

		// add dummy line
		case 12:
			linesToSend = buffer.Read<int>();
			if (auto pointer = application->FindObjectWithID(opponentBoard); pointer != nullptr)
				dynamic_cast<TetrisBoard*>(pointer)->AddDummyLines(linesToSend);
			break;
		}
	}
	catch (std::exception& e)
	{
		MessageBox(NULL, e.what(), "error on client's OnDataRecieved", MB_OK);
	}
}