#pragma once

#include "sprite-drawing-class.h"

using namespace DxSprite;

namespace Frame {
	class CreditRender {
	public:
		CreditRender();
		~CreditRender();
		void Rend();

	private:
		SpriteDrawing Logo;
		SpriteDrawing Black;
		DWORD oldTime;
		int state;
	};
}