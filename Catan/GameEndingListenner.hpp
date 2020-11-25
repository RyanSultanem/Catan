#ifndef GAMEENDINGLISTENNER_HPP
#define GAMEENDINGLISTENNER_HPP

class GameEndingListenner
{
public:
	virtual void updateGameEnd(int playerId) = 0;
};

#endif // !GAMEENDINGLISTENNER_HPP