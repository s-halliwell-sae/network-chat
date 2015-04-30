#include "Renderer.h"
#include "include\libtcod.hpp"
#include "IniManager.h"

namespace
{
	WPARAM wparam;

	WNDPROC g_previousWNDPROC;

	int scrollThisFrame;
}

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

void Renderer::SetupLayout(GUILayout style)
{
	int resX;
	int resY;

	//Pull values from the Config File
	IniManager::getInstance().GetInt("screenWidth", resX);
	IniManager::getInstance().GetInt("screenHeight", resY);

	//Convert resolution to LIBTCOD character size
	resX /= 8;
	resY /= 8;

	//Prevent the window from being too small to correctly function
	resX = CLAMP(30, 9999, resX);
	resY = CLAMP(30, 9999, resY);

	//Initialise TCOD window
	TCODConsole::initRoot(resX, resY, "Client", false);
	TCODSystem::setFps(25);
	TCODConsole::root->setDefaultForeground(TCODColor(255, 0, 0));
	TCODConsole::root->setDefaultBackground(TCODColor(0, 0, 0));
	
	//
	hwnd = FindWindow(NULL, "Client");

	RECT rect;
	if (GetWindowRect(hwnd, &rect))
	{
		windowX = rect.right - rect.left;
		windowY = rect.bottom - rect.top;
	}

	g_previousWNDPROC = (WNDPROC)SetWindowLongPtr(
		hwnd,
		GWLP_WNDPROC,
		(LONG) &CustomWindProc
		);

	if (style == GUILayout::CLIENT_SETUP)
	{
		
	}
	else if (style == GUILayout::CLIENT_CONNECTED)
	{
		int stepX = resX / 10;
		int stepY = resY / 10;
		mRenderables.push_back(new TextBox((stepX * 3) - 1, 1, stepX * 7, (stepY * 8), 3,  "Chat Log", "Callan: "));
		mRenderables.push_back(new TextBox(int(stepX * 1.5), 1, int(stepX * 1.5), (stepY * 8), 2, "Users", "- "));
		mRenderables.push_back(new TextBox(1, 1, int(stepX * 1.5), (stepY * 8), 2, "Rooms", "- "));
		mDynamicField = new DynamicTextBox(1, (stepY * 8), resX - 2, (stepY * 2) - 1, "Input Field");
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
	ProcessScroll();
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
	TCOD_key_t *key = mDynamicField->GetKey();

	if (key->vk == TCODK_ENTER || key->vk == TCODK_KPENTER)
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