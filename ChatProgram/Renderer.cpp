#include "Renderer.h"
#include "include\libtcod.hpp"
#include "IniManager.h"

Renderer::Renderer()
{

}

Renderer::~Renderer()
{

}

void Renderer::SetupLayout(GUILayout style)
{
	int resX;
	int resY;
	IniManager::getInstance().GetInt("screenWidth", resX);
	IniManager::getInstance().GetInt("screenHeight", resY);

	TCODConsole::initRoot(resX, resY, "Client", false);
	TCODSystem::setFps(25); // limit framerate to 25 frames per second
	TCODConsole::root->setDefaultForeground(TCODColor(255, 0, 0));
	TCODConsole::root->setDefaultBackground(TCODColor(0, 0, 0));

	if (style == GUILayout::CLIENT_SETUP)
	{
		
	}
	else if (style == GUILayout::CLIENT_CONNECTED)
	{
		//LOG(std::to_string(int((resY * 0.8) + 0.5) - 2));
		int stepX = resX / 10;
		int stepY = resY / 10;
		mRenderables.push_back(new TextBox((stepX * 3) - 1, 1, stepX * 7, (stepY * 8), "Chat Log", "Callan: "));
		mRenderables.push_back(new TextBox(int(stepX * 1.5), 1, int(stepX * 1.5), (stepY * 8), "Users", "- "));
		mRenderables.push_back(new TextBox(1, 1, int(stepX * 1.5), (stepY * 8), "Rooms", "- "));
		mDynamicField = new DynamicTextBox(1, (stepY * 8), resX - 2, (stepY * 2) - 1, "Input Field");
		mRenderables.push_back(mDynamicField);
		LOG(std::to_string((stepY * 2) - 1));
	}
	else if (style == GUILayout::SERVER_SETUP)
	{

	}
	else
	{

	}
}

void Renderer::Update()
{
	mDynamicField->ProcessInput();
	if (CheckForChanges())
	{
		RenderAll();
	}
}

bool Renderer::SetContents(std::string key, std::vector<std::string> newContents)
{
	for each (TextBox* box in mRenderables)
	{
		if (box->GetName() == key)
		{
			box->SetContents(newContents);
			return true;
		}
	}
	LOGWARN("TextBox with name " + key + " was not found.");
	return false;
}

bool Renderer::AddEntry(std::string key, std::string newEntry)
{
	for each (TextBox* box in mRenderables)
	{
		if (box->GetName() == key)
		{
			box->AddEntry(newEntry);
			return true;
		}
	}
	LOGWARN("TextBox with name " + key + " was not found.");
	return false;
}

bool Renderer::PressedEnter()
{
	//TCOD_key_t key = TCODConsole::checkForKeypress();
	TCOD_key_t *key = mDynamicField->GetKey();

	//if (key.vk == TCODK_ENTER || key.vk == TCODK_KPENTER)
	if (key->vk == TCODK_ENTER || key->vk == TCODK_KPENTER)
	{
		return true;
	}
	else
	{
		return false;
	}
}

std::string Renderer::RetrieveDynamicField()
{
	std::string temp = mDynamicField->GetContents();
	mDynamicField->Clear();
	mDynamicField->InSync = false;
	return temp;
}

bool Renderer::CheckForChanges()
{
	for each (TextBox* box in mRenderables)
	{
		if (!box->InSync)
		{
			//A change was made
			return true;
		}
	}
	return false;
}

void Renderer::RenderAll()
{
	TCODConsole::root->clear();
	for each (TextBox* box in mRenderables)
	{
		box->Render();
		box->InSync = true;
	}
	TCODConsole::root->flush();
}