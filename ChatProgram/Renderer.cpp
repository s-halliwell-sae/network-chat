#include "Renderer.h"
#include "include\libtcod.hpp"
#include "IniManager.h"

namespace
{
	//Data for Windows Message Pump functionality
	WPARAM wparam;

	WNDPROC g_previousWNDPROC;

	int scrollThisFrame;
}

//Function for Windows Message Pump functionality
LRESULT CALLBACK CustomWindProc(
	_In_ HWND   hwnd,
	_In_ UINT   uMsg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
	);

Renderer::Renderer()
{

}

Renderer::~Renderer()
{

}

//Creates the TCOD window and sets up the GUI elements
void Renderer::SetupLayout(GUILayout style)
{
	int resX;
	int resY;

	//Pull values from the Config File
	IniManager::getInstance().GetInt("screenWidth", resX);
	IniManager::getInstance().GetInt("screenHeight", resY);

	//LOG("Pixel Res X: " + std::to_string(resX));
	//LOG("Pixel Res Y: " + std::to_string(resY));
	
	//Convert resolution to LIBTCOD character size
	resX /= 8;
	resY /= 8;

	//LOG("TCOD Res X: " + std::to_string(resX));
	//LOG("TCOD Res Y: " + std::to_string(resY));

	//Prevent the window from being too small to correctly function
	resX = CLAMP(30, 9999, resX);
	resY = CLAMP(30, 9999, resY);

	windowX = resX * 8;
	windowY = resY * 8;

	//LOG("Pixel Res X: " + std::to_string(windowX));
	//LOG("Pixel Res Y: " + std::to_string(windowY));

	//Initialise TCOD window
	TCODConsole::initRoot(resX, resY, "Client", false);
	int frameRate = 25;
	IniManager::getInstance().GetInt("frameRate", frameRate);
	TCODSystem::setFps(frameRate);
	TCODConsole::root->setDefaultForeground(TCODColor(255, 0, 0));
	TCODConsole::root->setDefaultBackground(TCODColor(0, 0, 0));
	
	//Get a handle of the new TCOD window
	hwnd = FindWindow(NULL, "Client");

	//Setup our message to track mouse scrolling from the Windows Message Pump
	g_previousWNDPROC = (WNDPROC)SetWindowLongPtr(
		hwnd,
		GWLP_WNDPROC,
		(LONG) &CustomWindProc
		);

	//Preset layouts for different GUI setups
	//Creates several TextBoxes and adds them to mRenderables
	if (style == GUILayout::CLIENT_SETUP)
	{
		
	}
	else if (style == GUILayout::CLIENT_CONNECTED)
	{
		mRenderables.push_back(new TextBox(int(resX * 0.15) * 2 - 1, 1, resX - int(resX * 0.15) * 2, int(resY * 0.8), 3, "Chat Log", "Callan: "));
		mRenderables.push_back(new TextBox(int(resX * 0.15), 1, int(resX * 0.15), int(resY * 0.8), 2, "Users", "- "));
		mRenderables.push_back(new TextBox(1, 1, int(resX * 0.15), int(resY * 0.8), 2, "Rooms", "- "));		

		mDynamicField = new DynamicTextBox(1, int(resY * 0.8), resX - 2, int(resY * 0.2) - 1, "Input Field");
		mRenderables.push_back(mDynamicField);
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

	/*POINT p;

	if (GetCursorPos(&p))
	{
		if (ScreenToClient(hwnd, &p))
		{
			LOG("X: " + std::to_string(p.x) + ", Y: " + std::to_string(p.y));
		}
	}*/

	//Handle inputs detected this frame
	ProcessScroll();
	mDynamicField->ProcessInput();

	//Check to see if any fields are out-of-date (Their internal data is different to what is being displayed
	if (CheckForChanges())
	{
		RenderAll();
	}
}

//Set the contents of a TextBox with a given string
//Note: the contents of an edittable box (DynamicTextBox) cannot be manually set this way
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

//Add a new element to 
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
	TCOD_key_t *key = mDynamicField->GetKey();

	if ((key->vk == TCODK_ENTER || key->vk == TCODK_KPENTER) && mDynamicField->GetContents().length() != 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Renderer::ProcessScroll()
{
	if (scrollThisFrame != 0)
	{
		POINT p;

		if (GetCursorPos(&p))
		{
			if (ScreenToClient(hwnd, &p))
			{
				if ((p.x >= 0 && p.x <= windowY) && (p.y >= 0 && p.y <= windowY))
				{
					for each (TextBox* box in mRenderables)
					{
						if (box->Collision(p.x, p.y))
						{
							//LOG(box->GetName());
							box->Scroll(scrollThisFrame);
							scrollThisFrame = 0;
							return;
						}
					}
				}
			}
		}
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

LRESULT CALLBACK CustomWindProc(
	_In_ HWND   hwnd,
	_In_ UINT   uMsg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
	)
{
	switch (uMsg)
	{
	case WM_MOUSEWHEEL:
		if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
		{
			scrollThisFrame = -1;
		}
		else
		{
			scrollThisFrame = 1;
		}
		break;
	}

	return CallWindowProc(g_previousWNDPROC, hwnd, uMsg, wParam, lParam);
}