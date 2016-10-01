#pragma once

#include "globals.h"

class TextOutput {
public:
	static TextOutput *getInstance() {
		static MemoryHandler mh;
		if (!instance) {
			instance = new TextOutput();
		}
		return instance;
	}

	void drawText(const std::string text, const int x, const int y, const int w, const int h, const int font,
				  const SDL_Color color) {
		if (font >= 0 && font < fonts.size()) {
			SDL_Surface *surfaceMessage = TTF_RenderText_Solid(fonts[font], text.c_str(), color);
			SDL_Texture *message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
			SDL_Rect destRect;
			destRect.x = x;
			destRect.y = y;
			destRect.w = w;
			destRect.h = h;
			SDL_RenderCopy(renderer, message, NULL, &destRect);
		} else {
			std::cout << "Illegal FontIndex" << std::endl;
		}
	}

private:
	static TextOutput *instance;
	std::vector<TTF_Font *> fonts;

	TextOutput() {
		addFont(std::string(CMAKE_SOURCE_DIR) + "/fonts/Raleway-Medium.ttf", 30);
	}

	TextOutput(const TextOutput &);

	~TextOutput() {

	}

	void addFont(const std::string path, const int textSize) {
		const char *cpath = path.c_str();
		TTF_Font *font = TTF_OpenFont(cpath, textSize);
		if (font == NULL) {
			std::cout << SDL_GetError();
		} else {
			fonts.push_back(font);
		}
	}

	class MemoryHandler {
	public:
		~MemoryHandler() {
			if (TextOutput::instance != NULL) {
				delete TextOutput::instance;
				TextOutput::instance = NULL;
			}
		}
	};
};

TextOutput *TextOutput::instance = NULL;