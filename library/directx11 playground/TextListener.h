#pragma once

#include <string>

#include "InputListener.h"

namespace ownfos::graphic
{
	// -Listens to window's input messages and construct a text buffer.
	// -Each window can have multiple TextListener.
	// -You can activate/inactivate listening by calling TextListener::EnableListening().
	//
	// -----Example usage-----
	// TextListener listener(window, 20);
	// std::wstring inputMessage = listener.GetBuffer();
	// listener.ClearBuffer();
	// listener.EnableListening(false);
	class TextListener : public InputListener
	{
	private:
		size_t bufferLength;
		size_t caretPosition;
		bool listening;
		bool enableNewLine;
		bool enableCaretControl;
		std::wstring buffer;

	public:
		TextListener(std::shared_ptr<Window> window, size_t bufferLength, bool listening = true, bool enableNewLine = false, bool enableCaretControl = false, std::wstring initialText = L"");

		void OnMessage(UINT message, WPARAM wParam, LPARAM lParam) override;

		// -BasicInputListener class will stop sending messages if you pass false.
		void EnableListening(bool enable);

		void ClearBuffer();
		std::wstring GetBuffer();
		int GetCaretPosition();
	};
}

