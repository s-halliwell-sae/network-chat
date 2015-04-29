#ifndef TEXTBOX_H
#define TEXTBOX_H

#include <vector>
#include "Logger.h"

class TextBox
{
public:

	TextBox(float posX, float posY, float height, float width, std::string title);
	TextBox(float posX, float posY, float height, float width, std::string title, std::string prefixIn);
	~TextBox();
	
	//void ProcessInput();
	virtual void Render();
	virtual void Clear();
	
	void SetContents(std::vector<std::string> contentsIn);
	virtual void AddEntry(std::string newEntry);

	bool InSync = false;

	std::string GetName();

	std::string prefix = "";

	void Scroll(int factor);

protected:
	std::string name;

	int startingLine = 0;

	//Position
	float x;
	float y;

	//Dimensions
	float w;
	float h;

	float cursorX;
	float cursorY;

	void RenderFrame();
	void PrintLine(std::string& line);
	void ResetCursor();

	virtual int NumLines();

	int LinesOnScreen(int contentLength);

private:
	std::vector<std::string> contents;
};

#endif