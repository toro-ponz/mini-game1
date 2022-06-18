#include "font-texture-create-class.h"
#include "global.h"
#include <wchar.h>

namespace Font {
	/**
	*  コンストラクタ
	*/
	FontTextureCreate::FontTextureCreate() :
		fontTextures() {
		maxHeight = 0.0f;
		maxWidth = 0.0f;
	}

	/**
	*  デストラクタ
	*/
	FontTextureCreate::~FontTextureCreate() {
		if (pTex != NULL) {
			pTex->Release();
			pTex = NULL;
		}
	}

	/**
	*  フォントテクスチャを作成する関数.
	*  @param l<string> 描画する文字列
	*  @param x<float> 描画開始位置のX座標
	*  @param y<float> 描画開始位置のY座標
	*  @param size<int> フォントサイズ
	*  @param weight<int> フォントの太さ
	*  @param font<string> フォント
	*  @param horizontal<bool> 横書きか否か
	*/
	void FontTextureCreate::fontCreate(string l, float x, float y, int size, int weight, string font, bool horizontal) {
		letters = l;

		maxWidth = 0.0f;
		maxHeight = 0.0f;
		textMetric.clear();
		glyphMetrics.clear();
		monochrome.clear();
		textMetric.shrink_to_fit();
		glyphMetrics.shrink_to_fit();
		monochrome.shrink_to_fit();
		maxGmBlackBoxX = 0;
		maxGmBlackBoxY = 0;
		pTex = NULL;

		wchar_t *lettersWchar = convertCharToWchar_t(letters.c_str());
		lettersLength = wcslen(lettersWchar);
		fontTextures.reserve(lettersLength);
		textMetric.reserve(lettersLength);
		glyphMetrics.reserve(lettersLength);
		monochrome = vector<vector<BYTE>>(lettersLength);
		startPositionX = x;
		startPositionY = y;
		isHorizontal = horizontal;
		setFontStatus(size, weight, font);

		HFONT hFont = CreateFontIndirect(&logFont);
		if (hFont == NULL) {
			Direct3DDevice9->Release();
			Direct3D9->Release();
			return;
		}

		// デバイスにフォントを持たせないとGetGlyphOutline関数はエラーとなる
		HDC hdc = GetDC(NULL);
		HFONT oldFont = (HFONT)SelectObject(hdc, hFont);

		const int gradFlag = GGO_GRAY8_BITMAP; // GGO_GRAY2_BITMAP or GGO_GRAY4_BITMAP or GGO_GRAY8_BITMAP
		int grad = 0; // 階調の最大値
		switch (gradFlag) {
		case GGO_GRAY2_BITMAP: grad = 4; break;
		case GGO_GRAY4_BITMAP: grad = 16; break;
		case GGO_GRAY8_BITMAP: grad = 64; break;
		}
		if (grad == 0) {
			Direct3DDevice9->Release(); Direct3D9->Release();
			return;
		}

		for (int i = 0; i < lettersLength; i++) {
			wchar_t *letter = &lettersWchar[i];
			// フォントビットマップ取得
			UINT code = (UINT)*letter;

			TEXTMETRIC tm;
			GetTextMetrics(hdc, &tm);
			GLYPHMETRICS gm;
			CONST MAT2 mat = { { 0,1 },{ 0,0 },{ 0,0 },{ 0,1 } };
			DWORD size = GetGlyphOutlineW(hdc, code, gradFlag, &gm, 0, NULL, &mat);
			BYTE *pMono = new BYTE[size];


			if (GetGlyphOutlineW(hdc, code, gradFlag, &gm, size, pMono, &mat) != GDI_ERROR) {
				textMetric.push_back(tm);
				glyphMetrics.push_back(gm);
				for (int j = 0; j < (int)size; j++) {
					monochrome[i].push_back(pMono[j]);
				}

				if (maxGmBlackBoxX < gm.gmBlackBoxX)
					maxGmBlackBoxX = gm.gmBlackBoxX;
				if (maxGmBlackBoxY < gm.gmBlackBoxY)
					maxGmBlackBoxY = gm.gmBlackBoxY;
			}
		}

		// デバイスコンテキストとフォントハンドルはもういらないので解放
		SelectObject(hdc, oldFont);
		ReleaseDC(NULL, hdc);

		// テクスチャ作成
		//IDirect3DTexture9 *pTex = 0;
		int textureWidth = ((maxGmBlackBoxX + 3) / 4 * 4);
		int textureHeight = maxGmBlackBoxY  * lettersLength;
		Direct3DDevice9->CreateTexture(textureWidth, textureHeight, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &pTex, NULL);

		// テクスチャにフォントビットマップ情報を書き込み
		D3DLOCKED_RECT lockedRect;
		pTex->LockRect(0, &lockedRect, NULL, 0);
		DWORD *pTexBuf = (DWORD*)lockedRect.pBits;   // テクスチャメモリへのポインタ

		for (int i = 0; i < lettersLength; i++) {
			int fontHeight = glyphMetrics[i].gmBlackBoxY;
			wchar_t letter[2];
			letter[0] = lettersWchar[i];
			//終端文字
			letter[1] = L'\x0';
			if (!wcscmp(letter, L"　") || !wcscmp(letter, L" ") || !wcscmp(letter, L"\n")) {
				for (int y = i * maxGmBlackBoxY; y < (int)(i * maxGmBlackBoxY + fontHeight); y++) {
					int fontWidth = (glyphMetrics[i].gmBlackBoxX + 3) / 4 * 4;
					for (int x = 0; x < fontWidth; x++) {
						UINT monoSuffix = (UINT)((y - (i * maxGmBlackBoxY)) * fontWidth + x);
						UINT bufSuffix = (UINT)(y * textureWidth + x);
						DWORD alpha = 0 * 255 / grad;
						pTexBuf[bufSuffix] = (alpha << 24) | 0x00ffffff;
					}
				}
			}
			else {
				for (int y = i * maxGmBlackBoxY; y < (int)(i * maxGmBlackBoxY + fontHeight); y++) {
					int fontWidth = (glyphMetrics[i].gmBlackBoxX + 3) / 4 * 4;
					for (int x = 0; x < fontWidth; x++) {
						UINT monoSuffix = (UINT)((y - (i * maxGmBlackBoxY)) * fontWidth + x);
						UINT bufSuffix = (UINT)(y * textureWidth + x);
						DWORD alpha = monochrome[i][monoSuffix] * 255 / grad;
						pTexBuf[bufSuffix] = (alpha << 24) | 0x00ffffff;
					}
				}
			}
		}
		pTex->UnlockRect(0);

		fontTextures.setTexture(pTex);
		fontTextures.setChipPixel((float)textureWidth, (float)maxGmBlackBoxY);
		setPosition(startPositionX, startPositionY);

		for (int i = 0; i < lettersLength; i++) {
			fontTextures.enableDraw(i);
		}
		for (int i = lettersLength; i < fontTextures.getElementCount(); i++) {
			fontTextures.disableDraw(i);
		}
	}

	/**
	*  描画する関数.
	*/
	void FontTextureCreate::Rend() {
		fontTextures.Draw();
	}

	/**
	*  フォント情報を作成する関数.
	*  @param size<int> フォントサイズ
	*  @param weight<int> フォントの太さ
	*  @param font<TCHAR> フォント
	*/
	void FontTextureCreate::setFontStatus(int size, int weight, string font) {
		fontSize = size;
		fontWeight = weight;

		logFont = {
			fontSize,
			0,
			0,
			0,
			fontWeight,
			0,
			0,
			0,
			SHIFTJIS_CHARSET,
			OUT_TT_ONLY_PRECIS,
			CLIP_DEFAULT_PRECIS,
			PROOF_QUALITY,
			DEFAULT_PITCH | FF_MODERN,
			*convertStringToTchar(font)
		};
	}

	/**
	*  各文字の表示位置を計算する関数.
	*  @param x<float> 左上のX座標
	*  @param y<float> 左上のY座標
	*/
	void FontTextureCreate::setPosition(float x, float y) {
		wchar_t *lettersWchar = convertCharToWchar_t(letters.c_str());
		float startPositionX = x;
		float startPositionY = y;
		float posX = x;
		float posY = y;
		for (int i = 0; i < lettersLength; i++) {
			fontTextures.setRectFromChip(i, i + 1);
			fontTextures.setCenter(i, 0.0f, 0.0f, 0.0f);
			wchar_t letter[2];
			letter[0] = lettersWchar[i];
			//終端文字
			letter[1] = L'\x0';
			if (!wcscmp(letter, L"\n")) {
				if (isHorizontal) {
					posX = startPositionX;
					posY += maxGmBlackBoxY;
				}
				else {
					posX += maxGmBlackBoxX;
					posY = startPositionY;
				}
			}
			else {
				if (isHorizontal) {
					if (maxWidth != 0.0f && posX + glyphMetrics[i].gmCellIncX > maxWidth + startPositionX) {
						posX = startPositionX;
						posY += maxGmBlackBoxY;
					}
					fontTextures.setPosition(i, posX + glyphMetrics[i].gmptGlyphOrigin.x, posY + textMetric[i].tmAscent - glyphMetrics[i].gmptGlyphOrigin.y);
					posX += glyphMetrics[i].gmCellIncX;
				}
				else {
					if (maxHeight != 0.0f && posY + glyphMetrics[i].gmBlackBoxY > maxHeight + startPositionY) {
						posX += maxGmBlackBoxX;
						posY = startPositionY;
					}
					fontTextures.setPosition(i, posX + ((maxGmBlackBoxX - glyphMetrics[i].gmBlackBoxX) / 2), posY);
					posY += glyphMetrics[i].gmBlackBoxY;
				}
			}
		}
	}

	/**
	*  文字列の折り返し幅または高さを設定する関数.
	*  @param px<int> 文字列の折り返し幅または高さ
	*/
	void FontTextureCreate::setWidthOrHeight(float px) {
		if (isHorizontal)
			maxWidth = px;
		else
			maxHeight = px;
		setPosition(startPositionX, startPositionY);
	}

	/**
	*  描画をOFFにする関数.
	*/
	void FontTextureCreate::disableDraw() {
		for (int i = 0; i < lettersLength; i++) {
			fontTextures.disableDraw(i);
		}
	}

	/**
	*  描画をONにする関数.
	*/
	void FontTextureCreate::enableDraw() {
		for (int i = 0; i < lettersLength; i++) {
			fontTextures.enableDraw(i);
		}
	}

	/**
	*  文字色を設定する関数.
	*  @param c<D3DCOLOR> 文字色(ARGB)
	*/
	void FontTextureCreate::setColor(D3DCOLOR c) {
		for (int i = 0; i < lettersLength; i++) {
			fontTextures.setColor(i, c);
		}
	}
}