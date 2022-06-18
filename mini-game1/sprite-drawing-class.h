#pragma once

#include "global.h"
#include <string>
#include <vector>
#include <tchar.h>
#include <d3d9.h>
#include <d3dx9.h>

using std::vector;
using std::string;

namespace DxSprite {
	class SpriteDrawing {
	public:
		SpriteDrawing(int = 1, float = 32.0f, float = -1);
		SpriteDrawing(TCHAR*, int = 1, float = 32.0f, float = -1);
		SpriteDrawing(string, int = 1, float = 32.0f, float = -1);
		SpriteDrawing(LPDIRECT3DTEXTURE9, int = 1, float = 32.0f, float = -1);
		~SpriteDrawing();
		HRESULT setTexture(TCHAR*);
		HRESULT setTexture(string);
		void setTexture(LPDIRECT3DTEXTURE9);
		void setRectFromPixel(int, LONG, LONG, LONG, LONG);
		void setRectFromChip(int, int, int = -1);
		void setCenter(int, float = 0.0f, float = 0.0f, float = 0.0f);
		void setPosition(int, float = 0.0f, float = 0.0f, float = 0.0f);
		void setColor(int, D3DCOLOR);
		int getElementCount();
		float addPositionX(int, float);
		float addPositionY(int, float);
		bool addAlpha(int, int);
		void addAlpha(int, DWORD*, int);
		void setAnimationInterval(int, int, int, DWORD*, unsigned int, unsigned int, DWORD = 0);
		void disableDraw(int);
		void enableDraw(int);
		void setChipPixel(float = 32.0f, float = -1);
		void reserve(int);
		void Draw();

	private:
		void initialize();
		void culcAnimation(int, DWORD);
		LPDIRECT3DTEXTURE9 texture = NULL;
		vector<RECT> rect;
		vector<D3DXVECTOR3> center;
		vector<D3DXVECTOR3> position;
		vector<D3DCOLOR> color;
		vector<DWORD> animationOldTime;
		vector<DWORD> animationWait;
		vector<vector<DWORD>> animationInterval;
		vector<int> animationIntervalSuffix;
		vector<int> animationChipStart;
		vector<int> animationChipEnd;
		vector<bool> animationEnabled;
		vector<int> alpha;
		vector<bool> drawEnabled;
		int elementCount;
		unsigned int textureWidth, textureHeight;
		float chipPixelX, chipPixelY;
	};
}