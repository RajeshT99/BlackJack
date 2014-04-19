// Blackjack
 
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>                //Used for srand((unsigned) time(NULL)) command
#include <process.h>             //Used for system("cls") command
 
#define BlackJack 21		// BlackJack is Score of 21
#define Threshold 17		// Dealer keeps drawing cards till her total reaches 17 or above
#define spade 06                 //Used to print spade symbol
#define club 05                  //Used to print club symbol
#define diamond 04               //Used to print diamond symbol
#define heart 03                 //Used to print heart symbol
#define RESULTS "Blackjack.txt"  //File name is Blackjack
 
//Global Variables
int k;
int l;
int d;
int won;
int loss;
int cash = 500;
int bet;
int random_card;
int player_total=0;
int dealer_total;
 
//Function Prototypes
int clubcard();      //Displays Club Card Image
int diamondcard();   //Displays Diamond Card Image
int heartcard();     //Displays Heart Card Image
int spadecard();     //Displays Spade Card Image
int randcard();      //Generates random card
int betting();       //Asks user amount to bet
void asktitle();     //Asks user to continue
void rules();        //Prints "Rules of Vlad's Blackjack" menu
void play();         //Plays game
void dealer();       //Function to play for dealer AI
void stay();         //Function for when user selects 'Stay'
void cash_test();    //Test for if user has cash remaining in purse
void askover();      //Asks if user wants to continue playing
void fileresults();  //Prints results into Blackjack.txt file in program directory


//Main Function
int main(void)
{
	int	ChipCount = 100;	/* Initial Number of Chips with the Player */
	int	BetSize = 1;		/* Minimum Bet Size per Play */
	Char	Deal = " ";		/* Signal to the Dealer to begin "Dealing" the Cards (Y/N) */

	int	CardDeck[52];		/* An Array of Size 52 depicting one deck of cards */
	int	CardCount = 52;		/* Keeps track of Number of Cards left in the Deck */


	SuffleCards();			/* Deler Suffles the Card */

	/* Ask the Player, would she like to begin the game */
	printf("Would you like to play BlackJack? (Y/N)");
	scanf (%c, Deal);

	while ((ChipCount > 0) && (Deal = "Y")) {
		PlayGame ();
	}
     
    asktitle();
     
    printf("\n");
    printf("\n");
    system("pause");
    return(0);
} //end program


/* Shuffle the Card Deck and Initialize CarDeck Array with Card Values */
void ShuffleCards(CardDeck[]) {
	int i;

	for i = 0 to 51 {		/* Initialize the CardDeck Array with 1s, indicating that all cards are available */
		CardDeck[i, 0] = 1;
		ModValue = i Modulo 13;
		if (ModValue == 0) {
			Value = 11;
		} else {
			if ((ModValue > 10) && (ModValue < 13)) Value = 10;
		} else {
			Value = ModValue;
		}
		CardDeck[i, 1] = Value;
	}
}


void PlayOneRound(ChipCount, BetSize) {

	int	PlayerCards[26];	/* Cards issued to the Player are kept in this Array */
	int	DealerCards[26];	/* Cards issued to the Dealer are kept in this Array */

	int	PlayerTotal = 0;	/* Sum of Player's Card Values */
	int	DealerTotal = 0;	/* Sum of Deler's Card Values */

	int	i;


	/* Initialize PlayerCards and DealerCards Arrays with 0s */
	for i = 0 to 25 {
		PlayerCards[i] = 0;
		DealerCards[i] = 0;
	}

	/* Draw initial two cards for the Player and Dealer*/
	PlayerCards[0] = DrawOneCard();
	DisplayCard(PlayerCards[0]);
	DealerCards[0] = DrawOneCard();
	DisplayCard(DealerCards[0]);

	PlayerCards[1] = DrawOneCard();
	DisplayCard(PlayerCards[1]);
	DealerCards[0] = DrawOneCard();	/* Keep the 2nd Dealer Card Hidden for Now */

	PlayerTotal = PlayerCards[0] + PlayerCards[1];
	DealerTotal = DealerCards[0] + DealerCards[1];

	/* Check whether Player or Dealer have received a BlackJack, i.e. a total of 21 */
	If (PlayerTotal == 21) {
		If (DealerTotal == 21) {
			printf("PUSH\n");
			RoundOver = 'Y';
		} else {
			printf("Player Won\n");
			ChipCount = ChipCount + BetSize;
			RoundOver = 'Y';
		}
	}

	/* If the Round is Over then Return */
	if (RoundOver == 'Y') return();

	/* Ask Player, whether she would like a "Hit" or "Stand" the Round */
	printf("Would you like a "Hit" or "Stand"\n");
	scanf(%s, PlayerChoice);

	While (PlayerChoice == "Hit") {
		PlayerCards[i] = DrawOneCard();
		DisplayCard(PlayerCards[i]);

		PlayerTotal = PlayerTotal = PlayerCards[i];

		if (PlayerTotal == BlackJack) {
			if (DealerTotal == BlackJack) {
				printf("PUSH\n");
				RoundOver = 'Y';
			} else {
				printf("Player Won!\n");
				ChipCount = ChipCount + BetSize;
				RoundOver = 'Y'
			}
		} else if (PlayerTotal > BlackJack) {
			/* Player has Busted */
			printf("Dealer has Won!\n");
			ChipCount = ChipCount - BetSize;
			RoundOver = 'Y';
		}
	}

	/* If the Round is Over then Return */
	if (RoundOver == 'Y') return();
	
	
	/* When Player Choice is "Stand", then draw Cards for the Dealer till Delaer Wins or goes Bust */
	If (PlayerChoice == "Stand") {
		/* Display Dealer's Hidden Card */
		DisplayCard(DealerCards[1]);	/* Assuming Dealer Keeps the 2nd Card Hidden */

	while (DealerTotal < Threshold) {
		DealerCards = DrawOneCard();

		/*Compute Total while judging whether to count an 'Ace' as 1 or 11 */
		DealerTotal = ComputeTotal(DealerCards());
	}

	/* Check whether the Dealer has Busted */
	if (DealerTotal > BlackJack) {
		/* Dealer has Busted */
		printf("Player Won!\n");
		ChipCount = ChipCount + BetSize;
		RoundOver = 'Y';
	} else if (PlayerToal > DealerTotal) {
		/* Player has Won */
		printf("Player Won!\n");
		ChipCount = ChipCount + BetSize;
		RoundOver = 'Y';
	} else if (PlayerTotal < DealerTotal) {
		/* Dealer has Won!\n");
		printf("Dealer Won!\n");
		ChipCount = ChipCount - BetSize;
		RoundOver = 'Y';
	} else {
		/ The Round is a Draw */
		printf("PUSH\n");
		RoundOver = 'Y';
	}
}
	
	

/* Function to Draw One Card from the Deck */
int DrawCard(CardDeck[]) {
	int	Card;
	
	Card = (srand(), mod 51);
	while (CardDeck[Card] == 0) {
		Card = (srand(), mod 51);
	}
	CardDeck[Card] = 0;
	return (Card);
}
