#pragma once

#include "stats.h"
#include "colors.h"
#include "Tower.h"
#include "Button.h"

static void drawStats() {

}

static void drawShopMenu() {

}


static void initTowerMenu() {
	allGUIObjects.push_back(new Button("Upgrade", 1, 1, 5, 1, COLOR_GREEN, COLOR_RED));
	allGUIObjects.push_back(new Button("Sell", 1, 2, 5, 1, COLOR_GREEN, COLOR_RED));
	allGUIObjects.push_back(new Button("GodMode = on", 1, 3, 5, 1, COLOR_GREEN, COLOR_RED));
}

static void setTowerMenuIsActivated(const bool state) {

}

static void displayTowerStats(Tower &tower) {
	//cooldown
	TextOutput::getInstance()->drawText(" Cooldown: " + std::to_string(tower.cooldown), (MAP_WIDTH + 1) * TILE_WIDTH,
										TILE_HEIGHT * 5, 8, 0, COLOR_GREEN);
}


