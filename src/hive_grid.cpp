/* Copyright: (c) Kayne Ruse 2013-2016
 * 
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 * 
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 * 
 * 1. The origin of this software must not be misrepresented; you must not
 * claim that you wrote the original software. If you use this software
 * in a product, an acknowledgment in the product documentation would be
 * appreciated but is not required.
 * 
 * 2. Altered source versions must be plainly marked as such, and must not be
 * misrepresented as being the original software.
 * 
 * 3. This notice may not be removed or altered from any source
 * distribution.
*/
#include "hive_grid.hpp"

#include "texture_loader.hpp"

#include <stdexcept>

HiveGrid::HiveGrid() {
	//
}

HiveGrid::~HiveGrid() {
	Quit();
}

void HiveGrid::DrawTo(SDL_Renderer* const renderer, int camX, int camY, double zoom) {
	Image utilImage;
	TextureLoader& textureLoader = TextureLoader::GetSingleton();

	for (int j = 0; j < yCount; j++) {
		for (int i = 0; i < xCount; i++) {
			//get the correct texture data
			SDL_Texture* cellTexture = [&]() -> SDL_Texture* const {
				switch(cellArray[i][j].GetState()) {
					case BaseCell::State::BLANK: return nullptr;
					case BaseCell::State::EMPTY: return textureLoader.Find("comb-empty.png");
					case BaseCell::State::HONEY: return textureLoader.Find("comb-honey.png");
					case BaseCell::State::CAPPED: return textureLoader.Find("comb-capped.png");
					case BaseCell::State::BROOD: return textureLoader.Find("comb-brood.png");
				};
			}();

			if (cellTexture == nullptr) {
				continue;
			}
			else {
				utilImage.SetTexture(cellTexture);
			}

			//draw to the correct location
			int destX = i * 12;
			int destY = (j/2) * 18;

			if (j % 2 == 1) {
				destX += 6;
				destY += 9;
			}

			utilImage.DrawTo(renderer, (camX + destX) * zoom, (camY + destY) * zoom, zoom, zoom);
		}
	}
}

void HiveGrid::Init(int x, int y) {
	if (cellArray != nullptr) {
		throw(std::runtime_error("Cannot initialize a full hive grid"));
	}
	xCount = x;
	yCount = y;
	cellArray = new BaseCell*[xCount];
	for (int i = 0; i < xCount; i++) {
		cellArray[i] = new BaseCell[yCount];
	}
}

void HiveGrid::Quit() {
	for (int i = 0; i < xCount; i++) {
		delete cellArray[i];
	}
	delete cellArray;
	cellArray = nullptr;
	xCount = 0;
	yCount = 0;
}

BaseCell* HiveGrid::GetCell(int x, int y) {
	if (cellArray == nullptr) {
		throw(std::runtime_error("Cannot access an empty hive grid"));
	}
	if (x < 0 || y < 0 || x >= xCount || y >= yCount) {
		throw(std::out_of_range("HiveGrid::GetCell arguments out of range"));
	}

	return &cellArray[x][y];
}

int HiveGrid::GetXCount() {
	return xCount;
}

int HiveGrid::GetYCount() {
	return yCount;
}
