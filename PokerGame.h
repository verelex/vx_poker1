#pragma once
#include "game.h"

#include <list>
#include "trio.h"
#include "quad.h"

// suits
#define SUIT_CLUBS    0 // trefy
#define SUIT_SPADES   1 // piki
#define SUIT_HEARTS   2 // chervi
#define SUIT_DIAMONDS 3 // bubny
#define SUIT_DIFFER   4 // another

// other nominals
#define NOM_ACE1  1  // tuz
#define NOM_JACK  11 // valet
#define NOM_QUEEN 12 // dama
#define NOM_KING  13 // korol
#define NOM_ACE   14 // tuz
#define NOM_JOKER 15

// hand types
#define HT_ROYAL_FLUSH    9000
#define HT_STRAIGHT_FLUSH 8000
#define HT_QUADS          7000
#define HT_FULL_HOUSE     6000
#define HT_FLUSH          5000
#define HT_STRAIGHT       4000
#define HT_TRIO           3000
#define HT_TWO_PAIRS      2000
#define HT_ONE_PAIR       1000
#define HT_HIGH_CARD      0

/*
j=1+(int) (10.0*rand()/(RAND_MAX+1.0));
А по поводу "почему от 1 до 10": 
Как уже было отмечено rand() выдает псевдослучайное число от 0 до RAND_MAX. 
Это значит, что rand()/(RAND_MAX+1.0) выдает число от 0 до 1 (не включая 1) 
10.0*rand()/(RAND_MAX+1.0)) выдает, соответственно, от 0 до 10 (не включая 10) 
(int) (10.0*rand()/(RAND_MAX+1.0)) генерирует целые числа в интервале 0-9 (10 быть не может, потому что исходный интервал 10 не включал) 
1+(int) (10.0*rand()/(RAND_MAX+1.0)) - интервал становится от 1 до 10.
int R = (int) (52.0 * rand() / (RAND_MAX + 1.0)); // [0;51]
int R = (int) _RANDOM(52.0);  // [0;51]
*/
#define _RANDOM(X) X*rand()/(RAND_MAX+1.0); // rand[0;X)

#define REPEAT(X) for(int z=0; z<X; z++)

typedef trio< int,int,int >  CCard; // karta (nominal,suit,id)
typedef std::list< CCard >  CDeck; // koloda
typedef std::list< CCard >  CHand; // ruka
typedef quad< int,CHand,int,int >  CGamer; // igrok (id,card_list,hand_type,kicker)
typedef std::list< CGamer >  CGamers; // igroki
typedef std::pair< CHand,CGamers >  CTable; // stol (+za stolom neskolko igrokov)
typedef quad< int,int,int,int >  CResult; // resultat (user_id,hand_type,kicker,suit)
typedef std::list< CResult >  CResultTable;
//
typedef std::list< CCard >::const_iterator CITER;
typedef std::list< CCard >::iterator ITER;
typedef std::list< CGamer >::iterator GITER;
typedef std::list< CResult >::iterator RITER;


class CPokerGame : public CGame
{
public:
	CTable table;
	CDeck deck, trash;
	CResultTable rt;
	//
	CPokerGame(void);
	~CPokerGame(void);
	int Play();
	int PrepareTable();
	int AddGamers();
	int MixDeck();
	int PrepareDeckManually();
	int OneCardToTrash();
	int DealCards2Gamers();
	int Deal();
	int GetOneCard2Table();
	int GetFlop();
	int SevenCardsHand();
	int GetKicker();
	int ResultTableInit();
	int DetectFlush();
	int GetWinner();
};
