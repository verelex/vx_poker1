#pragma once

#include <list>
#include "trio.h"

typedef trio< int,int,int >  CCard; // card(value,suit,id)
typedef std::list< CCard >  CDeck;
typedef std::list< CCard >::const_iterator DeckIter;

class CHand
{
public:
	CDeck deck;
	int hand_id;
	int hand_type;
	int kicker;
	int kicker_card_id;
	//
	CHand(void);
	~CHand(void);
	//
	int DetectHandType();
	int DetectFlush();
	int DetectStraight(DeckIter SrcD, int* /*OUT*/ kicker_id);
	void TestStraight();
};
