#include "TextBox.h"
#include "include\libtcod.hpp"
#include <iostream>

TextBox::TextBox(float posX, float posY, float width, float height, std::string title) //: x(posX), y(posY), h(height), w(width), name(title)
{
	x = posX;
	y = posY;
	h = height;
	w = width;
	name = title;
	cursorX = x + 1;
	cursorY = y + 3;
}

TextBox::TextBox(float posX, float posY, float width, float height, std::string title, std::string prefixIn) //: x(posX), y(posY), h(height), w(width), name(title)
{
	x = posX;
	y = posY;
	h = height;
	w = width;
	name = title;
	ResetCursor();
	prefix = prefixIn;
}

TextBox::~TextBox()
{
	
}

/*void TextBox::ProcessInput()
{

	return;
}*/

void TextBox::Render()
{
	ResetCursor();
	RenderFrame();
	for each(std::string line in contents)
	{
		PrintLine(line);
	}
}

void TextBox::RenderFrame()
{
	//Render background box
	TCODConsole::root->printFrame(x, y, w, h);
	//Render title box
	TCODConsole::root->printFrame(x, y, w, 3);
	TCODConsole::root->print(int((x + (w / 2) - (name.length() / 2)) + 0.5), int(y + 0.5) + 1, name.c_str());
}

void TextBox::Clear()
{
	contents.clear();
	InSync = false;
	ResetCursor();
}

std::string TextBox::GetName()
{
	return name;
}

void TextBox::SetContents(std::vector<std::string> contentsIn)
{
	contents = contentsIn;
	InSync = false;
}

void TextBox::AddEntry(std::string newEntry)
{
	contents.push_back(newEntry);
	InSync = false;
}

void TextBox::PrintLine(std::string& line)
{
	if (line.length() <= 0)
		return;
	std::string printedLine = prefix + line;

	int iterations = 0;
	int temp = printedLine.length();
	
	iterations = LinesOnScreen(temp) - 1;

	for (int i = 0; i <= iterations; ++i)
	{
		if (((cursorY - y) + i) > (h - 2))
			break;
		TCODConsole::root->print(int(cursorX + 0.5), int(cursorY + 0.5) + i, printedLine.substr(0, int(w - 2)).c_str());
		printedLine.erase(0, int(w - 2));
	}
	cursorY += (iterations + 1);
}

void TextBox::Scroll(int factor)
{
	//LOG(std::to_string(factor));
	startingLine += factor;
	int max = NumLines();
	startingLine = CLAMP(0, max, startingLine);
}

void TextBox::ResetCursor()
{
	cursorX = x + 1;
	cursorY = y + 3;
}

int TextBox::NumLines()
{
	//TEST
	int ret = 0;
	for each (std::string line in contents)
	{
		ret += LinesOnScreen(line.length());
	}
	return ret;
}

int TextBox::LinesOnScreen(int contentLength)
{
	int ret = 0;
	while (contentLength > int(w - 2))
	{
		ret++;
		contentLength -= int(w - 2);
	}
	return ret + 1;
}