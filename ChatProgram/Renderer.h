#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include "TextBox.h"
#include "DynamicTextBox.h"

class Renderer
{
public:
	Renderer();
	~Renderer();

	enum GUILayout { CLIENT_SETUP, CLIENT_CONNECTED, SERVER_SETUP, SERVER_CONNECTED };

	void SetupLayout(GUILayout style);
	void Update();

	bool AddEntry(std::string key, std::string newEntry);
	bool SetContents(std::string key, std::vector<std::string> newContents);

	bool PressedEnter();

	std::string RetrieveDynamicField();

private:

	void RenderAll();

	bool CheckForChanges();

	std::vector<TextBox*> mRenderables;

	DynamicTextBox* mDynamicField;
};

#endif