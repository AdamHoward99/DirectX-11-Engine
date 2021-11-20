#pragma once
#include "KeyboardEvent.h"
#include <Queue>

class Keyboard
{
public:
	Keyboard();
	bool IsKeyPressed(const unsigned char);
	bool IsKeyBufferEmpty();
	bool IsCharBufferEmpty();
	bool IsKeyAutoRepeating();
	bool IsCharAutoRepeating();

	KeyboardEvent ReadKey();
	unsigned char ReadChar();

	void OnKeyPressed(const unsigned char);
	void OnKeyReleased(const unsigned char);
	void OnChar(const unsigned char);
	void EnableAutoRepeatedKeys();
	void DisableAutoRepeatedKeys();
	void EnableAutoRepeatedChars();
	void DisableAutoRepeatedChars();
private:
	bool autoRepeatedKeys = false;
	bool autoRepeatedChars = false;
	bool keyStates[256];
	std::queue<KeyboardEvent> keyBuffer;
	std::queue<unsigned char> charBuffer;
};