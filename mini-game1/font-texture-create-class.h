#pragma once

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include <tchar.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <stdio.h>
#include "sprite-drawing-class.h"

using namespace DxSprite;

namespace Font {
	class FontTextureCreate {
	public:
		FontTextureCreate();
		~FontTextureCreate();
		void Rend();
		void fontCreate(string, float, float, int, int, string, bool = true);
		void setPosition(float, float);
		void setWidthOrHeight(float);
		void disableDraw();
		void enableDraw();
		void setColor(D3DCOLOR);

	private:
		void setFontStatus(int, int, string);

		SpriteDrawing fontTextures;
		LOGFONT logFont;
		LPDIRECT3DTEXTURE9 pTex = NULL;

		string letters;

		float startPositionX;
		float startPositionY;
		float maxWidth;
		float maxHeight;
		int fontSize;
		int fontWeight;
		int lettersLength;
		bool isHorizontal;

		vector<TEXTMETRIC> textMetric;
		vector<GLYPHMETRICS> glyphMetrics;
		vector<vector<BYTE>> monochrome;

		UINT maxGmBlackBoxX = 0;
		UINT maxGmBlackBoxY = 0;

		struct Vtx {
			float x, y, z;
			float u, v;
		};
	};
}