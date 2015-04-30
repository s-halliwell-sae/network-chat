#include "DynamicTextBox.h"
#include "include\libtcod.hpp"
#include <iostream>

DynamicTextBox::DynamicTextBox(float posX, float posY, float width, float height, std::string title) : TextBox(posX, posY, height, width, title)
{
	x = posX;
	y = posY;
	h = height;
	w = width;
	name = title;
	ResetCursor();
	SetWindowDimensions();
	mScrollSpeed = 1;
}

DynamicTextBox::DynamicTextBox(float posX, float posY, float width, float height, std::string title, std::string prefixIn) : TextBox(posX, posY, height, width, title, prefixIn)
{
	x = posX;
	y = posY;
	h = height;
	w = width;
	name = title;
	ResetCursor();
	prefix = prefixIn;
	SetWindowDimensions();
	mScrollSpeed = 1;
}

void DynamicTextBox::ProcessInput()
{
	mKey = TCODConsole::checkForKeypress(TCOD_KEY_PRESSED);

	if (mKey.vk == TCODK_BACKSPACE)
	{
		if (contents.size() > 0)
		{
			contents = contents.substr(0, contents.size() - 1);
			Scroll(0);
			InSync = false;
		}
	}
	else if (mKey.c && mKey.vk != TCODK_ENTER && mKey.vk != TCODK_KPENTER)
	{
		contents += mKey.c;
		Scroll(LinesOnScreen(contents.length()));
		InSync = false;
	}
}

void DynamicTextBox::Clear()
{
	contents.clear();
	InSync = false;
	cursorX = x + 1;
	cursorY = y + 3;
}

const std::string& DynamicTextBox::GetContents()
{
	return contents;
}


void DynamicTextBox::SetContents(std::vector<std::string> contentsIn)
{
	LOGWARN("Attempted to set the contents of a DynamicTextBox");
}

void DynamicTextBox::AddEntry(std::string newEntry)
{
	LOGWARN("Attempted to add to the contents of a DynamicTextBox");
}

void DynamicTextBox::Render()
{
	ResetCursor();
	RenderFrame();
	PrintLine(contents);
}

int DynamicTextBox::NumLines()
{
	return LinesOnScreen(contents.length());
}