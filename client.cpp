/*********************************************************************
Names:       Rachel V., Cade R., Destiny C. and Grant L.
Date:        December 7th, 2022
Instructor:  Dr. Zhao
Class:       Network Programming
Purpose:     This program uses a server to implement the card game
             black jack.
*********************************************************************/
#include "blackjack.cpp"
#include "blackjack.h"
#include <iostream>
#include <unistd.h>
#include <vector>
#include <iomanip>
using namespace std;


/*==================================================================
                        Function Prototypes
//=================================================================*/
void player_game(FILE *fp, int sockfd);

/*==================================================================
                        struct messenger
Contains information needed to be passed back and forth from the 
client and the server. 
====================================================================*/
struct messenger
{
    Card cardRecieved;                //Make a temp card struct to pass things into
    bool playerFlag = false;          // Send true server to notify when player wants more cards
    bool dealerFlag = false;          // Send true server to notify when dealer wants more cards
    bool play_again;                  // Send true server to play another game
  };


/*==================================================================
                        Client (Player) Main:
This function creates a socket and connects to the server.
This function then calls the player game function.
//=================================================================*/
int main(int argc, char **argv) {
  
  int sockfd;
  struct sockaddr_in servaddr;

  // Check errors
  if (argc != 2) {
    cout << "usage: tcpcli <IPaddress>";
    exit(1);
  }

  // Make a socket and connect to server
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(SERV_PORT);
  inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
  connect(sockfd, (SA *)&servaddr, sizeof(servaddr));
  player_game(stdin, sockfd); /* do it all */
  exit(0);
}

/*==================================================================
                        player_game Function:
====================================================================*/
void player_game(FILE *fp, int sockfd) 
{
  BlackJackGame myGame;               // Make a game object
  struct messenger recieveData;       //Make a messenger struct to send/recieve data to server
  int n;                              //n "how much" data was read or written
  bool response;                      //Holds players y or n response for play again?
  // bool sufficentFunds;             //holds a 0 or 1. False when player has less than $5 left

  
  cout << "\n+-------------------------------------------------------------------------------+\n";
  cout << "|                             WELCOME TO BLACKJACK!                             |\n";
  cout << "|    'It is not about the cards you are dealt, but how you play the hand...'    |\n";
  cout << "+-------------------------------------------------------------------------------+\n";
  cout << "\nTerms to remember while playing:\n";
  cout << "'Bust' - Have a hand value over 21. You lose if you bust.\n";
  cout << "'Hit'  - Asks dealer for another card.\n";
  cout << "'Stay' - Tells dealer you do not want another card.\n";
  cout << "'Bet'  - Give money to the dealer to join the game.\n";
  cout << "\n+-------------------------------------------------------------------------------+\n";
  cout << "|                                PURCHASE CHIPS:                                |\n";
  cout << "+-------------------------------------------------------------------------------+\n\n";
  myGame.initialize_bank();            // Ask user how many chips they want to buy and print results

  //Come back here for each new game
  playagain:                         

  //Clear hands and hand values before new game
  myGame.playerHand.clear();
  myGame.dealerHand.clear();
  myGame.player_hand_value=0;
  myGame.dealer_hand_value=0;
  myGame.current_bet =0;

  cout << "\n+-------------------------------------------------------------------------------+\n";
  cout << "|                                 PLACE A BET:                                  |\n";
  cout << "+-------------------------------------------------------------------------------+\n";
  
  myGame.placeBet();                      // Get the players bet and remove from chip bank, return 0 if insuffiecnt funds left
    

  
  //Get 2 cards from the server for the player and print them
  for (int i = 0; i < 2;i++)
  {
      n = write(sockfd, &recieveData, sizeof(recieveData));     //Write the "empty" struct to have data loaded in
      n = read(sockfd, &recieveData, sizeof(recieveData));      //Read the struct containing the new card from server
      myGame.playerHand.push_back(recieveData.cardRecieved);    //Add card to players hand
  }
  //Save the players hand value after dealt 2 cards
  myGame.player_hand_value = myGame.calculate_hand(myGame.playerHand);


  
  //Get 2 cards from the dealer for the dealer and print (one face down)
  for (int i = 0; i < 2;i++)
  {
      n = write(sockfd, &recieveData, sizeof(recieveData));     //Write the "empty" struct to have data loaded in
      n = read(sockfd, &recieveData, sizeof(recieveData));      //Read the struct containing the new card from server
      myGame.dealerHand.push_back(recieveData.cardRecieved);    //Add card to dealers hand
  }
  //Save the dealers initial hand value
  myGame.dealer_hand_value = myGame.calculate_hand(myGame.dealerHand);
  
  //Print the initial dealer hand to screen, player hand will be printed in hit or stay function right after this
  myGame.start_print_dealer();

  //Ask the user if they want another card
  while (myGame.hit_or_stay_client() == true)                //Returns true if they respond "hit"
    {
      recieveData.playerFlag = true;                           //set the playerFlag to 1 to indicate player wants card
      n = write(sockfd, &recieveData, sizeof(recieveData));    //Write struct to the server notifying that player want card
      n = read(sockfd, &recieveData, sizeof(recieveData));     //Read struct back from server with new card in it
      myGame.playerHand.push_back(recieveData.cardRecieved);   //Add card to players hand
      myGame.player_hand_value = myGame.calculate_hand(myGame.playerHand);//Update the players hand value
      recieveData.playerFlag=false;                            //Set the playerFlag to false

        //If the player busts, break the loop
        if(myGame.check_bust(myGame.player_hand_value))
        {
          myGame.bust_break();
          break;
        }
    }
    //After while loop, send struct to server with false playerFlag indicating player does not need more cards
    n = write(sockfd, &recieveData, sizeof(recieveData));    

  //Check if the dealer needs another card
    while (myGame.hit_or_stay_dealer() == true)
    {
        recieveData.dealerFlag = true;                              //Returns true automatically if dealerHand < 17
        n = write(sockfd, &recieveData, sizeof(recieveData));       //Write struct to the server notifying that dealer need card
        n = read(sockfd, &recieveData, sizeof(recieveData));        //Read struct back from server with new card in it
        myGame.dealerHand.push_back(recieveData.cardRecieved);      //Add card to dealers hand
        myGame.dealer_hand_value = myGame.calculate_hand(myGame.dealerHand);//Update the dealers hand value
        recieveData.playerFlag=false;                                //Set the dealerFlag to false
    }

    //After while loop, send struct to server with false delaerFlag indicating dealer does not need more cards
    n = write(sockfd, &recieveData, sizeof(recieveData));

    // Check winner
    myGame.pickWinner();
    //Print game stats
    myGame.printStats();

    if(myGame.check_bank()>=myGame.minimum)                //Ensure player has at least 5 dollars before asking if they want to play again
    {
        response = myGame.ask_to_play();                            //Ask user if want to play again and save in response
        if (response == true)                                       //Set play_agian flag to true to tell server
        {                                                           
            recieveData.play_again = true;                            //Set play_agian flag to true to tell server want to play again
            n = write(sockfd, &recieveData, sizeof(recieveData));     //Write struct to the server notifying want to play again
            goto playagain;                                           //go to playagain to replay the game
        }
        else
        {
            cout << "+-------------------------------------------------------------------------------+\n";
            cout << "|                                GAME OVER:                                     |\n";
            cout << "+-------------------------------------------------------------------------------+\n\n";
            myGame.printStats(); 
            recieveData.play_again = false;                            //Set play_agian flag to false to tell server
            n = write(sockfd, &recieveData, sizeof(recieveData));      //Write struct to the server notifying that game over
        }
    }
    else
    {
        cout << "+-------------------------------------------------------------------------------+\n";
        cout << "|                                GAME OVER:                                     |\n";
        cout << "|                 This casino requires a $5.00 minimum.                         |\n";
        cout << "|               You do not have enough money left to play again.                |\n";
        cout << "|               Please leave the casino now. Have a great night!                |\n";
        cout << "+-------------------------------------------------------------------------------+\n\n";
        myGame.printStats();                                       //Print the stats
        recieveData.play_again = false;                            //Set play_agian flag to false to tell server that game is over
        n = write(sockfd, &recieveData, sizeof(recieveData));      //Send struct to server so they can read the play agian flag 
    }
  return;//End of the game
} // End of str_cli
