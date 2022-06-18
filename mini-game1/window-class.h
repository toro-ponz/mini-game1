#pragma once

#include <Windows.h>
#include <d3dx9.h>
#include <stdio.h>
#include "render-class.h"

#define SCREEN_WIDTH         500
#define SCREEN_HEIGHT        500

using namespace Frame;

namespace Win {
	class Window {
	public:
		Window(HINSTANCE, int);
		~Window();
		void Rend();
		bool getRendering();
		Render Render;

	private:
		static LRESULT CALLBACK WinProc(HWND, UINT, WPARAM, LPARAM);
		static bool isWindowActive;
		TCHAR *windowName = _T("ミニゲーム by toro_ponz");
		TCHAR *windowTitle = _T("ミニゲーム by toro_ponz");
		D3DPRESENT_PARAMETERS D3DPresentParameters;
		bool windowMode = true;
		bool vSync = true;
		bool rendering = false;
	};
}