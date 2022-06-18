#include "mini-game1.h"

int WINAPI _tWinMain(HINSTANCE hThisInst, HINSTANCE hPrevInst, LPTSTR lpszArgs, int nWinMode) {
	MSG msg;
	Win::Window window(hThisInst, nWinMode);
	while (TRUE) {
		if (!window.getRendering()) {
			window.Rend();
		}
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
			if (!GetMessage(&msg, NULL, 0, 0)) break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return msg.wParam;
}