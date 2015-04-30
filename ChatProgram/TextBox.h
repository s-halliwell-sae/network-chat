#ifndef TEXTBOX_H
#define TEXTBOX_H

#include <vector>
#include "Logger.h"
#include "WindowBounds.h"

class TextBox
{
public:

	TextBox(float posX, float posY, float height, float width, std::string title);
	TextBox(float posX, float posY, float height, float width, std::string title, std::string prefixIn);
	
	TextBox(float posX, float posY, float width, float height, int scrollSpeed, std::string title);
	TextBox(float posX, float posY, float width, float height, int scrollSpeed, std::string title, std::string prefixIn);
	
	~TextBox();
	
	//void ProcessInput();
	virtual void Render();
	virtual void Clear();
	
	void SetContents(std::vector<std::string> contentsIn);
	virtual void AddEntry(std::string newEntry);

	bool InSync = false;

	std::string GetName();

	void Scroll(int factor);

	bool Collision(int x, int y);

protected:
	std::string name;

	std::string prefix = "";

	int startingLine = 0;

	//Position
	float x;
	float y;

	//Dimensions
	float w;
	float h;

	WindowBounds mWB;

	int curLine;
	float cursorX;
	float cursorY;

	void RenderFrame();
	void PrintLine(std::string& line);
	void ResetCursor();

	virtual int NumLines();

	int LinesOnScreen(int contentLength);

	void SetWindowDimensions();

	int mScrollSpeed;

private:
	std::vector<std::string> contents;
};

#endif