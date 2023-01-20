/*============================================================================
Names:       Rachel V., Cade R., Destiny C. and Grant L.
Date:        December 7th, 2022
Instructor:  Dr. Zhao
Class:       Network Programming
Purpose:     This program uses a server to implement the card game
             black jack.
============================================================================*/
#include "blackjack.h"
using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

  /*===========================================================================
  CARD STRUCTURE:
  Description: Holds the value and suit of a single card. These card structs 
                will be held in vectors and represent a player/dealers hand.
  ===========================================================================*/
  struct Card 
  {
    int value;
    int suit;
  };

  class BlackJackGame {
  public:

    int player_bank_account;     // Total amount of money player brings to casino
    int current_bet;             // Amount of money they want to bet on the current game
    int earnings;                // Positive or negative earnings of all games played
    int wins;                    // the number of times the player won (or tied) total
    int losses;                  // the number of times the player lost total
    int ties;
    int minimum = 5;             //Minimum bet is $5.00
    int dealer_hand_value;       //Holds the value of the dealers hand
    int player_hand_value;       //Holds the value of the dealers hand

    // A playing card is one of 4 suits
    static const int HEARTS = 0;
    static const int CLUBS = 1;
    static const int SPADES = 2;
    static const int DIAMONDS = 3;
    static const int NUM_SUITS = 4;
  
    // and 13 ranks (A,2,3,4,5,6,7,8,9,10,J,Q,K)
    static const int ACE = 1;
    static const int JACK = 11;
    static const int QUEEN = 12;
    static const int KING = 13;
    static const int NUM_RANKS = 13;
    static const int NUM_CARDS = NUM_SUITS * NUM_RANKS; // 52
  
    // Make the deck, players hand and dealers hand
    Card deck[NUM_CARDS];
    vector<Card> dealerHand;
    vector<Card> playerHand;
    

  /*===========================================================================
  INITIALIZE BANK FUNCTION:                          
  Decription: This function asks the user how much money they brought to the 
  casino and then saves it to the player bank account variable. This function
  also performs input validation and prints the players bank account. 
  ===========================================================================*/
  void initialize_bank() 
{
  
    cout << "How much money worth of chips would you like to purchase for the "   //Ask the user how much money they would like to play with
            "evening? ";
  
    cin >> player_bank_account;                                         //Initialize the player bank account to users input
    while(validateInt(player_bank_account) == false){                    //Loop if the user enters invalid input
        cout << "\nInvalid format! Please re-enter your deposit: ";
		  cin.clear();                                                     //Clear the user input buffer
	  	cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');   //Ignores input up until the next new line happens
	  	cin >> player_bank_account;										                   //Allow user to re-enter the amount of money to deposit
      }
                                                                    //User must purchase at least $5 in chips
    while (player_bank_account < minimum ) {                        //If the user enters a value less than $5
      cout << "\nTry again. Minimum purchase of at least $5.00 in chips: ";
      cin >> player_bank_account;
       while(validateInt(player_bank_account) == false){                //If the value the user entered is not an integer
        cout << "\nInvalid format! Please re-enter your deposit: ";    //Ask them to re-enter their deposit
		  cin.clear();                                                    //Clear the user input buffer
	  	cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //Ignores input up until the next new line happens
	  	cin >> player_bank_account;										                //Allow user the to re-enter the amount of money to deposit
      }
    }
  
    cout << "\nGreat! $" << player_bank_account << ".00 has been added to your chip bank.";
    cout << "\nYour current chip bank balance is $" << this->check_bank() <<".00"<<endl;
    cin.clear();
  }

  /*===========================================================================
  CALCULATE HAND FUNCTION:                          
  Input:       A vector of card objects that represent either a player
               or a dealers hand.
  Decription:  This function calculates the value of the hand passed
               in and returns an int. This function handles the "ace" card
               which will automatically be counted as an 11. If a player's
               hand value is over 21 then the "ace" will be counted as a 1. 
  ===========================================================================*/
  int calculate_hand(const vector<Card> my_hand) {

    int sum = 0; // To hold the sum
    bool containsAce = false; // This flag will be set it the hand contains an Ace the user
               // will need to decide How they want to handle the ace.
    for (int i = 0; i < my_hand.size(); i++) {
      // Jack, queen, or king card
      if (my_hand[i].value > 10) {
        sum += 10;
      }
      // Ace card
      else if (my_hand[i].value == 1) {
        containsAce = true;
        {            // Set the ace flag to true
          sum += 11; // count as 11 for now
        }
      }
      // All cards between 2-9
      else {
        sum += my_hand[i].value;
      }
    }
    // If there was an ace then make the best decision after all cards have been
    // added up
    if (containsAce == true) {
      // If using A as an 11 caused a bust then use Ace as a 1.
      // Subtract 11 Plus 1 so basically minus 10
      if (sum > 21) {
        sum -= 10;//remove 11 and add a 1 
      }
    }
    return sum;
  }

  /*===========================================================================
  CHECK BANK FUNCTION:                           
  Decription: This function returns how much money the player has in 
              their chip bank.
  ===========================================================================*/
  int check_bank() 
  {
    return player_bank_account;  //Return player bank account
  }

  /*===========================================================================
  PICK WINNER FUNCTION:                           
  Decription: This function calculates the winner by checking if either the player
              or the dealer has busted. If no one busted then the person with the 
              highest hand wins the game. If the player wins then this function also
              handles adding 2 times their bet to the players chip bank. Lastly, this
              function updates the wins, losses, ties and total earnings.
  ===========================================================================*/
  void pickWinner() {
    cout << "\n+-------------------------------------------------------------------------------+\n";
    cout << "|                                 GAME RESULTS:                                 |\n";
    cout << "+-------------------------------------------------------------------------------+\n\n";

    //Calculat the hand values 
    this->player_hand_value = calculate_hand(playerHand);
    this->dealer_hand_value = calculate_hand(dealerHand);
    
    
    // if BOTH busted no one wins,incriment ties, give money back
    if (check_bust(player_hand_value) && (check_bust(dealer_hand_value))){
      cout<<"*** BOTH BUSTED! ***\n";
      cout<<"*** NO ONE WINS! ***\n\n";
      player_bank_account += current_bet;                    //Give the player their money back
      cout << "Player bet $" << current_bet <<".00 on this game" << endl;
      cout<<"Player got $"<<current_bet<<".00 back. \n";     //Print the current back
      ties++;                                                //Incriment ties
    }
    // If the player busted, money was already subtracted in place_bet so just deduct from earnings
    else if (check_bust(player_hand_value)) {
      cout<<"*** PLAYER BUSTED! ***\n";
      cout<<"*** DEAlER WINS!   ***\n\n";
      cout << "Player bet $" << current_bet <<".00 on this game." << endl;
      cout<<"Player lost $"<<current_bet<<".00. \n";
      earnings -=current_bet;                                //deduct the current bet from their earnings
      losses++;                                              //Incriment losses 
    }
    // Check if the dealer busted, player wins 2x bet
    else if (check_bust(dealer_hand_value)){
      cout<<"*** DEALER BUSTED! ***\n";
      cout<<"*** PLAYER WINS!   ***\n\n";
      cout << "Player bet $" << current_bet <<".00 on this game" << endl;
      cout<<"Player won $"<<(2*current_bet)<<".00. \n";
      player_bank_account = player_bank_account + (2* current_bet); //Adding 2* player bet to their account
      earnings +=current_bet;                                       //Add current bet from their earnings
      wins++;                                                       //Incriment wins 
    }
      
    //if neither person busted and the player hand is greater
    else if (player_hand_value > dealer_hand_value ){
      cout<<"*** PLAYER WINS! ***\n";
      cout << "Player bet $" << current_bet <<".00 on this game" << endl;
      cout<<"Player won $"<<(2*current_bet)<<".00. \n\n";
      player_bank_account = player_bank_account + (2* current_bet); //Adding 2* player bet to their account
      earnings +=current_bet;                               //Add current bet from their earnings
      wins++;                                               //Incriment wins 
    }
      
    // if neither person busted and the dealer hand is greater
    else if (dealer_hand_value > player_hand_value ){
      cout<<"*** DEALER WINS! ***\n\n";
      cout << "Player bet $" << current_bet <<".00 on this game" << endl;
      cout<<"Player lost $"<<current_bet<<".00. \n";
      earnings -=current_bet;                              //deduct bet from earnings since player lost
      losses++;                                            //Incriment losses
    }
      
    // If both hands are the same, give player their money back
    else{
      cout<<"*** IT IS A TIE! ***\n\n";
      cout << "Player bet $" << current_bet <<".00 on this game" << endl;
      cout<<"Player got $"<<current_bet<<".00 back. \n";
      player_bank_account += current_bet;                 //If its a tie they get their money back
      ties++;                                             //Incriment ties
    }

    //Print both hand values to see why
    cout<<"\nPlayer Hand Total: " << player_hand_value;
    cout<<"\nDealer Hand Total: " << dealer_hand_value << "\n\n";

    //Display bet amount of the current game
  }

  /*===========================================================================
  PRINT STATS FUNCTION:                        
  Decription: This function will print the number of wins/losses/ties and total 
              gains the player has earned throughout the night (could be negative). 
              Additionally, the players chip bank is printed.
  ===========================================================================*/
  void printStats() {
    cout << "Player Total Wins: " << wins << endl;                                 //Print out num of wins
    cout << "Player Total Losses: " << losses <<endl;                              //Print out num of losses
    cout << "Player Total Ties: " << ties <<endl;                                  //Print out num of ties
    cout << "Player Total Gains: $" <<  this->earnings << ".00 " << endl<<endl;   //Print gains
    cout << "\nPlayer has $" << check_bank() << ".00 left in chip bank." << endl;  //Print out player new bank 

  }

  /*===========================================================================
  PRINT CARD FUNCTION:                          
  Input:      Takes in a Card structure
  Decription: This function outputs the value and suit of a card using a switch
              statement. This function also prints the symbol representing the suit. 
  ===========================================================================*/
  void print_card(Card c) {
    switch (c.value) {
    case ACE:
      cout << "Ace";
      break;
    case JACK:
      cout << "Jack";
      break;
    case QUEEN:
      cout << "Queen";
      break;
    case KING:
      cout << "King";
      break;
    default:
      cout << c.value;
      break;
    }
    cout << " of ";
    switch (c.suit) {
    case HEARTS:
      cout << "hearts \u2665\n";
      break;
    case CLUBS:
      cout << "clubs \u2663\n";
      break;
    case SPADES:
      cout << "spades \u2660\n";
      break;
    case DIAMONDS:
      cout << "diamonds \u2666\n";
      break;
    }
  }

  /*===========================================================================
  START PRINT DEALER FUNCTION:                 
  Decription: This function prints the dealers intial hand to the screen.
              This means it will print the dealers hand with one card facing up and 
              the other card facing down.
  ===========================================================================*/
  void start_print_dealer() {
    cout << "\n+-------------------------------------------------------------------------------+\n";
    cout << "|                                DEALER'S HAND:                                 |\n";
    cout << "+-------------------------------------------------------------------------------+\n\n";
    print_card(this->dealerHand[0]);
    cout << "Face down card\n";
  }

  /*===========================================================================
  PRINT DEALER HAND  FUNCTION:                       
  Decription: Prints all Cards in the dealer hand vector out on the screen.
  ===========================================================================*/
  void print_dealer_hand() {
    for (int i = 0; i < dealerHand.size(); i++) {
      print_card(dealerHand[i]);
    }
  }

  /*===========================================================================
  PRINT PLAYER HAND FUNCTION:                       
  Decription: Prints all Cards in the player hand vector out on the screen.
  ===========================================================================*/
  void print_player_hand() {
    for (int i = 0; i < playerHand.size(); i++) {
      print_card(playerHand[i]);
    }
  }

  /*===========================================================================
  CREATE DECK FUNCTION:  
  Decription: Creates a Card value for each suit.
              Creates all of the heart suit values then club suit values ect...
              All cards are then stored in the deck variable. 
  ===========================================================================*/
  void create_deck() {
    // Pace to store the card
    int i = 0;
    // Assign suits for the cards
    for (int s = 0; s < NUM_SUITS; s++) {
      // Assigns values for the cards
      for (int v = 1; v <= NUM_RANKS; v++, i++) {
        deck[i].value = v;
        deck[i].suit = s;
      }
    }
  }

  /*===========================================================================
  SHUFFLE DECK FUNCTION:                      
  Description:This function "shuffles" the deck by randomly selecting 2 cards and 
              swapping them. This is done 520 times to ensure the deck has been randomized. 
  ===========================================================================*/
  void shuffle_deck() {
    srand(time(NULL));
    for (int i = 0; i < 520; i++) {
      int j = rand() % NUM_CARDS;
      int k = rand() % NUM_CARDS;

      Card temp = deck[j];
      deck[j] = deck[k];
      deck[k] = temp;
    }
  }

  /*===========================================================================
  DEAL SINGLE CARD FUNCTION:
  Decription: This function returns the next available card in the deck.
              The cards have been shuffled and are stored in an array of size 52.
              This function traverses that array, picks a card and returns it.
              After a card has been dealt a 0 is then placed in the 
              cards value to signify that this card has already been dealt.
  ===========================================================================*/
  Card deal_single_card()
  {
    Card tempCard;                           //Temporary card to hold the information of the selected card.
    for(int i=0; i<NUM_CARDS; i++)           //For loop that increments between 0 and 52
      {
        if(deck[i].value!=0)                 //If the current card slot value is not equal to zero
        {
          tempCard.value = deck[i].value;    //Assign the temp value to equal the value of the selected card in the deck
          tempCard.suit = deck[i].suit;     // Assign the temp suit to match the suit of the selected card
          deck[i].value=0;                  //Update the card in the deck (array) to zero so it can't be chosen again
          break;
        }
        
      }
    return tempCard;                          //Deal the card
  }


  /*===========================================================================
  PLACE BET FUNCTION:
  Decription: This function allows the user to place a bet within the game.
              If the user enters a bet less than five dollars they will be
              prompted to re-enter their bet. If they have less than five
              dollars in their bank account the game will end. Once the
              user enters a valid input for their bet amount and they
              have sufficient funds, their bet will be placed and deducted
              from their bank account.
  ===========================================================================*/
   void placeBet() {
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');    //Avoid reading unnecessary characters into new user input buffer
    cout << "\nHow much would you like to bet? ";                      //Ask the user for their bet amount
    cin >> current_bet;
     
      while(validateInt(current_bet) == false){                        //Loop while the input format is incorrect
      cout << "\nInvalid input! Please re-enter your bet: ";
		  cin.clear();                                                     //Clear the user input buffer
	  	cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');   //Ignores input up until the next new line happens
	  	cin >> current_bet;										                           //Allow user to re-enter their bet amount
      }
    
    // Check if bet amount is between 5 dollars and amount available in player bank account. (Inclusive)
    while (current_bet < minimum ||current_bet > this->player_bank_account) //current bet doesn't meet parameters
    {               
      
        cout << "\nEnter bet amount between $5.00 and $"<<this->player_bank_account<<".00: ";  //Ask the user to re-enter 
        cin >> current_bet;                                               //Update the current_bet to the new user input                                               
      
        while(validateInt(current_bet) == false)                         //Loop while the input format is invalid
        {                        
        cout << "\nInvalid input! Please re-enter your bet: ";
  		  cin.clear();                                                      //Clear the user input buffer
  	  	cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');    //Ignores input up until the next new line 
  	  	cin >> current_bet;										                            //Allow user to re-enter their bet amount
        } 
    }
    player_bank_account = player_bank_account - current_bet;             // Deduct the bet amount from players bank  
  }

  /*==============================================================================
  INPUT VALIDATION FUNCTION: 
  Input:       User input
  Description: This function checks to see if the user input is an integer
               or not. It will return true if the user input is recieved and
               has the data type of an integer, and false if not.
  ===============================================================================*/
  bool validateInt(int userInp){
    int check=0;                    //initialize variable of type integer
    if(!(userInp >> check)){       //compare the data types of the user input
      return false;               // and the test variable. Return false if they
    }                            // do not match.
    return true;                //Return true if the data types match
  }

  /*===========================================================================
  HIT OR STAY CLIENT FUNCTION:  
  Description:  This function displays the players current hand and value and 
                then asks if they want another card. If they respond, "hit" then return 1,
                else if they respond "stay" it returns false. This function also
                performs input validation. 
  ===========================================================================*/
  bool hit_or_stay_client() {
    string response; // To hold the users response
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  //Ignore unwanted characters in buffer so they aren't read into new input
    //Display hand and value for the player
    cout << "\n+-------------------------------------------------------------------------------+\n";
    cout << "|                          YOUR CURRENT HAND & VALUE:                           |\n";
    cout << "+-------------------------------------------------------------------------------+\n\n";
    print_player_hand();
    cout << "Hand Value: ";
    cout<<calculate_hand(playerHand)<<"\n";

    // Ask user if they want another card and validate input
    again:
    cout << "\n+-------------------------------------------------------------------------------+\n";
    cout << "|                                'HIT' OR 'STAY':                               |\n";
    cout << "+-------------------------------------------------------------------------------+\n";
     
    cout << "\nDo you want to 'hit' or 'stay'? ";
    cin >> response;
    
    // Use the transform function to make input uppercase
    transform(response.begin(), response.end(), response.begin(), ::toupper);

      //Hit, Stay Input validation
        if (response == "HIT") 
        {
          return 1;
        }
        else if (response == "STAY") 
        {
          return 0; // this is to break out of the while loop in main
        } 
        else 
        {
          cout << "\n\nWrong input. Try again.\n";
          goto again;
        }
  }



  /*===========================================================================
  HIT OR STAY DEALER FUNCTION:
  Description:  Displays the dealers current hand and value and then checks if they need 
                another card. According to blackjack rules, if the dealers hand is 
                less than 17 they are required to take another card. If dealers hand
                is less than 17 then return 1. 
                Else if dealer hand is more than 17, return 0.
  ===========================================================================*/
  bool hit_or_stay_dealer() 
  { 
    cout << "\n+-------------------------------------------------------------------------------+\n";
    cout << "|                             DEALER's HAND & VALUE:                            |\n";
    cout << "+-------------------------------------------------------------------------------+\n\n";
    print_dealer_hand();
    cout << "Hand Value: ";
    cout<<calculate_hand(dealerHand)<<"\n";
    dealer_hand_value = calculate_hand(dealerHand);
  
    //If dealer hand is less than 17 in hand value then return true
    if (dealer_hand_value < 17) {
      return 1;
    }
    else 
    {
      return 0; 
    }  
}

  /*===========================================================================
  CHECK BUST FUNCTION:
  Input:       Takes in an integer
  Description: This function will be used to check if a user has
               "busted" meaning they went over 21 and lost the game.
                If they busted, return true, else return false.
  ===========================================================================*/
  bool check_bust(int anyHandValue) {
    if (anyHandValue > 21) 
    { // If handValue is greater than one return true
      return true;        // Meaning the user busted
    } else {              // Else return false the user did not bust
      return false;
    }
  }

  /*===========================================================================
  ASK TO PLAY FUNCTION:
  Decription: This function asks the user if they would like to play again
              at the end of each game. This function also performs input validation. 
  ===========================================================================*/
  bool ask_to_play()
  {
    string temp;
    cout << "Do you want to play again? Y/N: ";
    
    bool valid= false;
    
    do{                                //Do while loop to check user input
      cin >> temp;
    if (temp == "y" || temp == "Y") {          //If the user types y or Y, then input is valid
      valid = true;        
      break;             //set valid to true (ends do while)
    } else if( temp == "n" || temp =="N") {    //If the user types n or N, the input is valid
      valid = true;
      break;                 //set valid to true (ends do while)
    }
      cout << "\nInvalid Answer! Enter Y or N: ";   //The user has not entered Y or N. Ask them to re-enter input 
      }while(valid==false); 

    if (temp == "y" || temp == "Y") {      //If the user enters Y, they would like to play the game again
      return true;
    } else {                              //The user enter N and does not want to play again
      return false;
    }
  }
  /*===========================================================================
  BUST BREAK FUNCTION:
  Decription: This function displays the player's hand and hand value.
              Also, checks if the hand is over 21 and tells them that they busted.
  ===========================================================================*/
  void bust_break()
  {
     cout << "\n+-------------------------------------------------------------------------------+\n";
      cout << "|                          YOUR CURRENT HAND & VALUE:                           |\n";
      cout << "+-------------------------------------------------------------------------------+\n\n";
      print_player_hand();
      cout << "Hand Value: ";
      cout<<calculate_hand(playerHand)<<"\n";
      if(calculate_hand(playerHand)>21)  
      {
        cout<< "Busted\n";
      }
  }
}; // End of class

