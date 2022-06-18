#include "window-class.h"
#include "global.h"

namespace Win {
	/**
	*  コンストラクタ
	*  @param hInst<HINSTANCE>
	*  @param nWinMode<int>
	*/
	Window::Window(HINSTANCE hInst, int nWinMode) {
		WNDCLASSEX wc;
		wc.cbSize = sizeof(WNDCLASSEX);                                 //WNDCLASSEX構造体のサイズを設定
		wc.style = NULL;                                                //ウィンドウスタイル（デフォルト）
		wc.lpfnWndProc = WinProc;                                       //ウィンドウ関数
		wc.cbClsExtra = 0;                                              //通常は使わない（０にしておく）
		wc.cbWndExtra = 0;                                              //通常は使わない（０にしておく）
		wc.hInstance = hInst;                                           //このインスタンスへのハンドル
		wc.hIcon = NULL;                                                //ラージアイコン（なし）
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);                       //カーソルスタイル
		wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);         //ウィンドウの背景（黒）
		wc.lpszMenuName = NULL;                                         //メニュー（なし）
		wc.lpszClassName = windowName;                                  //ウィンドウクラス名
		wc.hIconSm = NULL;                                              //スモールアイコン（なし）

		if (!RegisterClassEx(&wc)) return;
		if (windowMode) {
			hWnd = CreateWindowEx(
				NULL,
				windowName,                                             //ウィンドウクラスの名前
				windowTitle,                                            //ウィンドウタイトル
				WS_OVERLAPPEDWINDOW ^ WS_MAXIMIZEBOX ^ WS_THICKFRAME,   //ウィンドウスタイル
				CW_USEDEFAULT,
				CW_USEDEFAULT,
				CW_USEDEFAULT,                                          //ウィンドウの幅
				CW_USEDEFAULT,                                          //ウィンドウの高さ
				NULL,                                                   //親ウィンドウ（なし）
				NULL,                                                   //メニュー（なし）
				hInst,                                                  //このプログラムのインスタンスのハンドル
				NULL                                                    //追加引数（なし）
			);
			RECT wRect, cRect;
			int ww, wh;
			int cw, ch;
			int Screen_PosX, Screen_PosY;
			GetWindowRect(hWnd, &wRect);
			ww = wRect.right - wRect.left;
			wh = wRect.bottom - wRect.top;
			GetClientRect(hWnd, &cRect);
			cw = cRect.right - cRect.left;
			ch = cRect.bottom - cRect.top;
			ww = ww - cw;
			wh = wh - ch;
			ww = SCREEN_WIDTH + ww;
			wh = SCREEN_HEIGHT + wh;
			Screen_PosX = (GetSystemMetrics(SM_CXSCREEN) - ww) / 2;
			Screen_PosY = (GetSystemMetrics(SM_CYSCREEN) - wh) / 2;
			hWnd = CreateWindowEx(
				NULL,
				windowName,                                             //ウィンドウクラスの名前
				windowTitle,                                            //ウィンドウタイトル
				WS_OVERLAPPEDWINDOW ^ WS_MAXIMIZEBOX ^ WS_THICKFRAME,   //ウィンドウスタイル
				CW_USEDEFAULT,
				CW_USEDEFAULT,
				CW_USEDEFAULT,                                          //ウィンドウの幅
				CW_USEDEFAULT,                                          //ウィンドウの高さ
				NULL,                                                   //親ウィンドウ（なし）
				NULL,                                                   //メニュー（なし）
				hInst,                                                  //このプログラムのインスタンスのハンドル
				NULL                                                    //追加引数（なし）
			);
			SetWindowPos(hWnd, HWND_TOP, Screen_PosX, Screen_PosY, ww, wh, NULL);
		}
		else {
			hWnd = CreateWindowEx(
				NULL,
				windowName,
				windowTitle,
				WS_POPUP,
				300,
				CW_USEDEFAULT,
				SCREEN_WIDTH,
				SCREEN_HEIGHT,
				NULL,
				NULL,
				hInst,
				NULL
			);
		}
		ShowWindow(hWnd, nWinMode); //ウィンドウを表示
		ValidateRect(hWnd, 0);		//WM_PAINTが呼ばれないようにする
		UpdateWindow(hWnd);			//ウィンドウの更新

		D3DDISPLAYMODE  DispMode;	//ディスプレイモード
		HRESULT         hr;
		Direct3D9 = Direct3DCreate9(D3D_SDK_VERSION);
		if (!Direct3D9) {           //オブジェクト生成失敗
			MessageBox(hWnd, _T("DirectXD3D9オブジェクト生成失敗"), _T("ERROR"), MB_OK);
		}
		//ディスプレイデータ格納構造体初期化
		ZeroMemory(&D3DPresentParameters, sizeof(D3DPRESENT_PARAMETERS));
		//現在のディスプレイモードデータ取得
		Direct3D9->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &DispMode);
		//バックバッファフォーマットをディスプレイと等価に
		D3DPresentParameters.BackBufferFormat = DispMode.Format;
		//横ドット幅設定
		D3DPresentParameters.BackBufferWidth = SCREEN_WIDTH;
		//縦ドット幅設定
		D3DPresentParameters.BackBufferHeight = SCREEN_HEIGHT;
		//バックバッファの数
		D3DPresentParameters.BackBufferCount = 1;
		//フリップの方法（通常はこの定数でよい）
		D3DPresentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
		//ウインドウモードの設定
		D3DPresentParameters.Windowed = windowMode;
		//ステンシルONOFF
		D3DPresentParameters.EnableAutoDepthStencil = FALSE;
		//ステンシルフォーマット
		D3DPresentParameters.AutoDepthStencilFormat = D3DFMT_UNKNOWN;
		//垂直同期OFF
		if (!vSync)
			D3DPresentParameters.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

		hr = Direct3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
			D3DCREATE_HARDWARE_VERTEXPROCESSING, &D3DPresentParameters, &Direct3DDevice9);
		if (FAILED(hr)) {
			//ハードウェアデバイスの生成を試みる
			hr = Direct3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING, &D3DPresentParameters, &Direct3DDevice9);
			if (FAILED(hr)) {
				//ソフトウェアデバイスの生成を試みる
				hr = Direct3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd,
					D3DCREATE_SOFTWARE_VERTEXPROCESSING, &D3DPresentParameters, &Direct3DDevice9);
				if (FAILED(hr)) {
					//--どのデバイスも取得できなかった
					MessageBox(hWnd, _T("DirectXデバイス生成失敗"), _T("ERROR"), MB_OK);
				}
			}
		}

		D3DXCreateSprite(Direct3DDevice9, &Sprite);
		Direct3DDevice9->SetRenderState(D3DRS_LIGHTING, FALSE);
		Direct3DDevice9->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		Direct3DDevice9->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		Direct3DDevice9->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		//SetTimer(hWnd, 0, 15, NULL);
		inputDevice = new InputDevice(hWnd);
	}

	/**
	*  デストラクタ
	*/
	Window::~Window() {
		if (Sprite != NULL) {
			Sprite->Release();
			Sprite = NULL;
		}
		if (Direct3DDevice9 != NULL) {
			Direct3DDevice9->Release();
			Direct3DDevice9 = NULL;
		}
		if (Direct3D9 != NULL) {
			Direct3D9->Release();
			Direct3D9 = NULL;
		}
		if (inputDevice != NULL) {
			delete inputDevice;
		}
	}

	/**
	*  描画する関数
	*/
	void Window::Rend() {
		rendering = true;
		if (isWindowActive) {
			Direct3DDevice9->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 0);
			if (SUCCEEDED(Direct3DDevice9->BeginScene())) {
				Render.Rend();
				Direct3DDevice9->EndScene();
			}
			Direct3DDevice9->Present(NULL, NULL, NULL, NULL);
		}
		rendering = false;
	}

	bool Window::getRendering() {
		return rendering;
	}

	/**
	*  ウィンドウプロシージャ関数.
	*  @param hWnd<HWND>
	*  @param message<UINT>
	*  @param wParam<WPARAM>
	*  @param lParam<LPARAM>
	*  @return <LRESULT>
	*/
	LRESULT CALLBACK Window::WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
		switch (message) {
		case WM_ACTIVATE:
			if (wParam == WA_INACTIVE) {
				if (((wParam >> 16) & 0xFFFF) == 0) {
					isWindowActive = true;
				}
				else {
					isWindowActive = false;
				}
			}
			else {
				isWindowActive = true;
				if (inputDevice != NULL) {
					if (inputDevice->getEnabled()) {
						inputDevice->acquire();
					}
				}
				else {
					inputDevice = new Device::InputDevice(hWnd);
				}
			}
			break;
		case WM_MOUSEMOVE:
			break;
		case WM_KEYDOWN:
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		return 0;
	}
}