#include "Keyboard.h"

Keyboard::Keyboard()
{
	for (auto k : keyStates)
		k = false;
}

bool Keyboard::IsKeyPressed(const unsigned char keycode)
{
	return keyStates[keycode];
}

bool Keyboard::IsKeyBufferEmpty()
{
	return keyBuffer.empty();
}

bool Keyboard::IsCharBufferEmpty()
{
	return charBuffer.empty();
}

KeyboardEvent Keyboard::ReadKey()
{
	if (keyBuffer.empty())
		return KeyboardEvent();

	KeyboardEvent ev = keyBuffer.front();
	keyBuffer.pop();
	return ev;
}

unsigned char Keyboard::ReadChar()
{
	if (charBuffer.empty())
		return 00;

	unsigned char c = charBuffer.front();
	charBuffer.pop();
	return c;
}

void Keyboard::OnKeyPressed(const unsigned char key)
{
	keyStates[key] = true;
	keyBuffer.push(KeyboardEvent(KeyboardEvent::EventType::Press, key));
}

void Keyboard::OnKeyReleased(const unsigned char key)
{
	keyStates[key] = false;
	keyBuffer.push(KeyboardEvent(KeyboardEvent::EventType::Release, key));
}

void Keyboard::OnChar(const unsigned char key)
{
	charBuffer.push(key);
}

void Keyboard::EnableAutoRepeatedKeys()
{
	autoRepeatedKeys = true;
}

void Keyboard::DisableAutoRepeatedKeys()
{
	autoRepeatedKeys = false;
}

void Keyboard::EnableAutoRepeatedChars()
{
	autoRepeatedChars = true;
}

void Keyboard::DisableAutoRepeatedChars()
{
	autoRepeatedChars = false;
}

bool Keyboard::IsKeyAutoRepeating()
{
	return autoRepeatedKeys;
}

bool Keyboard::IsCharAutoRepeating()
{
	return autoRepeatedChars;
}