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
	ResetCursor();
	SetWindowDimensions();
	mScrollSpeed = 1;
}

TextBox::TextBox(float posX, float posY, float width, float height, int scrollSpeed, std::string title) //: x(posX), y(posY), h(height), w(width), name(title)
{
	x = posX;
	y = posY;
	h = height;
	w = width;
	name = title;
	ResetCursor();
	SetWindowDimensions();
	mScrollSpeed = scrollSpeed;
	LOG("Cons");
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
	SetWindowDimensions();
	mScrollSpeed = 1;
	LOG("Cons");
}

TextBox::TextBox(float posX, float posY, float width, float height, int scrollSpeed, std::string title, std::string prefixIn) //: x(posX), y(posY), h(height), w(width), name(title)
{
	x = posX;
	y = posY;
	h = height;
	w = width;
	name = title;
	ResetCursor();
	prefix = prefixIn;
	SetWindowDimensions();
	mScrollSpeed = scrollSpeed;
	LOG("Cons");
}

TextBox::~TextBox()
{
	
}

//Display the contents of the GUI Element
void TextBox::Render()
{
	//Move the "printing point" to the start of the box
	ResetCursor();
	RenderFrame();
	//Print out the contents of the TextBox
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
	RenderScrollBar();
}

//Calculate the necessity, size and position of the scroll bar, then render it to the screen
void TextBox::RenderScrollBar()
{
	//Find what percentage of the TextBox the scroll bar should take up and convert it to on-screen positions
	int scrollBarLength = (h - 4) * (((h - 4) / ((NumLines() == 0) ? 1 : NumLines())));

	//Makes sure the scroll bar isn't rendered as larger than the TextBox or smaller than one TCOD character
	if (scrollBarLength > h - 4)
	{
		scrollBarLength = h - 4;
	}
	else if (scrollBarLength < 1)
	{
		scrollBarLength = 1;
	}

	//Find the maximum scrolling value
	int max = NumLines();
	max -= (h - 4);
	if (max < 0)
	{
		max = 0;
	}

	//Lerp between the minimum printing position and the maximum printing position within the TextBox 
	//based on the percentage through the content the user has currently scrolled
	int printBuf = LERP(0, h - 4 - scrollBarLength, float(startingLine) / float(max));

	//If there is enough content to render a scroll bar
	if (printBuf >= 0)
	{
		for (int i = 0; i < scrollBarLength; ++i)
		{
			TCODConsole::root->print(x + w - 2, y + 3 + i + printBuf, "|");
		}
	}
}

//Empties the TextBox and resets all relevant Data
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
	Scroll(0);
	InSync = false;
}

void TextBox::AddEntry(std::string newEntry)
{
	contents.push_back(newEntry);
	//Check to see if the user is looking at the bottom line of the contents
	if (NumLines() == (MaxScroll() + 2))
	{
		//If so, scroll down so the user can see the newest entry
		Scroll(1);
	}
	InSync = false;
}

void TextBox::PrintLine(std::string& line)
{
	if (line.length() <= 0)
		return;
	std::string printedLine = prefix + line;

	int iterations = 0;
	
	iterations = LinesOnScreen(printedLine.length());
	
	for (int i = 0; i < iterations; ++i)
	{
		/*if (((cursorY - y) + i) > (h - 2))
			break;*/
		//LOG("If(" + std::to_string(cursorY - (y + 3)) + " >= " + std::to_string(startingLine) + " && " + std::to_string(cursorY - (y + 3)) + " <= " + std::to_string(startingLine + int(h - 4)) + ")");
		if (curLine - (y + 3) >= startingLine && curLine - (y + 3) <= MaxScroll())
		{
			TCODConsole::root->print(int(cursorX + 0.5), int(cursorY + 0.5), printedLine.substr(0, int(w - 3)).c_str());
			cursorY++;
		}
		curLine++;
		printedLine.erase(0, int(w - 3));
	}
}

void TextBox::Scroll(int factor)
{
	//LOG("CursorY: " + std::to_string(cursorY) + ", Starting Line " + std::to_string(startingLine) + ", Current Line: " + std::to_string(curLine));
	//LOG(std::to_string(factor));
	int compare = startingLine;
	factor *= mScrollSpeed;
	startingLine += factor;
	int max = NumLines();
	//if(name == "Users")LOG(std::to_string(max));
	//LOG("Num lines in " + name + ":" + std::to_string(max));
	//LOG("Height: " + std::to_string(int(h - 4)));
	max -= (h - 4);
	if (max < 0)
	{
		max = 0;
	}

	//LOG("Pre: " + std::to_string(startingLine) + ", Post: " + std::to_string(CLAMP(0, max, startingLine)));
	
	startingLine = CLAMP(0, max, startingLine);
	if (startingLine != compare)
		InSync = false;
}

void TextBox::ResetCursor()
{
	cursorX = x + 1;
	cursorY = y + 3;
	curLine = cursorY;
}

int TextBox::NumLines()
{
	//TEST
	int ret = 0;
	for each (std::string line in contents)
	{
		ret += LinesOnScreen(line.length() + prefix.length());
	}
	return ret;
}

int TextBox::LinesOnScreen(int contentLength)
{
	//int backup = contentLength;
	int ret = 0;
	if (contentLength == 0)
		return ret;
	while (contentLength > 0)
	{
		//LOG("Length: " + std::to_string(contentLength) + ", Width: " + std::to_string(w - 3));
		ret++;
		contentLength -= int(w - 3);
	}
	//LOG(name + " with character length: " + std::to_string(backup) + " takes up: " + std::to_string(ret) + " lines on screen.");
	return ret;
}

bool TextBox::Collision(int x, int y)
{
	if (float(x) > mWB.minX && float(x) < mWB.maxX && float(y) > mWB.minY && float(y) < mWB.maxY)
	{
		//LOG(name);
		return true;
	}
	return false;
}

void TextBox::SetWindowDimensions()
{
	mWB.minX = x * 8;
	mWB.minY = y * 8;
	mWB.maxX = mWB.minX + (w * 8);
	mWB.maxY = mWB.minY + (h * 8);
}

int TextBox::MaxScroll()
{
	return (startingLine + int(h - 5));
}