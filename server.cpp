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
#include <sys/wait.h>
#include <unistd.h>
#include <vector>
using namespace std;

/*==================================================================
                   Function Prototypes
==================================================================*/
void dealer_game(int sockfd);

/*==================================================================
                        struct messenger
Contains information needed to be passed back and forth from the 
client and the server. 
====================================================================*/
struct messenger
{
    Card sendThisCard;                //Make a temp card struct to pass things into
    bool playerFlag = false;          // Send true server to notify when player wants more cards
    bool dealerFlag = false;          // Send true server to notify when dealer wants more cards
    bool play_again;                  // Send true server to play another game
};

/*==================================================================
                         Server Main Function:
===================================================================*/
int main(int argc, char **argv) {
  int listenfd, connfd;
  pid_t childpid;
  socklen_t clilen;
  struct sockaddr_in cliaddr, servaddr;

  listenfd = socket(AF_INET, SOCK_STREAM, 0);

  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  servaddr.sin_port = htons(SERV_PORT);

  bind(listenfd, (SA *)&servaddr, sizeof(servaddr));

  listen(listenfd, LISTENQ);
  for (;;) {
    clilen = sizeof(cliaddr);
    printf("server is running...\n");
    connfd = accept(listenfd, (SA *)&cliaddr, &clilen);
    fprintf(stderr, "accept client...\n");
    if ((childpid = fork()) == 0) 
    { /* child process */
      close(listenfd);  /* close listening socket */
      // printf("closed the childs listening socket...\n");
      // printf("Child is calling dealer game function...\n");
      dealer_game(connfd); /* process the request */
      exit(0);
    }
    close(connfd); /* parent closes connected socket */
    cout << "client has left..." << endl;
    wait(&connfd);
  }
}

/*==================================================================
                         dealer_game Function:
====================================================================*/
void dealer_game(int sockfd) 
{
  BlackJackGame sameGame;          // Make a game object
  ssize_t n;                       //n "how much" data was read or written

  playagain:

  //Clear hands and hand values before new game
  sameGame.playerHand.clear();
  sameGame.dealerHand.clear();
  sameGame.player_hand_value=0;
  sameGame.dealer_hand_value=0;

  //Check Winner
  sameGame.create_deck();
  //Shuffle Deal
  sameGame.shuffle_deck();
  struct messenger sendData;         //Make a send data struct

  //Give player their 2 cards
   for (int i = 0; i < 2;i++)
    {
        n = read(sockfd, &sendData, sizeof(sendData));            //Read the struct to put the new card in
        sendData.sendThisCard = sameGame.deal_single_card();      //Generate a new card
        sameGame.playerHand.push_back(sendData.sendThisCard);     //Save the card to the players hand 
        n = write(sockfd, &sendData, sizeof(sendData));           //Write the struct with card back to the client 
    }
  
   //Give dealer their 2 cards
   for (int i = 0; i < 2;i++)
   {
      n = read(sockfd, &sendData, sizeof(sendData));              //Read the struct to put the new card in
      sendData.sendThisCard = sameGame.deal_single_card();      //Generate a new card
      sameGame.dealerHand.push_back(sendData.sendThisCard);       //Save the card to the dealers hand
      n = write(sockfd, &sendData, sizeof(sendData));             //Write the struct with card back to the client 
    }

    //Print the initial hands dealer and player hand
    sameGame.start_print_dealer();
    sameGame.print_player_hand();

    //Get the intial info about the player(if he needs more cards) using playerFlag in send data struct
    n = read(sockfd, &sendData, sizeof(sendData));

    again:
  
    //If the player typed "hit" then enter here because the playerFlag will be true
    //If the player typed "stay" then skip this if statement
    if((n  > 0)  && (sendData.playerFlag == true))
    {
        sendData.sendThisCard = sameGame.deal_single_card();    //Generate a new card
        sameGame.playerHand.push_back(sendData.sendThisCard);     //Save the card to the players hand
        sendData.playerFlag = false;                              //set playerFalg back to false before sending struct back
        n= write(sockfd, &sendData, sizeof(sendData));            //write the struct containing card back to the client
        n = read(sockfd, &sendData, sizeof(sendData));            //Do a read to see if playerFlag was set back to true again
        if((n  > 0)  && (sendData.playerFlag == true))            //If playerFlag was read in as true, player wants another card
        {
            goto again;                                           //Go back to again to generate and send another card
        }
    }

    //Print the players final hand 
    cout << "\n+-------------------------------------------------------------------------------+\n";
    cout << "|                          PLAYER'S FINAL HAND & VALUE:                         |\n";
    cout << "+-------------------------------------------------------------------------------+\n\n";
    sameGame.print_player_hand();
    cout << "Hand Value: ";
    cout<<sameGame.calculate_hand(sameGame.playerHand) << endl;

    //Get the intial info about the dealer(if he needs more cards after first 2) using dealerFlag in send data struct
    n = read(sockfd, &sendData, sizeof(sendData));
    
    again2:
  
    //Enter if statement if dealerFlag is true, dealerFlad true when dealer hand less than 17
    if((n  > 0)  && (sendData.dealerFlag == true))
    {
        sendData.sendThisCard = sameGame.deal_single_card();           //Generate a new card
        sameGame.dealerHand.push_back(sendData.sendThisCard);            //Save the card to the players hand
        sendData.dealerFlag = false;                                     //set dealerFalg back to false before sending struct back
        n= write(sockfd, &sendData, sizeof(sendData));                   //write the struct containing card back to the client
        n = read(sockfd, &sendData, sizeof(sendData));                    //Read to see if dealerFlag was set back to true again
        if((n  > 0)  && (sendData.dealerFlag == true))                    //If dealerFlag read in true, dealer needs another card
        {
            goto again2;
        }
    }

    //print the players final hand 
    cout << "\n+-------------------------------------------------------------------------------+\n";
    cout << "|                          DEALER'S FINAL HAND & VALUE:                         |\n";
    cout << "+-------------------------------------------------------------------------------+\n\n";
    sameGame.print_dealer_hand();
    cout << "Hand Value: ";
    cout<<sameGame.calculate_hand(sameGame.dealerHand) << endl;

    //Read in if the user wants to play again
     n = read(sockfd, &sendData, sizeof(sendData));                       //Read to see if play_again flag is true 
     if (sendData.play_again==true)
     {
        goto playagain;
     }
}//End of the dealer function