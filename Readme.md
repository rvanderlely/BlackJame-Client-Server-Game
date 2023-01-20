Program: Blackjack Game
Contributors: Rachel Vanderlely, Grant Long, Destiny Conley, and Cade Ruple
Date: December 7th, 2022
Description: Client/Server program used to faciliate the gameplay of the casino banking game Blackjack.
--------------------------------------------------------------------------------------------------------
Technologies: 
--------------------------------------------------------------------------------------------------------
C/C++, UNIX, Replit. 
--------------------------------------------------------------------------------------------------------
How to Run Program
--------------------------------------------------------------------------------------------------------
1. Type and execute the 'make clean' command to free the system of unused files.
2. Type 'make' to compile the server and client files. This will create two seperate
   server and client executable applications.
3. In the open console, type "./server" to run the server.
4. Open up a seperate console and type "./client 127.0.0.1" to run the client application.
5. Now that both applications are connected and running, the game can be started.

--------------------------------------------------------------------------------------------------------
Blackjack Program
--------------------------------------------------------------------------------------------------------
Blackjack is a casino banking game played with the standard 52 card deck consisting of
13 ranks in four different suits (clubs, diamonds, hearts, and spades). In this program, 
a client/server connection is used to facilitate the gameplay. The server acts as the dealer
and deals the cards to the client. The user interacts with the client interface to loop 
through the different functions, and the output or results of the game session is dependent on 
the decisions they make. 


--------------------------------------------------------------------------------------------------------
How to Play Blackjack
--------------------------------------------------------------------------------------------------------
Definitions:
Stay- The user does not want anymore cards
Hit-  The user wants to be dealt another card
Bust- The total value of the players cards amounts to over 21, game over

1. The user will enter the amount of money they would like to deposit in their bank account
   for the duration of the game.
2. After establishing their bank account, the user will enter the amount of money they
   would like to bet. 
3. The game will begin and the player will recieve two cards both face up. The dealer will
   receive one card face up, and one card face down.
4. If the value of the players cards amount to 21, the player will win 2x the amount of their
   bet and the round is over.
4b. If the value of the players card is not equal to 21, the player has an option to be dealt an
    additional card. This process will repeat until the player calls "Stay" to stop being dealt 
    cards, or the value of the players cards is greater than 21 and they bust.
5. After the player has been dealt their final cards, the dealer will flip over their face down 
   card. 
	a. If the dealers' hand is less than 17, they must deal themselves another card
	b. If the dealers' hand is equal to or greater than 17, they must stay.
6. After all cards have been dealt to the dealer and player, a winner must be decided.
	a. Player Wins: if the dealer busts, the player wins 2x their bet amount
	b. Player Wins: if the players' hand is higher than the dealers hand, but no greater
	   than 21, the player wins 2x their bet amount.
	c. Dealer Wins: if the dealers' hand is higher than the players hand, but no greater
         than 21, the dealer takes the players bet amount.

