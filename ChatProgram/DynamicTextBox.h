#ifndef DYNAMICTEXTBOX_H
#define DYNAMICTEXTBOX_H

#include "TextBox.h"
#include "include\libtcod.hpp"

class DynamicTextBox : public TextBox
{
public:

	DynamicTextBox(float posX, float posY, float height, float width, std::string title);
	DynamicTextBox(float posX, float posY, float height, float width, std::string title, std::string prefixIn);

	void ProcessInput();
	void Render();
	void Clear();

	void AddEntry(std::string newEntry);

	void SetContents(std::vector<std::string> contentsIn);
	const std::string& GetContents();

	TCOD_key_t *GetKey(){ return &mKey; };

private:
	std::string contents;
	TCOD_key_t mKey;
};

#endif