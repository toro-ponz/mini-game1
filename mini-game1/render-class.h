#pragma once

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include "credit-render-class.h"
#include "level-select-render-class.h"
#include "play-render-class.h"
#include <Windows.h>
#include <string>


namespace Frame {
	class Render {
	public:
		Render();
		~Render();
		void Rend();

	private:
		CreditRender *cr;
		LevelSelectRender *lsr;
		PlayRender *pr;
		int level;
	};
}