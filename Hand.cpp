#include "Hand.h"

CHand::CHand(void)
{
	hand_id = 0;
	hand_type = 0;
	kicker = 0;
	kicker_card_id = 0;
}

CHand::~CHand(void)
{
}


//---------------------------------------------------------
// 
//
int CHand::DetectHandType()
{
	int Flush = DetectFlush();

	if( Flush && (kicker == VAL_ACE) )
	{
		return HT_ROYAL_FLUSH;
	}
	else
	{
		if( DetectStraightFlush() )
		{
			return HT_STRAIGHT_FLUSH;
		}
		else
		{
			if( DetectQuad() )
			{
				return HT_QUAD;
			}
			else
			{
				if( DetectFullHouse() )
				{
					return HT_FULL_HOUSE;
				}
				else
				{
					if( Flush )
					{
						return HT_FLUSH;
					}
					else
					{
						if( DetectStraight() )
						{
							return HT_STRIGHT;
						}
						else
						{
							if( DetectTrio() )
							{
								return HT_TRIO;
							}
							else
							{
								if( DetectTwoPairs() )
								{
									return HT_TWO_PAIRS;
								}
								else
								{
									if( DetectPair() )
									{
										return HT_PAIR;
									}
									else
									{
										GetHighCard();
									}
								}
							}
						}
					}
				}
			}
		}
	}
	return HT_HIGH_CARD;
}

//---------------------------------------------------------
//       FLUSH
//
int CHand::DetectFlush()
{
	int clubs = 0;
	int spades = 0;
	int hearts = 0;
	int diamonds = 0;

	int kicker_clubs = 0;
	int kicker_spades = 0;
	int kicker_hearts = 0;
	int kicker_diamonds = 0;

	int kicker_clubs_id = 0;
	int kicker_spades_id = 0;
	int kicker_hearts_id = 0;
	int kicker_diamonds_id = 0;

	// NOTE: card(value,suit,id)

	for (DeckIter di = deck.begin(), end = deck.end(); di != end; ++di) // for each card in hand
	{
		switch(di->second) // card suit
		{
			case SUIT_CLUBS:
				clubs++;
				if(kicker_clubs < di->first) // value
				{
					kicker_clubs = di->first;
					kicker_clubs_id = di->third; // card id
				}
			break;

			case SUIT_SPADES:
				spades++;
				if(kicker_spades < di->first)
				{
					kicker_spades = di->first;
					kicker_spades_id = di->third; // card id
				}
			break;

			case SUIT_HEARTS:
				hearts++;
				if(kicker_hearts < di->first)
				{
					kicker_hearts = di->first;
					kicker_hearts_id = di->third; // card id
				}
			break;

			case SUIT_DIAMONDS:
				diamonds++;
				if(kicker_diamonds < di->first)
				{
					kicker_diamonds = di->first;
					kicker_diamonds_id = di->third; // card id
				}
			break;

			default:
			break;
		}
	}

	if(clubs >= 5)
	{
		hand_type = HT_FLUSH;
		kicker = kicker_clubs;
		kicker_card_id = kicker_clubs_id;
		return HT_FLUSH + SUIT_CLUBS;
	}


	if(spades >= 5)
	{
		hand_type = HT_FLUSH;
		kicker = kicker_spades;
		kicker_card_id = kicker_spades_id;
		return HT_FLUSH + SUIT_SPADES;
	}

	if(hearts >= 5)
	{
		hand_type = HT_FLUSH;
		kicker = kicker_hearts;
		kicker_card_id = kicker_hearts_id;
		return HT_FLUSH + SUIT_HEARTS;
	}

	if(diamonds >= 5)
	{
		hand_type = HT_FLUSH;
		kicker = kicker_diamonds;
		kicker_card_id = kicker_diamonds_id;
		return HT_FLUSH + SUIT_DIAMONDS;
	}

	return 0;
}

//---------------------------------------------------------
//       STRAIGHT 
//
int CHand::DetectStraight(DeckIter SrcD, int* /*OUT*/ kicker_id)
{
	int count[3], ids[3];
	CDeck TempD;
	CCard PokerCard;

	memset(count,0,sizeof(int)*3);
	memset(ids,0,sizeof(int)*3);

	for(int k=0; k<3; k++)
	{
		DeckIter TempIter = SrcD;
		for(int i=0; i<5; i++, TempIter++) // suit
		{
				PokerCard.first  = TempIter->first; // value
				PokerCard.second = TempIter->second; // suit
				PokerCard.third  = TempIter->third; // id
				TempD.push_back(PokerCard);
		}

		DeckIter ti = TempD.begin();
		ti++;

		int n=0;
		for (DeckIter di = TempD.begin(), end = TempD.end(); di != end; ++di, n++) // for each card in hand
		{
			if (ti != end)
			{
				if(n == 3)
				{
					ids[k] = ti->third;
				}
				if( di->first == (ti->first - 1) )
				{
					count[k]++;
				}
				else
				{
					count[k]--;
				}
				ti++;
			}
		}
		TempD.clear();
		SrcD++;
	}

	if( (count[0] >= 4)  && (count[1] < 4) && (count[2] < 4) )
	{
		//*kicker = 5; // position 5 of 7
		*kicker_id = ids[0];
		return count[0];
	}
	if( (count[1] >= 4) && (count[2] < 4) )
	{
		//*kicker = 6; // position 7 of 7
		*kicker_id = ids[1];
		return count[1];
	}
	if( count[2] >= 4 )
	{
		//*kicker = 7; // position 7 of 7
		*kicker_id = ids[2];
		return count[2];
	}

	return 0;
}
//---------------------------------------------------------
//       Test STRAIGHT state
//
void CHand::TestStraight()
{
	CDeck TestDeck;
	CCard PokerCard;

	PokerCard.first  = 7; // value
	PokerCard.second = 0; // suit
	PokerCard.third  = 1; // id
	TestDeck.push_back(PokerCard);

	PokerCard.first  = 4; // value
	PokerCard.second = 0; // suit
	PokerCard.third  = 2; // id
	TestDeck.push_back(PokerCard);

	PokerCard.first  = 10; // value
	PokerCard.second = 0; // suit
	PokerCard.third  = 3; // id
	TestDeck.push_back(PokerCard);

	PokerCard.first  = 3; // value
	PokerCard.second = 0; // suit
	PokerCard.third  = 4; // id
	TestDeck.push_back(PokerCard);

	PokerCard.first  = 6; // value
	PokerCard.second = 0; // suit
	PokerCard.third  = 5; // id
	TestDeck.push_back(PokerCard);

	PokerCard.first  = 2; // value
	PokerCard.second = 0; // suit
	PokerCard.third  = 6; // id
	TestDeck.push_back(PokerCard);

	PokerCard.first  = 5; // value
	PokerCard.second = 0; // suit
	PokerCard.third  = 7; // id
	TestDeck.push_back(PokerCard);
	//--------------------------------------
	// sorting
	TestDeck.sort();
	//--------------------------------------
	DeckIter ti = TestDeck.begin();

	int kicker_id = 0;
	int count =  DetectStraight(ti, &kicker_id);
 
	if( count >= 4 )
	{
		MessageBox(L"Straight!!!",0,MB_OK+MB_ICONASTERISK);
	}
	else
	{
		MessageBox(L"Straight not found!",0,MB_OK+MB_ICONERROR);
	}
}
