// Blackjack Game written by Rajesh Trivedi in 'C' Programming Language
// (c) Rajesh Trivedi - April 2014

 
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>                //Used for srand((unsigned) time(NULL)) command
#include <process.h>             //Used for system("cls") command

#define BlackJack 21		// BlackJack is Score of 21
#define Threshold 17		// Dealer keeps drawing cards till her total reaches 17 or above
#define	AceValue  11		// Default BlackJack Value of an Ace Card

#define	PlayerCard "Player Card:"	// String used in diaplaying Player Cards
#define	DealerCard "Dealer Card:"	// String used in displaying Dealer Cards

#define	Heart	01		// Heart Card
#define	Spade	02		// Spade Card
#define	Diamond	03		// Diamond Card
#define	Club	04		// Club Card

#define	TRUE	01
#define	FALSE	00


/************************************************************************************************
 * Global Data Structure for the Card Deck.							*
 *												*
 * Normally, I don't use Global Variables as these can lead to unintended side-effects.		*
 * However, in my implementation of BlackJack, I was passing this Matrix back and forth between	*
 * a number of modular functions, which could have affected the performance.			*
 * Therefore, I implemented the CardDeck Matrix as a Global Variable.				*
 *												*
 * Assuming that the Game is being played with just One Deck of Cards.				*
 * Shuffling is implemented by randomly picking a Card from the Deck using the rand() function.	*
 * When less than 26 Cards are Left, Dealer Combines and Initializes the CardDeck before	*
 * beginning a New Game.									*
 *												*
 * Each Row of CardDeck Matrix contains 4 Fields for:						*
 * 		- Avail Flag = 1 Flags that the Card is Available.				*
 *		- Card Type = An Integer indicating Heart, Spade, Diamond, or Club		*
 *		- Face Value = An Integer indicating the Face Value of Card from 1 to 13.	*
 *				1 is Ace; 11 is Jack; 12 is Queen; 13 is King.			*
 * 		- Black Value = BlackJack Value of the Card from 1 to 11.			*
 *				Jack, Queen, and King have Black Value of 10.			*
 *				Ace can have a value of 1 or 11.				*
 *												*
 ************************************************************************************************/

int	CardDeck[52][4];	// A Matrix of 52 Rows and 4 Column depicting one deck of cards.
int	CardCount = 52;		// Keeps track of Number of Cards left in the Deck.
				// When a Card is drawn, Counter is decremented.

// Indexes for CardDeck Matrix Columns
#define	AvailFlagInd	00	// Index for the Card Availability Flag
#define	CardTypeInd	01	// Index for the Card Type
#define FaceValueInd	02	// Index for the Face Value of the Card so that it can be Displayed
#define BlackValueInd	03	// Index for the BlackJack Value of a Card


// Function Prototypes
void	InitializeCardDeck();	// Function to Initialize the Dealer's Card Deck
int	DrawOneCard();		// Randomly Draw One Card from the Deck
void	DisplayCard();		// Display One Card on the Screen
char	PlayOneRound();		// Play One Round of the BlackJack Game
int	ComputePlayerTotal();	// Compute Total keeping in track Ace value of 11 or 1
int	ComputeDealerTotal();	// Context-sensitive computation of Dealer's Card Total
void	Clear();		// Function to Clear Input Buffer of Left-over CR, LF, and Garbage



//Main Function
int main(void)
{
	int	ChipCount = 100;	// Initial Number of Chips with the Player
	int	BetSize = 1;		// Minimum Bet Size per Play
	int	CorrectBet = FALSE;	// Boolean Flag to Indicate whether the Bet is in Range
	char	Deal = ' ';		// Signal to the Dealer to begin "Dealing" the Cards (Y/N)
	char	WhoWon = ' ';		// Player (P), Dealer (D), or Even (E)



	// Clear the Screen before begining the Game.
	system("clear");
	printf("\nWelcome to Rajesh Trivedi's BlackJack Casino!\n");

	// Initialize the CardDeck Matrix.
	// Shuffle is implemented by randomly picking a card from Deck.
	InitializeCardDeck();

	// Ask the Player, would she like to begin the game.
	printf("\nWould you like to play a Game of BlackJack? (Y/N): ");
	scanf("%c", &Deal);
	Clear();
	printf("\n");

	// Check whether the Player has Entered a Correct Choice
	while ((Deal != 'Y') && (Deal != 'y') && (Deal != 'N') && (Deal != 'n')) {
		printf("Incorrect Choice -- Please Enter Y for Yes or N for No: ");
	  	scanf("%c", &Deal);
		Clear();
		printf("\n");
	}

	// Ask Player the BetSize
	if ((Deal == 'Y') || (Deal == 'y')) {
		printf("Please enter the Bet Size (between 1 and %d): ", ChipCount);
		scanf("%d", &BetSize);
		Clear();
		printf("\n");

		// Check whether the Player has Entered a Correct Bet Size
		while ((BetSize < 1) || (BetSize > ChipCount)) {
			printf("Incorrect Bet Size -- Please Enter a Number between 1 and %d: ", ChipCount);
			scanf("%d", &BetSize);
			Clear();
			printf("\n");
		}
		CorrectBet = TRUE;
	}

	while (((Deal == 'Y') || (Deal == 'y')) && (CorrectBet)) {

		// If less than half the cards are left in Deck, then Initialize or Reshuffle it.
		if (CardCount < 26) InitializeCardDeck();

		// Play one round of the BlackJack Game and Return with WhoWon.
		WhoWon = PlayOneRound();

		// Increment or Decrement Player's ChipCount with the BetSize
		switch (WhoWon) {
			case 'P':
				printf("\nResult = Player Won!\n");
				ChipCount = ChipCount + BetSize;
				break;
			case 'D':
				printf("\nResult = Dealer Won!\n");
				ChipCount = ChipCount - BetSize;
				break;
			case 'E':
				printf("\nResult = Draw\n");
		}

		printf("\nRemaining Money = %i\n", ChipCount);
		printf("\n****************** End of Round *********************\n");

		// Check whether the Player has any Money Left
		if (ChipCount <= 0) {
			printf("\nPlayer has become Bankrupt. Cannot Continue the Game.\n");
			return (0);
		}

		Deal = ' ';
		printf("\nWould you like to play Another Round? (Y/N): ");
		scanf("%c", &Deal);
		Clear();
		printf("\n");

		// Check whether the Player has Entered the Correct Choice
		while ((Deal != 'Y') && (Deal != 'y') && (Deal != 'N') && (Deal != 'n')) {
			printf("Incorrect Choice -- Please Enter Y for Yes or N for No: ");
	  		scanf("%c", &Deal);
			Clear();
			printf("\n");
		}

		// Ask Player the BetSize
		CorrectBet = FALSE;
		BetSize = 1;	// Default Minimum Bet Size
		if ((Deal == 'Y') || (Deal == 'y')) {
			printf("Please enter the Bet Size (between 1 and %d): ", ChipCount);
			scanf("%d", &BetSize);
			Clear();
			printf("\n");

			// Check whether the Player has Entered the Correct Bet Size
			while ((BetSize < 1) || (BetSize > ChipCount)) {
				printf("Incorrect Bet Size -- Please Enter a Number between 1 and %d: ", ChipCount);
				scanf("%d", &BetSize);
				Clear();
				printf("\n");
			}
			CorrectBet = TRUE;
		}

	}
	// system("PAUSE");
	return(0);
} //end program



/* Initialize the CardDeck Matrix */
void InitializeCardDeck() {
	int 	i;
	int 	ModValue;


	// Shuffle the Card Deck by Initializing all 52 Rows
	printf("\nShuffling the Card Deck ...\n");

	for (i = 0; i < 52; i++) {

		// Initialize the Avail Flag with 1s indicating that all cards are available.
		CardDeck[i][AvailFlagInd] = 1;

		// Initialize Card Type with one of the 4 Values - Heart, Spade, Diamond, or Club.
		CardDeck[i][CardTypeInd] = ((int) (i / 13)) + 1;

		// Initialize CardDeck with Face Values and BlackJack Values.
		ModValue = ((i % 13) + 1);
		CardDeck[i][FaceValueInd] = ModValue;

		switch (ModValue) {
			case 1:
				// Default BlackJack Value of Ace card is 11.
				CardDeck[i][BlackValueInd] = 11;
				break;
			case 11:
				// BlackJack Value of Jack card is 10.
				CardDeck[i][BlackValueInd] = 10;
				break;
			case 12:
				// BlackJack Value of Queen card is also 10.
				CardDeck[i][BlackValueInd] = 10;
				break;
			case 13:
				// BlackJack Value of King card is also 10.
				CardDeck[i][BlackValueInd] = 10;
				break;
			default:
				// BlackJack Value of other Cards is their Numeric Value.
				CardDeck[i][BlackValueInd] = ModValue;
		}
	}
	// Initialize the CardCount with 52 indicating that all Cards are available for Dealing.
	CardCount = 52;
}



/************************************************************************************************
 * Function to Draw One Available Card from the Deck						*
 ************************************************************************************************/

int DrawOneCard() {

	int	iCard;

	// Generate random seed for rand() function
	srand((unsigned) time(NULL));

	iCard = (rand() % 51);
	while (CardDeck[iCard][AvailFlagInd] == 0) {
		// Keep Looking till you find an available Card in the Deck
		iCard = (rand() % 51);
	}
	CardDeck[iCard][AvailFlagInd] = 0;	// Set the Card Availability Flag to 0
	CardCount--;				// Decrement the CardCount by 1
	return (iCard);
}



/************************************************************************************************
 * Function to Display a Card on the Screen							*
 * Displays Cards simply as "Heart--Ace" or "Club--10" or "Diamond--Jack" per line.		*
 ************************************************************************************************/

void DisplayCard (char* WhoseCard, int iCard) {

	int	CardType  = CardDeck[iCard][CardTypeInd];
	int	FaceValue = CardDeck[iCard][FaceValueInd];

	switch (CardType) {	// Switch for 4 Card Types
		case Heart:
			printf("%s Heart--", WhoseCard);
			break;
		case Spade:
			printf("%s Spade--", WhoseCard);
			break;
		case Diamond:
			printf("%s Diamond--", WhoseCard);
			break;
		case Club:
			printf("%s Club--", WhoseCard);
	}

	switch (FaceValue) {	// Switch for Different Card Values
		case 1:
			printf("Ace\n");
			break;
		case 11:
			printf("Jack\n");
			break;
		case 12:
			printf("Queen\n");
			break;
		case 13:
			printf("King\n");
			break;
		default:
			printf("%i\n", FaceValue);
	}
}



/************************************************************************************************
 * Function to Play One Round of the BlackJack.							*
 * Function Returns a Char:									*
 *			- 'P' if the Player Wins;						*
 *			- 'D' if the Dealer Wins;						*
 *			- 'E' if the Game is Even or "Push"					*
 *												*
 ************************************************************************************************/

char PlayOneRound() {

	// Need to keep track of Cards issued to Player and Dealer because some times
	// Ace is counted as 1 while at other times as 11.
	// It is unlikely that more than 26 Cards will be issued to Player or Dealer in One Round.
	int	PlayerCards[26];	// Cards issued to the Player are kept in this Array.
	int	DealerCards[26];	// Cards issued to the Dealer are kept in this Array

	int	PlayerTotal = 0;	// Sum of Player Cards' BlackJack Values
	int	DealerTotal = 0;	// Sum of Deler Cards' BlackJack Values

	char	WhoWon = 'E';		// Return WhoWon to the Main program
	char	RoundOver = 'N';	// A Flag which indicates whether a Round is Over
	char    PlayerChoice = ' ';	// Variable to accept Player's choice - Hit (H) or Stand (S)
	int	i;			// Loop Counter


	// Initialize PlayerCards and DealerCards Arrays with -1s.
	for (i = 0; i < 26; i++) {
		PlayerCards[i] = -1;
		DealerCards[i] = -1;
	}

	// Draw initial two cards for the Player and Dealer.
	PlayerCards[0] = DrawOneCard();
	DisplayCard(PlayerCard, PlayerCards[0]);
	DealerCards[0] = DrawOneCard();
	DisplayCard(DealerCard, DealerCards[0]);

	PlayerCards[1] = DrawOneCard();
	DisplayCard(PlayerCard, PlayerCards[1]);
	DealerCards[1] = DrawOneCard();
	// Keep the 2nd Dealer Card Hidden for Now.

	// Determine Player Total while accounting for the 'Soft' value of Aces (11 or 1).
	PlayerTotal = ComputePlayerTotal(PlayerCards, 2);
	DealerTotal = ComputeDealerTotal(DealerCards, 2);
	printf("Player Total = %d\n", PlayerTotal);

	// Check whether Player or Dealer have received a BlackJack, i.e. a total of 21.
	if (PlayerTotal == BlackJack ) {
		if (DealerTotal == BlackJack ) {
			WhoWon = 'E';		// Round ended as Even
			RoundOver = 'Y';
		} else {
			WhoWon = 'P';		// Player won the Round
			RoundOver = 'Y';
		}
	}

	// if the Round is Over then Return.
	if (RoundOver == 'Y') {
		// Display Dealer's 2nd Hidden Card
		DisplayCard(DealerCard, DealerCards[1]);
		printf("Dealer Total = %d\n", DealerTotal);
		return(WhoWon);
	}


	// Ask Player, whether she would like a "Hit" or 'S' the Round.
	printf("\nWould you like a Hit or Stand (H/S): ");
	scanf("%c", &PlayerChoice);
	Clear();
	printf("\n");

	// Check whether the Player Choice is Correct or Not
	while ((PlayerChoice != 'H') && (PlayerChoice != 'h') &&
	       (PlayerChoice != 'S') && (PlayerChoice != 's')) {
		printf("Incorrect Choice -- Please Enter H for Hit or S for Stand: ");
	  	scanf("%c", &PlayerChoice);
		Clear();
		printf("\n");
	}

	
	i = 2;	// Player has already received 2 Cards
	while (((PlayerChoice == 'H') || (PlayerChoice == 'h')) && (RoundOver == 'N')) {

		// Draw a card for the Player and Display it.
		PlayerCards[i] = DrawOneCard();
		DisplayCard(PlayerCard, PlayerCards[i]);

		// Determine Player Total while accounting for the 'Soft' value of Aces.
		PlayerTotal = ComputePlayerTotal(PlayerCards, i+1);
		printf("Player Total = %d\n", PlayerTotal);

		if (PlayerTotal == BlackJack) {
			if (DealerTotal == BlackJack) {
				WhoWon = 'E';
				RoundOver = 'Y';
			} else {
				WhoWon = 'P';
				RoundOver = 'Y';
			}
		} else if (PlayerTotal > BlackJack) {
			// Player has Busted
			WhoWon = 'D';
			RoundOver = 'Y';
		}
		
		if (RoundOver == 'Y') {
			// Now Display Dealer's 2nd Hidden Card
			DisplayCard(DealerCard, DealerCards[1]);
			printf("Dealer Total = %d\n", DealerTotal);
			return(WhoWon);
		}

		// Ask Player, whether she would like a "Hit" or 'S' the Round.
		PlayerChoice = ' ';	// Initialize PlayerChoice
		printf("\nWould you like a Hit or Stand (H/S): ");
		scanf("%c", &PlayerChoice);
		Clear();
		printf("\n");

		// Check whether the Player Choice is Correct or Not
		while ((PlayerChoice != 'H') && (PlayerChoice != 'h') &&
		       (PlayerChoice != 'S') && (PlayerChoice != 's')) {
			printf("Incorrect Choice -- Please Enter H for Hit or S for Stand: ");
		  	scanf("%c", &PlayerChoice);
			Clear();
			printf("\n");
		}

		i++;
	}


	// At this Point, Player has Selected 'S' or 's'.
	// When Player Choice is 'S', then draw Cards for the Dealer till Delaer Wins or goes Bust.
	// Dealer must draw on 16 and soft 17.
	// Compute Total while judging whether to count an 'Ace' as 1 or 11.
	DealerTotal = ComputeDealerTotal(DealerCards, 2);

	// Display Dealer's Hidden Card.
	// Assuming Dealer Keeps the 2nd Card Hidden.
	DisplayCard(DealerCard, DealerCards[1]);
	printf("Dealer Total = %d\n", DealerTotal);

	i = 2;
	while (DealerTotal < Threshold) {
		DealerCards[i] = DrawOneCard();
		DisplayCard(DealerCard, DealerCards[i]);

		// Compute Total while judging whether to count an 'Ace' as 1 or 11.
		DealerTotal = ComputeDealerTotal(DealerCards, i+1);
		printf("Dealer Total = %d\n", DealerTotal);
		i++;
	}

	// Check whether the Dealer has Busted.
	if (DealerTotal > BlackJack) {
		// Dealer has Busted
		WhoWon = 'P';
		RoundOver = 'Y';
	} else if (PlayerTotal > DealerTotal) {
		// Player has Won
		WhoWon = 'P';
		RoundOver = 'Y';
	} else if (PlayerTotal < DealerTotal) {
		// Dealer has Won
		WhoWon = 'D';
		RoundOver = 'Y';
	} else {
		// The Round is a Draw
		WhoWon = 'E';
		RoundOver = 'Y';
	}
	return (WhoWon);
}



/************************************************************************************************
 * Compute BlackJack Total while keeping track of Dual Values of Ace Cards (11 or 1).		*
 * For Players Ace needs to be counted as 1 only when she is about to go bust (> 21).		*
 * Function Inputs:										*
 *		- Cards[] = Array of Cards issued to Player or Dealer				*
 *		- Count = Num of Cards issued to Player or Dealer				*
 * Function Returns: Total BlackJack Value of the Cards isued to Player / Dealer		*
 ************************************************************************************************/

int ComputePlayerTotal (int* Cards, int Count) {

	int	i;
	int	Total = 0;


	// Compute Total while accounting for Ace Cards as 11.
	for (i = 0; i < Count; i++) {
		Total = Total + CardDeck[Cards[i]][BlackValueInd];
	}

	// While Total is > 21 Find Aces and Count them as 1.
	i = 0;
	while ((Total > BlackJack) && (i < Count)) {
		if (CardDeck[Cards[i]][BlackValueInd] == AceValue) {
			Total = Total - 10;
		}
		i++;
	}
	return (Total);
}


/************************************************************************************************
 * Compute BlackJack Total while keeping track of Dual Values of Ace Cards (11 or 1).		*
 *												*
 * if the Dealer has not received a BlackJack (21) in the first two Cards and			*
 * the Player has said 'S', then she needs to keep hitting till she reaches 			*
 * the Threshold (17) and Wins or Goes Bust (above 21).						*
 * The Dealer needs to hit at 16 and soft 17 (i.e., when some of the Cards are Ace).		*
 *												*
 * For Dealer Ace needs to be counted as 1 when Total is 17 or above 21.			*
 * Function Inputs:										*
 *		- Cards[] = Array of Cards issued to Player or Dealer				*
 *		- Count = Num of Cards issued to Player or Dealer				*
 * Function Returns: Total BlackJack Value of the Cards isued to Player / Dealer		*
 ************************************************************************************************/

int ComputeDealerTotal (int* Cards, int Count) {

	int	i;
	int	Total = 0;
	int	AceFound = FALSE;


	// Compute Total while accounting for Ace Cards as 11 (default value of Ace Cards).
	for (i = 0; i < Count; i++) {
		Total = Total + CardDeck[Cards[i]][BlackValueInd];
	}

	// Dealer needs to Hit at "Soft 17".
	// If Dealer's Total is 17 and there is an Ace, then find one Ace and count it as 1.
	if (Total == Threshold) {
		i = 0;
		while ((!AceFound) && (i < Count)) {
			if (CardDeck[Cards[i]][BlackValueInd] == AceValue) {
				Total = Total - 10;
				AceFound = TRUE;
			}
			i++;
		}
	}

	// While Total is > 21 Find Aces and Count them as 1.
	i = 0;
	while ((Total > BlackJack) && (i < Count)) {
		if (CardDeck[Cards[i]][BlackValueInd] == AceValue) {
			Total = Total - 10;
		}
		i++;
	}
	return (Total);
}


/* Function to Clear the Input Buffer of CR, LF, and other left-over Garbage after a scanf(). */

void Clear (void)
{    
  while ( getchar() != '\n' );
}
