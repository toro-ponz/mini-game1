#include "input-device.h"

namespace Device {
	/**
	*  コンストラクタ
	*  @param hWnd<HWND> ウィンドウハンドル
	*/
	Keyboard::Keyboard(HWND hWnd) {
		if (SUCCEEDED(DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&DirectInput8, NULL))) {
			if (SUCCEEDED(DirectInput8->CreateDevice(GUID_SysKeyboard, &DirectInputDevice8, NULL))) {
				DirectInputDevice8->SetDataFormat(&c_dfDIKeyboard);
				DirectInputDevice8->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
				DirectInputDevice8->Acquire();
				loadKeyNumber();
				keyboardEnabled = true;
			}
			else {
				keyboardEnabled = false;
			}
		}
		else {
			keyboardEnabled = false;
		}
	}

	/**
	*  デストラクタ
	*/
	Keyboard::~Keyboard() {
		if (keyboardEnabled) {
			DirectInputDevice8->Unacquire();
		}
		if (DirectInputDevice8 != NULL) {
			DirectInputDevice8->Release();
			DirectInputDevice8 = NULL;
		}
		if (DirectInput8 != NULL) {
			DirectInput8->Release();
			DirectInput8 = NULL;
		}
	}

	/**
	*  キーボードが接続されており、正しく動作するか判定する関数.
	*  @return <bool> keyboardEnabled
	*/
	bool Keyboard::getEnabled() {
		return keyboardEnabled;
	}

	/**
	*  全ボタンの押下状態を保存する関数.
	*  @return <bool> 正常に保存できればtrue, そうでなければfalse
	*/
	bool Keyboard::getPushState() {
		memcpy(oldKeyState, currentKeyState, sizeof(currentKeyState));
		if (keyboardEnabled) {
			if (SUCCEEDED(DirectInputDevice8->GetDeviceState(sizeof(currentKeyState), (LPVOID)&currentKeyState))) {
				return true;
			}
			else {
				DirectInputDevice8->Acquire();
				if (SUCCEEDED(DirectInputDevice8->GetDeviceState(sizeof(currentKeyState), (LPVOID)&currentKeyState))) {
					return true;
				}
				else {
					return false;
				}
			}
		}
		else {
			return false;
		}
	}

	/**
	*  指定ボタンが押されているかどうか判定する関数.
	*  @param key<int> キー番号
	*  @param mode<int> 0:押されているかどうか, 1:今押されたかどうか
	*  @return <bool> 判定結果
	*/
	bool Keyboard::getPushState(int key, int mode) {
		if (keyboardEnabled) {
			if (!mode) {
				if (currentKeyState[keyNumber[key]] & 0x80) {
					return true;
				}
				else {
					return false;
				}
			}
			else {
				if ((currentKeyState[keyNumber[key]] & 0x80) && !(oldKeyState[keyNumber[key]] & 0x80)) {
					return true;
				}
				else {
					return false;
				}
			}
		}
		else {
			return false;
		}
	}

	/**
	*  1つでもボタンが押されているかどうか判定する関数.
	*  @param mode<int> 0:押されているかどうか, 1:今押されたかどうか
	*  @return <bool> 判定結果
	*/
	bool Keyboard::getPushStateAny(int mode) {
		if (keyboardEnabled) {
			if (!mode) {
				for (int i = 0; i < sizeof(currentKeyState); i++) {
					if (currentKeyState[i] & 0x80) {
						return true;
					}
				}
				return false;
			}
			else {
				for (int i = 0; i < sizeof(currentKeyState); i++) {
					if ((currentKeyState[i] & 0x80) && !(oldKeyState[i] & 0x80)) {
						return true;
					}
				}
				return false;
			}
			return true;
		}
		else {
			return false;
		}
	}

	/**
	*  押されているキーナンバーを返す関数.
	*  @param mode<int> 0:押されているかどうか, 1:今押されたかどうか
	*  @return <int> キーナンバー
	*/
	int Keyboard::getPushedKeyNumber(int mode) {
		if (keyboardEnabled) {
			if (!mode) {
				for (int i = 0; i < sizeof(currentKeyState); i++) {
					if (currentKeyState[i] & 0x80) {
						return i;
					}
				}
				return NULL;
			}
			else {
				for (int i = 0; i < sizeof(currentKeyState); i++) {
					if ((currentKeyState[i] & 0x80) && !(oldKeyState[i] & 0x80)) {
						return i;
					}
				}
				return NULL;
			}
		}
		else {
			return NULL;
		}
	}

	/**
	* キー割り当てをconf.iniから読み込む関数.
	*/
	void Keyboard::loadKeyNumber() {
		TCHAR* configFilePath = _T("Data/conf.ini");
		keyNumber[0] = GetPrivateProfileInt(_T("keyboard"), _T("arrow-up"), DIK_W, configFilePath);
		keyNumber[1] = GetPrivateProfileInt(_T("keyboard"), _T("arrow-down"), DIK_S, configFilePath);
		keyNumber[2] = GetPrivateProfileInt(_T("keyboard"), _T("arrow-left"), DIK_A, configFilePath);
		keyNumber[3] = GetPrivateProfileInt(_T("keyboard"), _T("arrow-right"), DIK_D, configFilePath);
		keyNumber[4] = GetPrivateProfileInt(_T("keyboard"), _T("button-up"), DIK_I, configFilePath);
		keyNumber[5] = GetPrivateProfileInt(_T("keyboard"), _T("button-down"), DIK_K, configFilePath);
		keyNumber[6] = GetPrivateProfileInt(_T("keyboard"), _T("button-left"), DIK_J, configFilePath);
		keyNumber[7] = GetPrivateProfileInt(_T("keyboard"), _T("button-right"), DIK_L, configFilePath);
		keyNumber[8] = GetPrivateProfileInt(_T("keyboard"), _T("button-select"), DIK_F1, configFilePath);
		keyNumber[9] = GetPrivateProfileInt(_T("keyboard"), _T("button-start"), DIK_F2, configFilePath);
		keyNumber[10] = GetPrivateProfileInt(_T("keyboard"), _T("trigger-left"), DIK_Q, configFilePath);
		keyNumber[11] = GetPrivateProfileInt(_T("keyboard"), _T("trigger-right"), DIK_O, configFilePath);
	}

	/**
	*  入力デバイスを再開する関数.
	*  @return <HRESULT> デバイスが有効ならAcquire関数の戻り値, 無効ならE_FAIL
	*/
	HRESULT Keyboard::acquire() {
		if (keyboardEnabled) {
			return DirectInputDevice8->Acquire();
		}
		else {
			return E_FAIL;
		}
	}

	/**
	*  コンストラクタ
	*  @param hWnd<HWND> ウィンドウハンドル
	*/
	Joystick::Joystick(HWND hWnd) {
		HINSTANCE hInst = GetModuleHandle(NULL);
		//DirectInput8インターフェイスの取得
		if (SUCCEEDED(DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&DirectInput8, NULL))) {
			if (SUCCEEDED(DirectInput8->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback, NULL, DIEDFL_ATTACHEDONLY))) {
				if (DirectInputDevice8 != NULL) {
					DirectInputDevice8->SetDataFormat(&c_dfDIJoystick);
					// 協調モードの設定
					DirectInputDevice8->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
					diDevCaps.dwSize = sizeof(DIDEVCAPS);
					DirectInputDevice8->GetCapabilities(&diDevCaps);
					DirectInputDevice8->EnumObjects(EnumAxesCallback, (VOID*)hWnd, DIDFT_AXIS);
					// 入力の開始
					DirectInputDevice8->Poll();
					DirectInputDevice8->Acquire();
					loadKeyNumber();
					joystickEnabled = true;
				}
				else {
					joystickEnabled = false;
				}
			}
			else {
				joystickEnabled = false;
			}
		}
		else {
			joystickEnabled = false;
		}
	}

	/**
	*  デストラクタ
	*/
	Joystick::~Joystick() {
		if (joystickEnabled) {
			DirectInputDevice8->Unacquire();
		}
		if (DirectInputDevice8 != NULL) {
			DirectInputDevice8->Release();
			DirectInputDevice8 = NULL;
		}
		if (DirectInput8 != NULL) {
			DirectInput8->Release();
			DirectInput8 = NULL;
		}
	}

	/**
	*  ジョイスティックが接続されており、正しく動作するか判定する関数.
	*  @return <bool> joystickEnabled
	*/
	bool Joystick::getEnabled() {
		return joystickEnabled;
	}

	/**
	*  全ボタンの押下状態を保存する関数.
	*  @return <bool> joystickEnabled
	*/
	bool Joystick::getPushState() {
		if (joystickEnabled) {
			for (int i = 0; i < 20; i++) {
				//前回の状態をold変数に保存する
				oldButtonState[i] = currentButtonState[i];
				//今回の状態を変数に保存する
				currentButtonState[i] = getPushState(i);
			}
			return true;
		}
		else {
			return false;
		}
	}

	/**
	*  指定ボタンが押されているかどうか判定する関数.
	*  @param key<int> キー番号
	*  @return <bool> 判定結果
	*/
	bool Joystick::getPushState(int key) {
		if (joystickEnabled) {
			bool result = false;
			HRESULT hr = DirectInputDevice8->GetDeviceState(sizeof(DIJOYSTATE), &js);
			if (key == 0) {
				if (js.lY == -1000)
					result = true;
			}
			else if (key == 1) {
				if (js.lY == 1000)
					result = true;
			}
			else if (key == 2) {
				if (js.lX == -1000)
					result = true;
			}
			else if (key == 3) {
				if (js.lX == 1000)
					result = true;
			}
			else {
				if (js.rgbButtons[keyNumber[key] - 4] & 0x80)
					result = true;
			}
			return result;
		}
		else {
			return false;
		}
	}

	/**
	*  指定ボタンが押されているかどうか判定する関数.
	*  @param key<int> キー番号
	*  @param mode<int> 0:押されているかどうか, 1:今押されたかどうか
	*  @return <bool> 判定結果
	*/
	bool Joystick::getPushState(int key, int mode) {
		if (joystickEnabled) {
			bool result = false;
			if (!mode) {
				result = currentButtonState[key];
			}
			else {
				if ((oldButtonState[key] == false) && (currentButtonState[key] == true)) result = true;
			}
			return result;
		}
		else {
			return false;
		}
	}

	/**
	*  1つでもボタンが押されているかどうか判定する関数.
	*  @param mode<int> 0:押されているかどうか, 1:今押されたかどうか
	*  @return <bool> 判定結果
	*/
	bool Joystick::getPushStateAny(int mode) {
		if (joystickEnabled) {
			if (!mode) {
				for (int i = 0; i < 20; i++) {
					if (currentButtonState[i] == true) {
						return true;
					}
				}
				return false;
			}
			else {
				for (int i = 0; i < 20; i++) {
					if ((oldButtonState[i] == false) && (currentButtonState[i] == true)) {
						return true;
					}
				}
				return false;
			}
		}
		else {
			return false;
		}
	}

	/**
	*  押されているキーナンバーを返す関数.
	*  @param mode<int> 0:押されているかどうか, 1:今押されたかどうか
	*  @return <int> キーナンバー
	*/
	int Joystick::getPushedKeyNumber(int mode) {
		if (joystickEnabled) {
			if (!mode) {
				for (int i = 0; i < 20; i++) {
					if (currentButtonState[i] == true) {
						return keyNumber[i] + 1;
					}
				}
				return NULL;
			}
			else {
				for (int i = 0; i < 20; i++) {
					if ((oldButtonState[i] == false) && (currentButtonState[i] == true)) {
						return keyNumber[i] + 1;
					}
				}
				return NULL;
			}
		}
		else {
			return NULL;
		}
	}

	/**
	* キー割り当てをconf.iniから読み込む関数.
	*/
	void Joystick::loadKeyNumber() {
		TCHAR* configFilePath = _T("Data/conf.ini");
		keyNumber[0] = GetPrivateProfileInt(_T("joystick"), _T("arrow-up"), 1, configFilePath) - 1;
		keyNumber[1] = GetPrivateProfileInt(_T("joystick"), _T("arrow-down"), 2, configFilePath) - 1;
		keyNumber[2] = GetPrivateProfileInt(_T("joystick"), _T("arrow-left"), 3, configFilePath) - 1;
		keyNumber[3] = GetPrivateProfileInt(_T("joystick"), _T("arrow-right"), 4, configFilePath) - 1;
		keyNumber[4] = GetPrivateProfileInt(_T("joystick"), _T("button-up"), 5, configFilePath) - 1;
		keyNumber[5] = GetPrivateProfileInt(_T("joystick"), _T("button-down"), 6, configFilePath) - 1;
		keyNumber[6] = GetPrivateProfileInt(_T("joystick"), _T("button-left"), 7, configFilePath) - 1;
		keyNumber[7] = GetPrivateProfileInt(_T("joystick"), _T("button-right"), 8, configFilePath) - 1;
		keyNumber[10] = GetPrivateProfileInt(_T("joystick"), _T("trigger-left"), 9, configFilePath) - 1;
		keyNumber[11] = GetPrivateProfileInt(_T("joystick"), _T("trigger-right"), 10, configFilePath) - 1;
		keyNumber[8] = GetPrivateProfileInt(_T("joystick"), _T("button-select"), 11, configFilePath) - 1;
		keyNumber[9] = GetPrivateProfileInt(_T("joystick"), _T("button-start"), 12, configFilePath) - 1;
	}

	/**
	*  入力デバイスを再開する関数.
	*  @return <HRESULT> デバイスが有効ならAcquire関数の戻り値, 無効ならE_FAIL
	*/
	HRESULT Joystick::acquire() {
		if (joystickEnabled) {
			return DirectInputDevice8->Acquire();
		}
		else {
			return E_FAIL;
		}
	}

	BOOL CALLBACK Joystick::EnumJoysticksCallback(const DIDEVICEINSTANCE *pdidInstance, VOID *pContext) {
		if (FAILED(DirectInput8->CreateDevice(pdidInstance->guidInstance, &DirectInputDevice8, NULL))) {
			return DIENUM_CONTINUE;
		}
		else {
			return DIENUM_STOP;
		}
	}

	BOOL CALLBACK Joystick::EnumAxesCallback(const DIDEVICEOBJECTINSTANCE *pdidoi, VOID *pContext) {
		DIPROPRANGE diprg;
		diprg.diph.dwSize = sizeof(DIPROPRANGE);
		diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		diprg.diph.dwHow = DIPH_BYID;
		diprg.diph.dwObj = pdidoi->dwType;
		diprg.lMin = 0 - 1000;
		diprg.lMax = 0 + 1000;

		if (FAILED(DirectInputDevice8->SetProperty(DIPROP_RANGE, &diprg.diph))) {
			return DIENUM_STOP;
		}
		else {
			return DIENUM_CONTINUE;
		}
	}

	/**
	*  コンストラクタ
	*  @param hWnd<HWND> ウィンドウハンドル
	*/
	InputDevice::InputDevice(HWND hWnd) :
		joystick(hWnd),
		keyboard(hWnd) {

	}

	/**
	*  デストラクタ
	*/
	InputDevice::~InputDevice() {

	}

	/**
	*  入力デバイスの少なくともどちらかが有効であるかを判定する関数.
	*  @return <bool> 判定結果
	*/
	bool InputDevice::getEnabled() {
		return joystick.getEnabled() || keyboard.getEnabled();
	}

	/**
	*  全ボタンの押下状態を保存する関数.
	*  @return <bool> 正常に保存できればtrue, そうでなければfalse
	*/
	bool InputDevice::getPushState() {
		bool joy = joystick.getPushState();
		bool key = keyboard.getPushState();
		return joy || key;
	}

	/**
	*  指定ボタンが押されているかどうか判定する関数.
	*  @param key<KEY> キー番号
	*  @param mode<KEY_STATE> STATE_HAVE_PUSHED:押されているかどうか, STATE_PUSH:今押されたかどうか
	*  @return <bool> 判定結果
	*/
	bool InputDevice::getPushState(KEY key, KEY_STATE mode) {
		bool joy;
		if (key >= KEY::RETURN)
			joy = false;
		else
			joy = joystick.getPushState(joystickKeys[key], mode);
		bool kb;
		if (key == KEY::ARROW_UP || key == KEY::ARROW_DOWN || key == KEY::ARROW_LEFT || key == KEY::ARROW_RIGHT)
			kb = keyboard.getPushState(keyboardKeys[key], mode) || keyboard.getPushState(keyboardKeys[key + 14], mode);
		else
			kb = keyboard.getPushState(keyboardKeys[key], mode);
		return joy || kb;
	}

	/**
	*  1つでもボタンが押されているかどうか判定する関数.
	*  @param mode<KEY_STATE> STATE_HAVE_PUSHED:押されているかどうか, STATE_PUSH:今押されたかどうか
	*  @return <bool> 判定結果
	*/
	bool InputDevice::getPushStateAny(KEY_STATE mode) {
		return joystick.getPushStateAny(mode) || keyboard.getPushStateAny(mode);
	}

	/**
	*  キーボードの1つでもキーが押されているかどうか判定する関数.
	*  @param mode<KEY_STATE> STATE_HAVE_PUSHED:押されているかどうか, STATE_PUSH:今押されたかどうか
	*  @return <bool> 判定結果
	*/
	bool InputDevice::getKeyboardPushStateAny(KEY_STATE mode) {
		return keyboard.getPushStateAny(mode);
	}

	/**
	*  ジョイスティックの1つでもキーが押されているかどうか判定する関数.
	*  @param mode<KEY_STATE> STATE_HAVE_PUSHED:押されているかどうか, STATE_PUSH:今押されたかどうか
	*  @return <bool> 判定結果
	*/
	bool InputDevice::getJoystickPushStateAny(KEY_STATE mode) {
		return joystick.getPushStateAny(mode);
	}

	/**
	*  押されているキーナンバーを返す関数.
	*  @param mode<KEY_STATE> STATE_HAVE_PUSHED:押されているかどうか, STATE_PUSH:今押されたかどうか
	*  @return <int> キーナンバー
	*/
	int InputDevice::getKeyboardPushKeyNumber(KEY_STATE mode) {
		return keyboard.getPushedKeyNumber(mode);
	}

	/**
	*  押されているキーナンバーを返す関数.
	*  @param mode<KEY_STATE> STATE_HAVE_PUSHED:押されているかどうか, STATE_PUSH:今押されたかどうか
	*  @return <int> キーナンバー
	*/
	int InputDevice::getJoystickPushKeyNumber(KEY_STATE mode) {
		return joystick.getPushedKeyNumber(mode);
	}

	/**
	* キー割り当てをconf.iniから読み込む関数.
	*/
	void InputDevice::loadKeyNumber() {
		keyboard.loadKeyNumber();
		joystick.loadKeyNumber();
	}

	/**
	*  入力デバイスを再開する関数.
	*  @return <bool> どちらかのデバイスが再開できればtrue
	*/
	bool InputDevice::acquire() {
		bool joy = (bool)SUCCEEDED(joystick.acquire());
		bool key = (bool)SUCCEEDED(keyboard.acquire());
		return joy || key;
	}
}