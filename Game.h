#pragma once

class CGame
{
public:
	virtual int Play() = 0; // play behavior
	CGame(void);
	~CGame(void);
};
