#pragma once

#include "sprite-drawing-class.h"
#include "font-texture-create-class.h"

using namespace DxSprite;
using namespace Font;

namespace Frame {
	class LevelSelectRender {
	public:
		LevelSelectRender();
		~LevelSelectRender();
		void Rend();
		int getLevel();

	private:
		SpriteDrawing background;
		SpriteDrawing selectedLevel;
		FontTextureCreate loading;
		int level = 1;
	};
}