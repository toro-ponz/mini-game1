#include "global.h"
#include <vector>

namespace Global {
	HWND hWnd;
	LPD3DXSPRITE Sprite;
	LPDIRECT3DDEVICE9 Direct3DDevice9;
	LPDIRECT3D9 Direct3D9 = NULL;
	InputDevice *inputDevice;
	Frame::FrameNumber frameNumber;

	const string dataDirectoryPath = "./Data/";
	const string soundEffectDirectoryPath = dataDirectoryPath + "sound-effect/";
	const string imageDirectoryPath = dataDirectoryPath + "img/";
	const string colorDirectoryPath = dataDirectoryPath + "img/color/";
	const string backgorundMusicDirectoryPath = dataDirectoryPath + "bgm/";
	const string songsDirectoryPath = dataDirectoryPath + "songs/";
	const string savedataFilePath = dataDirectoryPath + "savedata.dat";
	const string configFilePath = dataDirectoryPath + "conf.ini";

	const string configImageDirectoryPath = imageDirectoryPath + "config/";
	const string creditImageDirectoryPath = imageDirectoryPath + "credit/";
	const string openingImageDirectoryPath = imageDirectoryPath + "opening/";
	const string playImageDirectoryPath = imageDirectoryPath + "play/";
	const string resultImageDirectoryPath = imageDirectoryPath + "result/";
	const string levelSelectImageDirectoryPath = imageDirectoryPath + "level-select/";

	const string resourcePath = "";

	const string fontName = "メイリオ";

	/**
	*  const char*型の文字列をchar*型に変換する関数.
	*  @param letters<const char*> 変換する文字列のポインタ
	*  @return <char*> 変換された文字列のポインタ
	*/
	char* convertConstCharToChar(const char* letters) {
		size_t length = strlen(letters);
		char *result = new char[length + 1];
		if (!strcpy_s(result, length + 1, letters))
			return result;
		else
			return NULL;
	}

	/**
	*  const char*型の文字列をwchar_t*型に変換する関数.
	*  @param letters<const char*> 変換する文字列のポインタ
	*  @return <wchar_t*> 変換された文字列のポインタ
	*/
	wchar_t* convertCharToWchar_t(const char *letters) {
		size_t length = strlen(letters);
		wchar_t* result = new wchar_t[length + 1];
		setlocale(LC_ALL, "japanese");
		if (!mbstowcs_s(&length, result, length + 1, letters, _TRUNCATE))
			return result;
		else
			return NULL;
	}

	/**
	*  const wchar_t*型の文字列をchar*型に変換する関数.
	*  @param letters<const wchar_t*> 変換する文字列のポインタ
	*  @return <char*> 変換された文字列のポインタ
	*/
	char* convertWchar_tToChar(const wchar_t *letters) {
		size_t length = (wcslen(letters) + 1) * 2;
		char *result = new char[length + 1];
		setlocale(LC_ALL, "japanese");
		if (!wcstombs_s(&length, result, length, letters, _TRUNCATE))
			return result;
		else
			return NULL;
	}

	/**
	*  const TCHAR*型の文字列をstring型に変換する関数.
	*  @param letters<const TCHAR*> 変換する文字列のポインタ
	*  @return <string> 変換された文字列
	*/
	string convertTcharToString(const TCHAR *letters) {
#ifdef UNICODE
		return convertWchar_tToChar(letters);
#else
		return letters;
#endif
	}

	/**
	*  const string型の文字列をTCHAR*型に変換する関数.
	*  @param letters<const string> 変換する文字列
	*  @return <TCHAR*> 変換された文字列のポインタ
	*/
	TCHAR* convertStringToTchar(const string letters) {
#ifdef UNICODE
		return convertCharToWchar_t(letters.c_str());
#else
		return convertConstCharToChar(letters.c_str());
#endif
	}

	/**
	*  SEを再生する関数.
	*  @param path<string> ファイルパス
	*/
	void playSoundEffectFromFile(string path) {
		if (path != "") {
			PlaySound(convertStringToTchar(path), NULL, SND_FILENAME | SND_ASYNC);
		}
	}

	/**
	*  SEを再生する関数.
	*  @param suffix<int> リソース番号
	*/
	void playSoundEffectFromResource(int suffix) {

	}
}