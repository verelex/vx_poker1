#include "StdAfx.h"
#include "PokerGame.h"
#include "windows.h" // GetTickCount()

CPokerGame::CPokerGame(void)
{
}

CPokerGame::~CPokerGame(void)
{
}

//---------------------------------------------------------
// 
//
int CPokerGame::Play(void)
{
	/*GetAnte();
	GetBlinds();*/
	OneCardToTrash();
	DealCards2Gamers();
	Deal();
	OneCardToTrash();
	GetFlop();
	Deal();
	OneCardToTrash();
	GetOneCard2Table(); // river
	Deal();
	OneCardToTrash();
	GetOneCard2Table(); // last
	Deal();
	GetWinner();
	dbg = 1;
	return 0;
}

//---------------------------------------------------------
// 
//
int CPokerGame::ResultTableInit()
{
	CResult r;
	for(GITER giter=table.second.begin(),end=table.second.end(); giter!=end; ++giter)
	{
		r.first = giter->first; // user_id
		rt.push_back(r);
	}
	return 0;
}

//---------------------------------------------------------
// 
//
int CPokerGame::PrepareTable(void)
{
	AddGamers();
	//MixDeck();
	PrepareDeckManually();
	ResultTableInit();
	return 0;
}

//---------------------------------------------------------
// 
//
int CPokerGame::AddGamers() // test
{
	int n=0;
	CGamer gamer;
	REPEAT(3)
	{
		gamer.first = n;
		table.second.push_back(gamer);
		n++;
	}
	return 0;
}

//---------------------------------------------------------
// Mixing deck
//
int CPokerGame::MixDeck()
{
	CDeck DefaultDeck;
	CCard PokerCard;

	// create default deck
	for(int i=0,n=0; i<4; i++) // suit
	{
		for(int j=2; j<15; j++, n++) // nominal
		{
			PokerCard.first  = j; // nominal
			PokerCard.second = i; // suit
			PokerCard.third  = n; // id
			DefaultDeck.push_back(PokerCard);
		}
	}
	// mixing deck
	double k = 52.0;
	srand(GetTickCount());
	for(int i=0; i<52; i++,k=k-1.0)
	{
		int R = (int) _RANDOM(k); // [0;51--]
		ITER iter = DefaultDeck.begin();
		advance(iter,R);
		deck.push_back(iter._Mynode()->_Myval);
		iter = DefaultDeck.erase(iter);
	}
	return 0;
}

//---------------------------------------------------------
// Prepare deck manually for testing detect-foo
//
int CPokerGame::PrepareDeckManually()
{
	CDeck DefaultDeck;
	CCard PokerCard;

	for(int i=0,n=0; i<4; i++) // suit
	{
		for(int j=2; j<15; j++, n++) // nominal
		{
			PokerCard.first  = j; // nominal
			PokerCard.second = i; // suit
			PokerCard.third  = n; // id
			DefaultDeck.push_back(PokerCard);
		}
	}
}

//---------------------------------------------------------
// Drop one card to trash
//
int CPokerGame::OneCardToTrash()
{
	ITER iter = deck.begin();
	trash.push_back(iter._Mynode()->_Myval);
	iter = deck.erase(iter);
	return 0;
}

//---------------------------------------------------------
// Deal two cards to each gamer
//
int CPokerGame::DealCards2Gamers()
{
	for (GITER giter=table.second.begin(),end=table.second.end(); giter!=end; ++giter)
	{
		REPEAT(2)
		{
			ITER iter = deck.begin();
			giter->second.push_back(iter._Mynode()->_Myval);
			iter = deck.erase(iter);
		}
	}
	return 0;
}

//---------------------------------------------------------
// Deal first three cards on the table
//
int CPokerGame::GetOneCard2Table()
{
	ITER iter = deck.begin();
	table.first.push_back(iter._Mynode()->_Myval);
	iter = deck.erase(iter);
	return 0;
}

//---------------------------------------------------------
// Deal first three cards on the table
//
int CPokerGame::GetFlop()
{
	REPEAT(3)
	{
		GetOneCard2Table();
	}
	return 0;
}

//---------------------------------------------------------
// 
//
int CPokerGame::Deal() // TODO: change method name (raise, check, pass)
{
	return 0;
}

//---------------------------------------------------------
// 
//
int CPokerGame::SevenCardsHand()
{
	for (GITER giter=table.second.begin(),end=table.second.end(); giter!=end; ++giter)
	{
		for(ITER iter=table.first.begin(),end=table.first.end(); iter!=end; ++iter)
		{
			giter->second.push_back(iter._Mynode()->_Myval);
		}
	}
	return 0;
}

//---------------------------------------------------------
// 
//
int CPokerGame::GetWinner()
{
	SevenCardsHand();
	DetectFlush();
	return 0;
}

//---------------------------------------------------------
// 
//
int CPokerGame::GetKicker()
{
	return 0;
}

//---------------------------------------------------------
// 
//
int CPokerGame::DetectFlush()
{
	int suit = -1;
	int clubs_count    = 0; // trefy
	int spades_count   = 0; // piki
	int hearts_count   = 0; // chervi
	int diamonds_count = 0; // bubny
	//
	RITER riter = rt.begin();
	for (GITER giter = table.second.begin(), end = table.second.begin(); giter!=end; ++giter) // 3 = gamers count
	{
		for(ITER iter=giter->second.begin(),end=giter->second.end(); iter!=end; ++iter) // 7 = cards on each gamer
		{
			suit = iter->second;
			if (suit == SUIT_CLUBS)    { clubs_count++; }
			if (suit == SUIT_SPADES)   { spades_count++; }
			if (suit == SUIT_HEARTS)   { hearts_count++; }
			if (suit == SUIT_DIAMONDS) { diamonds_count++; }
		}
		// set hand_type
		if(clubs_count >= 5)    { riter->fourth = SUIT_CLUBS; }
		if(spades_count >= 5)   { riter->fourth = SUIT_SPADES; }
		if(hearts_count >= 5)   { riter->fourth = SUIT_HEARTS; }
		if(diamonds_count >= 5) { riter->fourth = SUIT_DIAMONDS; } 
		++riter;
	}
	return 0;
}
