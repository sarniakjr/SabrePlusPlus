/*
 * SabreController.h
 *
 *  Created on: Feb 5, 2014
 *      Author: sarniakjr
 */

#ifndef SABRECONTROLLER_H_
#define SABRECONTROLLER_H_

#include "Board.h"
#include "Player.h"
#include <vector>

#define MAX_TILES (7)

class SabreController {
public:
	SabreController(Board *board);
	virtual ~SabreController();

	void addPlayer(const char *name);
	void removePlayer(const char *name);
	void nextPlayer();

	void pickTile(int idx);
	bool canPutTile(Field *field);
	void putTile(Field *field);
	void gatherTiles();
	Player *getActivePlayer();
	Board *getBoard();
	TileBag *getTileBag();
	bool canCommit();
	void commit();
	void flushTiles();

private:
	void nextRound();
	Board *board;
	TileBag *bag;
	Player *activePlayer;
	Field *activeField;
	Tile *activeTile;
	std::vector<Player *> players;
	bool firstMove;
};

#endif /* SABRECONTROLLER_H_ */
