/*
 * SabreController.cpp
 *
 *  Created on: Feb 5, 2014
 *      Author: sarniakjr
 */

#include "SabreController.h"
#include <cstring>
#include <algorithm>

using namespace std;

SabreController::SabreController(Board *b, const char *dictPath) {
	board = b;
	firstMove = true;
	activeTile = nullptr;
	activeField = nullptr;
	bag = new TileBag();
	dict = new Dictionary();
	dict->loadFile(dictPath);
}

SabreController::~SabreController() {
	delete dict;
	delete bag;
}

void SabreController::loadTileBag(string path) {
	bag->loadFile(path);
}

void SabreController::addPlayer(string name) {
	Player *player = new Player(name, board, bag);
	players.push_back(player);
	activePlayer = players.front();

}

void SabreController::removePlayer(string name) {
	for (std::vector<Player *>::iterator it = players.begin(); it != players.end(); ++it) {
		if ((*it)->getName() == name) {
			players.erase(it);
			break;
		}
	}
}

void SabreController::nextPlayer() {
	std::vector<Player *>::iterator p = std::find(players.begin(), players.end(), activePlayer);
	p++;
	activePlayer = p == players.end() ? players.front() : *p;
}

void SabreController::pickTile(int idx) {
	Tile *tile = activePlayer->pickTile(idx);
	activeTile = tile;
}

bool SabreController::canPutTile(int x, int y) {
	Field *field = board->get(x, y);
	return field->isFree();
}

void SabreController::putTile(int x, int y) {
	Field *field = board->get(x, y);
	activePlayer->putTile(activeTile, field);
	activeTile = nullptr;
}

void SabreController::gatherTiles() {
	bool done = activePlayer->getHandSize() == MAX_TILES || bag->empty();

	while (!done) {
		activePlayer->takeTile(bag);
		done = activePlayer->getHandSize() == MAX_TILES || bag->empty();
	}
}

Player *SabreController::getActivePlayer() {
	return activePlayer;
}

Tile *SabreController::getActiveTile() {
	return activeTile;
}

Board *SabreController::getBoard() {
	return board;
}

TileBag *SabreController::getTileBag() {
	return bag;
}

bool SabreController::canCommit() {
	if (firstMove) {
		firstMove = false;
		return activePlayer->getMove()->isValidAsFirst();
	}
	return activePlayer->getMove()->isValid();
}

void SabreController::commit() {
	Move *move = activePlayer->getMove();
	activePlayer->addPoints(move->getScore());
	move->clear();
}

void SabreController::rollback() {
	vector<Field *> fields = activePlayer->getMove()->getFieldsCopy();
	for (vector<Field *>::iterator it = fields.begin(); it != fields.end(); ++it) {
		activePlayer->removeTile(*it);
	}
}

bool SabreController::isMoveCorrect() {
	size_t max_length = 0;
	set<string> words = activePlayer->getMove()->getWords();

	for (set<string>::iterator it = words.begin(); it != words.end(); ++it)
		max_length = MAX(max_length, it->length());

	/* Special case, only one tile was set as first move */
	if (max_length == 1)
		return dict->contains(*(words.begin()));

	for (set<string>::iterator it = words.begin(); it != words.end(); ++it)
		if (it->length() > 1 && !dict->contains(*it))
			return false;

	return true;
}

void SabreController::flushTiles() {

}
