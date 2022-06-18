#include "sprite-drawing-class.h"

namespace DxSprite {
	/**
	*  コンストラクタ
	*  @param count<int> 要素数
	*  @param x<float> チップのX方向のピクセル値
	*  @param y<float> チップのY方向のピクセル値(-1でxと同じ値となる)
	*/
	SpriteDrawing::SpriteDrawing(int count, float x, float y) :
		elementCount(count) {
		chipPixelX = x;
		if (y == -1)
			chipPixelY = x;
		else
			chipPixelY = y;
		initialize();
	}

	/**
	*  コンストラクタ
	*  @param path<TCHAR*> テクスチャのファイルパス
	*  @param count<int> 要素数
	*  @param x<float> チップのX方向のピクセル値
	*  @param y<float> チップのY方向のピクセル値(-1でxと同じ値となる)
	*/
	SpriteDrawing::SpriteDrawing(TCHAR* path, int count, float x, float y) :
		elementCount(count) {
		chipPixelX = x;
		if (y == -1)
			chipPixelY = x;
		else
			chipPixelY = y;
		initialize();
		setTexture(path);
	}

	/**
	*  コンストラクタ
	*  @param path<string> テクスチャのファイルパス
	*  @param count<int> 要素数
	*  @param x<float> チップのX方向のピクセル値
	*  @param y<float> チップのY方向のピクセル値(-1でxと同じ値となる)
	*/
	SpriteDrawing::SpriteDrawing(string path, int count, float x, float y) :
		elementCount(count) {
		chipPixelX = x;
		if (y == -1)
			chipPixelY = x;
		else
			chipPixelY = y;
		initialize();
		setTexture(path);
	}

	/**
	*  コンストラクタ
	*  @param pTex<LPDIRECT3DTEXTURE9> テクスチャのファイルパス
	*  @param count<int> 要素数
	*  @param x<float> チップのX方向のピクセル値
	*  @param y<float> チップのY方向のピクセル値(-1でxと同じ値となる)
	*/
	SpriteDrawing::SpriteDrawing(LPDIRECT3DTEXTURE9 pTex, int count, float x, float y) :
		elementCount(count) {
		chipPixelX = x;
		if (y == -1)
			chipPixelY = x;
		else
			chipPixelY = y;
		initialize();
		setTexture(pTex);
	}

	/**
	*  デストラクタ
	*/
	SpriteDrawing::~SpriteDrawing() {
		if (texture != NULL) {
			texture->Release();
			texture = NULL;
		}
	}

	/**
	*  テクスチャを読み込んで登録する関数.
	*  @param path<TCHAR*> 画像ファイルパス
	*  @return <HRESULT> 画像ファイルロードの成否
	*/
	HRESULT SpriteDrawing::setTexture(TCHAR* path) {
		HRESULT hr = D3DXCreateTextureFromFileEx(Direct3DDevice9,
			path,
			D3DX_DEFAULT,
			D3DX_DEFAULT,
			1,
			0,
			D3DFMT_A8R8G8B8,
			D3DPOOL_MANAGED,
			D3DX_FILTER_NONE,
			D3DX_FILTER_LINEAR,
			0,
			NULL,
			NULL,
			&texture);

		// テクスチャサイズの取得
		D3DXIMAGE_INFO info;
		D3DXGetImageInfoFromFile(path, &info);
		textureWidth = info.Width;
		textureHeight = info.Height;
		for (int i = 0; i < elementCount; i++) {
			setRectFromPixel(i, 0, textureHeight, 0, textureWidth);
		}
		return hr;
	}


	/**
	*  テクスチャを読み込んで登録する関数.
	*  @param path<TCHAR*> 画像ファイルパス
	*  @return <HRESULT> 画像ファイルロードの成否
	*/
	HRESULT SpriteDrawing::setTexture(string path) {
		HRESULT hr = D3DXCreateTextureFromFileEx(Direct3DDevice9,
			convertStringToTchar(path),
			D3DX_DEFAULT,
			D3DX_DEFAULT,
			1,
			0,
			D3DFMT_A8R8G8B8,
			D3DPOOL_MANAGED,
			D3DX_FILTER_NONE,
			D3DX_FILTER_LINEAR,
			0,
			NULL,
			NULL,
			&texture);

		// テクスチャサイズの取得
		D3DXIMAGE_INFO info;
		D3DXGetImageInfoFromFile(convertStringToTchar(path), &info);
		textureWidth = info.Width;
		textureHeight = info.Height;
		for (int i = 0; i < elementCount; i++) {
			setRectFromPixel(i, 0, textureHeight, 0, textureWidth);
		}
		return hr;
	}

	/**
	*  既存のテクスチャを登録する関数.
	*  @param pTex<LPDIRECT3DTEXTURE9> テクスチャのポインタ
	*/
	void SpriteDrawing::setTexture(LPDIRECT3DTEXTURE9 pTex) {
		texture = pTex;
		// テクスチャサイズの取得
		D3DSURFACE_DESC desc;
		if (FAILED(texture->GetLevelDesc(0, &desc))) return;
		textureWidth = desc.Width;
		textureHeight = desc.Height;
		for (int i = 0; i < elementCount; i++) {
			setRectFromPixel(i, 0, textureHeight, 0, textureWidth);
		}
	}

	/**
	*  描画範囲の矩形を指定する関数.
	*  @param suffix<int> 要素番号
	*  @param top<LONG> 始点のY座標
	*  @param bottom<LONG> 終点のY座標
	*  @param left<LONG> 始点のX座標
	*  @param right<LONG> 終点のX座標
	*/
	void SpriteDrawing::setRectFromPixel(int suffix, LONG top, LONG bottom, LONG left, LONG right) {
		rect[suffix].top = top;
		rect[suffix].bottom = bottom;
		rect[suffix].left = left;
		rect[suffix].right = right;
	}

	/**
	*  描画範囲の矩形をタイルのインデックス値から指定する関数.
	*  @param suffix<int> 要素番号
	*  @param start<int> 範囲開始タイルのインデックス値
	*  @param end<int> 範囲終了タイルのインデックス値
	*/
	void SpriteDrawing::setRectFromChip(int suffix, int start, int end) {
		int widthChipCount = (int)(textureWidth / chipPixelX);
		if (end == -1)
			end = start;

		float top = chipPixelY * (int)((start - 1) / widthChipCount);
		float bottom = chipPixelY * (int)((end - 1) / widthChipCount + 1);
		float left = chipPixelX * ((start - 1) % widthChipCount);
		float right = chipPixelX * ((end - 1) % widthChipCount + 1);

		setRectFromPixel(suffix, (LONG)top, (LONG)bottom, (LONG)left, (LONG)right);
	}

	/**
	*  描画範囲の基点を指定する関数.
	*  @param suffix<int> 要素番号
	*  @param x<float> 基点のX値
	*  @param y<float> 基点のY値
	*  @param z<float> 基点のZ値
	*/
	void SpriteDrawing::setCenter(int suffix, float x, float y, float z) {
		center[suffix].x = x;
		center[suffix].y = y;
		center[suffix].z = z;
	}

	/**
	*  描画座標を指定する関数.
	*  @param suffix<int> 要素番号
	*  @param x<float> 描画座標のX値
	*  @param y<float> 描画座標のY値
	*  @param z<float> 描画座標のZ値
	*/
	void SpriteDrawing::setPosition(int suffix, float x, float y, float z) {
		position[suffix].x = x;
		position[suffix].y = y;
		position[suffix].z = z;
	}

	/**
	*  ブレンドするARGBを指定する関数.
	*  @param suffix<int> 要素番号
	*  @param c<D3DCOLOR> D3DCOLOR_ARGB
	*/
	void SpriteDrawing::setColor(int suffix, D3DCOLOR c) {
		color[suffix] = c;
	}

	/**
	*  要素数を返す関数.
	*  @return <int> 要素数
	*/
	int SpriteDrawing::getElementCount() {
		return elementCount;
	}

	/**
	*  描画座標を移動する関数.
	*  @param suffix<int> 要素番号
	*  @param add<float> X座標の増減
	*  @return <float> 移動後のX座標
	*/
	float SpriteDrawing::addPositionX(int suffix, float add) {
		position[suffix].x += add;
		return position[suffix].x;
	}

	/**
	*  描画座標を移動する関数.
	*  @param suffix<int> 要素番号
	*  @param add<float> Y座標の増減
	*  @return <float> 移動後のY座標
	*/
	float SpriteDrawing::addPositionY(int suffix, float add) {
		position[suffix].y += add;
		return position[suffix].y;
	}

	/**
	*  透明度を増減させる関数.
	*  @param suffix<int> 要素番号
	*  @param alpha<int> 透明度の増減
	*  @return <bool> 計算後のアルファ値が正常値か否か
	*/
	bool SpriteDrawing::addAlpha(int suffix, int alpha) {
		unsigned int a = ((color[suffix] & 0xff000000) >> 24) + alpha;
		if (a > 255) return false;
		color[suffix] = (a << 24) | (color[suffix] & 0x00ffffff);
		return true;
	}

	/**
	*  透明度を増減させる関数.
	*  @param suffix<int> 要素番号
	*  @param interval<DWORD*>
	*  @param intervalCount<int>
	*/
	void SpriteDrawing::addAlpha(int suffix, DWORD *interval, int intervalCount) {
		if ((color[suffix] & 0xff000000) >> 24 == 0) {

		}
		else {

		}
	}

	/**
	*  アニメーションの間隔を指定する関数.
	*  @param suffix<int> 要素番号
	*  @param start<int> アニメーションの最初のチップ番号
	*  @param end<int> アニメーションの最後のチップ番号
	*  @param interval<DWORD*> 各間隔の配列のポインタ
	*  @param intervalCount<int> interval配列の要素数
	*  @param animationCount<int> アニメーションのコマ数
	*  @param wait<DWORD> アニメーション開始までの待機時間
	*/
	void SpriteDrawing::setAnimationInterval(int suffix, int start, int end, DWORD *interval, unsigned int intervalCount, unsigned int animationCount, DWORD wait) {
		animationChipStart[suffix] = start;
		animationChipEnd[suffix] = end;
		animationWait[suffix] = wait;
		setRectFromChip(suffix, start);
		if (intervalCount == animationCount) {
			for (int i = 0; i < (int)intervalCount; i++) {
				animationInterval[suffix].push_back(interval[i]);
			}
		}
		else if (intervalCount > animationCount) {
			animationEnabled[suffix] = false;
			return;
		}
		else {
			for (int i = 0; i < (int)intervalCount; i++) {
				animationInterval[suffix].push_back(interval[i]);
			}
			for (int i = (int)intervalCount; i < (int)animationCount; i++) {
				animationInterval[suffix].push_back(interval[0]);
			}
		}
		animationEnabled[suffix] = true;
	}

	/**
	*  アニメーションを再現するためにRECT等を計算する関数.
	*  @param suffix<int> 要素番号
	*/
	void SpriteDrawing::culcAnimation(int suffix, DWORD time) {
		if (animationOldTime[suffix] == 0)
			animationOldTime[suffix] = time + animationWait[suffix];
		//前回の時間+今回の間隔の和が現在の時間よりも大きい場合次のコマへ
		if (animationOldTime[suffix] <= time - animationInterval[suffix][animationIntervalSuffix[suffix]]) {
			//animationIntervalSuffixが最後なら最初のコマに戻る
			if (animationIntervalSuffix[suffix] == animationChipEnd[suffix] - animationChipStart[suffix])
				animationIntervalSuffix[suffix] = 0;
			else
				animationIntervalSuffix[suffix]++;
			animationOldTime[suffix] = time;
			//描画するチップ番号を計算する
			int animationChipNumber = animationIntervalSuffix[suffix] + animationChipStart[suffix];
			setRectFromChip(suffix, animationChipNumber);
		}
	}

	/**
	*  描画をOFFにする関数.
	*  @param suffix<int> 要素番号
	*/
	void SpriteDrawing::disableDraw(int suffix) {
		drawEnabled[suffix] = false;
	}

	/**
	*  描画をONにする関数.
	*  @param suffix<int> 要素番号
	*/
	void SpriteDrawing::enableDraw(int suffix) {
		drawEnabled[suffix] = true;
	}

	/**
	*  テクスチャのチップ幅を指定する関数.
	*  @param x<float> チップのX方向のピクセル値
	*  @param y<float> チップのY方向のピクセル値(-1でxと同じ値となる)
	*/
	void SpriteDrawing::setChipPixel(float x, float y) {
		chipPixelX = x;
		if (y == -1)
			chipPixelY = x;
		else
			chipPixelY = y;
	}

	/**
	*  要素数を増やす関数.
	*  @param q<int> 増加後の要素数
	*/
	void SpriteDrawing::reserve(int q) {
		if (elementCount < q) {
			elementCount = q;
			rect.reserve(elementCount);
			center.reserve(elementCount);
			position.reserve(elementCount);
			color.reserve(elementCount);
			animationOldTime.reserve(elementCount);
			animationWait.reserve(elementCount);
			animationInterval = vector<vector<DWORD>>(elementCount);
			animationIntervalSuffix.reserve(elementCount);
			animationChipStart.reserve(elementCount);
			animationChipEnd.reserve(elementCount);
			animationEnabled.reserve(elementCount);
			drawEnabled.reserve(elementCount);
			for (int i = 0; i < elementCount; i++) {
				rect.push_back({ 0, 0, 0, 0 });
				center.push_back({ 0.0f, 0.0f, 0.0f });
				position.push_back({ 0.0f, 0.0f, 0.0f });
				color.push_back(D3DCOLOR_ARGB(255, 255, 255, 255));
				animationOldTime.push_back(0);
				animationWait.push_back(0);
				animationIntervalSuffix.push_back(0);
				animationChipStart.push_back(0);
				animationChipEnd.push_back(0);
				animationEnabled.push_back(false);
				drawEnabled.push_back(true);
				setRectFromPixel(i, 0, textureHeight, 0, textureWidth);
			}
		}
	}

	/**
	*  初期化する関数.
	*/
	void SpriteDrawing::initialize() {
		rect.reserve(elementCount);
		center.reserve(elementCount);
		position.reserve(elementCount);
		color.reserve(elementCount);
		animationOldTime.reserve(elementCount);
		animationWait.reserve(elementCount);
		animationInterval = vector<vector<DWORD>>(elementCount);
		animationIntervalSuffix.reserve(elementCount);
		animationChipStart.reserve(elementCount);
		animationChipEnd.reserve(elementCount);
		animationEnabled.reserve(elementCount);
		drawEnabled.reserve(elementCount);
		for (int i = 0; i < elementCount; i++) {
			rect.push_back({ 0, 0, 0, 0 });
			center.push_back({ 0.0f, 0.0f, 0.0f });
			position.push_back({ 0.0f, 0.0f, 0.0f });
			color.push_back(D3DCOLOR_ARGB(255, 255, 255, 255));
			animationOldTime.push_back(0);
			animationWait.push_back(0);
			animationIntervalSuffix.push_back(0);
			animationChipStart.push_back(0);
			animationChipEnd.push_back(0);
			animationEnabled.push_back(false);
			drawEnabled.push_back(true);
		}
	}

	/**
	*  各要素をスプライトに流し込み描画させる関数.
	*/
	void SpriteDrawing::Draw() {
		DWORD time = timeGetTime();
		for (int i = 0; i < elementCount; i++) {
			if (drawEnabled[i]) {
				if (animationEnabled[i])
					culcAnimation(i, time);
				Sprite->Draw(texture, &rect[i], &center[i], &position[i], color[i]);
			}
		}
	}
}