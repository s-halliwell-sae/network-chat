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
#ifdef NC_SERVER
	TCODConsole::initRoot(resX, resY, "Server", false);
#elif defined NC_CLIENT
	TCODConsole::initRoot(resX, resY, "Client", false);
#endif
	int frameRate = 25;
	IniManager::getInstance().GetInt("frameRate", frameRate);
	TCODSystem::setFps(frameRate);

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
		//Pull the desired colors from the Config file
		TCODConsole::root->setDefaultForeground(GetColour("foreground"));
		TCODConsole::root->setDefaultBackground(GetColour("background"));

		//Setup the GUI elements
		mRenderables.push_back(new TextBox(int(resX * 0.15) * 2 - 1, 1, resX - int(resX * 0.15) * 2, int(resY * 0.8), 3, "Chat Log"));
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

//Find the RGB values stored in the Config file corresponding to the colour name given
TCODColor Renderer::GetColour(std::string colName)
{
	int r = 0;
	int g = 0;
	int b = 0;

	if (IniManager::getInstance().GetInt(colName + "r", r) && IniManager::getInstance().GetInt(colName + "g", g) && IniManager::getInstance().GetInt(colName + "b", b))
	{
		return TCODColor(CLAMP(0, 255, r), CLAMP(0, 255, g), CLAMP(0, 255, b));
	}

	LOGWARN("Values associated with colour: " + colName + " couldn't be found/used.");

	return TCODColor(0, 0, 0);
}

void Renderer::Update()
{
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

//Add a new element to a TextBox
//Note: A dynamic text box cannot have entries added to it
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

//Returns true if enter was pressed this frame, false otherwise
//Used to prevent User from having to access Windows Message Pump
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

//Finds if the mouse is within the Window, then finds the GUI element (if any) that it is hovering over
void Renderer::ProcessScroll()
{
	//If we've scrolled
	if (scrollThisFrame != 0)
	{
		//Get cursor position, translate it to local space (Within the window)
		POINT p;

		if (GetCursorPos(&p))
		{
			if (ScreenToClient(hwnd, &p))
			{
				//If the cursor is within the window
				if ((p.x > 0 && p.x < windowX) && (p.y > 0 && p.y < windowY))
				{
					for each (TextBox* box in mRenderables)
					{
						//Check if the mouse is over a given box
						if (box->Collision(p.x, p.y))
						{
							//Scroll the box, reset the scroll for the next frame
							box->Scroll(scrollThisFrame);
							scrollThisFrame = 0;
							return;
						}
					}
				}
			}
		}
		//If the user scrolled but wasn't hovered over anything at the time, the scroll is reset
		scrollThisFrame = 0;
	}
}

//Get the contents of the active dynamic field
//There can be only one dynamic field at a time, so this can be a simple function
std::string Renderer::RetrieveDynamicField()
{
	std::string temp = mDynamicField->GetContents();
	mDynamicField->Clear();
	mDynamicField->InSync = false;
	return temp;
}

//Check to see if a TextBox has had any changes made to it that would warrant a re-render
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

//Used to access the Windows Message Pump
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