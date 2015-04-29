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
	cursorX = x + 1;
	cursorY = y + 3;
}

DynamicTextBox::DynamicTextBox(float posX, float posY, float width, float height, std::string title, std::string prefixIn) : TextBox(posX, posY, height, width, title, prefixIn)
{
	x = posX;
	y = posY;
	h = height;
	w = width;
	name = title;
	cursorX = x + 1;
	cursorY = y + 3;
	prefix = prefixIn;
}

//Work out awkwardness for the keypresses. Needs to be fixed.
void DynamicTextBox::ProcessInput()
{
	//LOG("PI");
	TCOD_key_t key = TCODConsole::checkForKeypress();

	if (key.vk == TCODK_BACKSPACE)
	{
		if (contents.size() > 0)
		{
			contents = contents.substr(0, contents.size() - 1);
			InSync = false;
		}
	}
	else if (key.c && key.vk != TCODK_ENTER && key.vk != TCODK_KPENTER)
	{
		contents += key.c;
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
	//format shite
	//Might just not be necessary at all
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