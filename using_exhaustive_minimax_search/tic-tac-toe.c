/* 
	Tic-tac-toe using simple minimax algorithm
	Here we assume that the player is the maximizer and the computer is the minimizer
	Also the player always moves first	
	The evaluation function is:
	+) +10 if player wins
	+) -10 if computer wins
	+) 0 if draws

	To compile with gcc, use:
	gcc -ansi -pedantic -W -Wall -o tic-tac-toe  tic-tac-toe.c
	Then run:
	./tic-tac-toe
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARBITRARILY_LOW_VALUE -10000
#define ARBITRARILY_HIGH_VALUE 10000

int print_board(char board[3][3]);

int is_legal(char board[3][3], int row_choice, int col_choice);

int is_victorious(char board[3][3], char player);

int is_draw(char board[3][3]);

int player_choose(char board[3][3], int *row_choice, int *col_choice);

int computer_choose(char board[3][3], int *row_choice, int *col_choice);

int min_max(char board[3][3], int is_maximizer);

int main()
{
	char board[3][3] =
    {
        { '_', '_', '_'},
        { '_', '_', '_'},
        { '_', '_', '_'}
    };
	int is_maximizer = 1;
	int row_choice, col_choice;		
		
	while (1){
		printf("\n\n");
		print_board(board);
		if (is_maximizer){			
			player_choose(board, &row_choice, &col_choice);
			board[row_choice][col_choice] = 'x';			
			if (is_victorious(board, 'x')){
				printf("\n\n");
				print_board(board);
				printf("YOU WON! \n");
				break;
			}
			if (is_draw(board)){
				printf("\n\n");
				print_board(board);
				printf("IT'S A DRAW! \n");
				break;
			}
			is_maximizer = 0;
		} else {
			/* The computer */		
			printf("Computer's turn (o). Choose row and column: \n");
			computer_choose(board, &row_choice, &col_choice);
			board[row_choice][col_choice] = 'o';			
			if (is_victorious(board, 'o')){
				printf("\n\n");
				print_board(board);
				printf("THE COMPUTER WON! \n");
				break;
			}		
			if (is_draw(board)){
				printf("\n\n");
				print_board(board);
				printf("IT'S A DRAW! \n");
				break;
			}	
			is_maximizer = 1;
		}
	}
	
	return 0;
	
}

/*
 * Function:  print_board 
 * --------------------
 * Print the board
 *    
 *  board: The board configuration   
 * 
 *  returns: 0
 */
int print_board(char board[3][3]){
	printf("   1 2 3\n");
	printf("  ______\n");
	printf("1 |%c %c %c \n", board[0][0], board[0][1], board[0][2]);
	printf("2 |%c %c %c \n", board[1][0], board[1][1], board[1][2]);
	printf("3 |%c %c %c \n", board[2][0], board[2][1], board[2][2]);	
	return 0;
}

/*
 * Function:  is_legal 
 * --------------------
 * Check if the move is legal or not
 *    
 *  board: The board configuration   
 *  row_choice: Row index of the move
 *  col_choice: Column index of the move
 *
 *  returns: 1 if the move is legal and 0 otherwise
 */
int is_legal(char board[3][3], int row_choice, int col_choice){
	if ((row_choice < 0) || (row_choice >= 3) || (col_choice < 0) || (col_choice > 3)) {
		return 0;
	}
	if (board[row_choice][col_choice] == '_'){
		return 1;
	} else {
		return 0;
	}
}

/*
 * Function:  is_victorious 
 * --------------------
 * Check if the player is victorious or not
 *    
 *  board: The board configuration   
 *  player: The player ('x' or 'o') 
 *
 *  returns: 1 if the player is victorious and 0 otherwise
 */
int is_victorious(char board[3][3], char player){
	int i,j;
	
	/* Check rows */
	for (i = 0; i < 3; i++){
		if ((board[i][0] == player) && (board[i][1] == player) && (board[i][2] == player)) {
			return 1;
		}
	}
	
	/* Check columns */
	for (j = 0; j < 3; j++){
		if ((board[0][j] == player) && (board[1][j] == player) && (board[2][j] == player)) {
			return 1;
		}
	}
	
	/* Check the main diagonal */
	if ((board[0][0] == player) && (board[1][1] == player) && (board[2][2] == player)) {
		return 1;
	}
	
	/* Check the other diagonal */
	if ((board[0][2] == player) && (board[1][1] == player) && (board[2][0] == player)) {
		return 1;
	}
	
	return 0;
}

/*
 * Function:  is_draw 
 * --------------------
 * Check if the current is draw or not
 *    
 *  board: The board configuration   
 *
 *  returns: 1 if the game is draw and 0 otherwise
 */
int is_draw(char board[3][3]){
	int i,j, num_of_empty_pos;
	num_of_empty_pos = 0;
	for (i = 0; i < 3; i++){
		for (j = 0; j < 3; j++){
			if (board[i][j] == '_'){
				num_of_empty_pos++;
			}
		}
	}
	if (num_of_empty_pos == 0){
		return 1;
	} else {
		return 0;
	}
}

/*
 * Function:  player_choose 
 * --------------------
 * Ask player to enter the next move. Will run until the entered move is correct
 *    
 *  board: The board configuration   
 *  row_choice: Row index of the move (output)
 *  col_choice: Column index of the move (output)
 *
 *  returns: 1 if the game is draw and 0 otherwise
 */
int player_choose(char board[3][3], int *row_choice, int *col_choice){
	do {
		printf("Your turn (x). Choose row and column: \n"); 
		scanf("%d %d", row_choice, col_choice);	
		(*row_choice)--;
		(*col_choice)--;
		if (is_legal(board, *row_choice, *col_choice)){
			return 0;
		} else {
			printf("Illegal move! Please choose again!\n");
		}
	} while (1);
}

/*
 * Function:  computer_choose 
 * --------------------
 * Run an AI routine  to choose the best move for the computer
 * Here we use the minimax algorithm
 *    
 *  board: The board configuration   
 *  row_choice: Row index of the move (output)
 *  col_choice: Column index of the move (output)
 *
 *  returns: 0
 */
int computer_choose(char board[3][3], int *row_choice, int *col_choice){
	int i,j;	
	int best_value;
	int value;	
	int row, col;
	
	best_value = ARBITRARILY_HIGH_VALUE;	
	for (i = 0; i < 3; i++){
		for (j = 0; j < 3; j++) {			
			if (is_legal(board, i, j) == 0) {
				continue;
			}			
			board[i][j] = 'o';
			value = min_max(board, 1);			
			board[i][j] = '_';			
			if (value < best_value) {
				best_value = value;
				row = i;
				col = j;
			}
		}
	}	
	*row_choice = row;
	*col_choice = col;
	return 0;
}

/*
 * Function:  min_max 
 * --------------------
 * The workhorse of the AI
 * Returns the best score possible given the current board and player
 *    
 *  board: The board configuration   
 *  is_maximizer: Whether the current player is the maximizer (as opposed to the minimizer)  
 *
 *  returns: The best possible score
 */
int min_max(char board[3][3], int is_maximizer){
	int i,j;	
	int best_value;
	int value;	
	int no_legal_move;
	
	if (is_victorious(board, 'o')){
		return -10;
	}
	if (is_victorious(board, 'x')){
		return 10;
	}
	if (is_draw(board)){
		return 0;
	}
	if (is_maximizer) {
		
		best_value = ARBITRARILY_LOW_VALUE;
		no_legal_move = 1;
		for (i = 0; i < 3; i++){
			for (j = 0; j < 3; j++) {
				if (is_legal(board, i, j) == 0) {
					continue;
				}
				no_legal_move = 0;
				board[i][j] = 'x';
				value = min_max(board, 0);				
				board[i][j] = '_';					
				if (value > best_value) {
					best_value = value;
				}
			}
		}
		if (no_legal_move == 0){
			return best_value;
		} else {
			return 0;
		}
	} else {		
		best_value = ARBITRARILY_HIGH_VALUE;
		no_legal_move = 1;
		for (i = 0; i < 3; i++){
			for (j = 0; j < 3; j++) {
				if (is_legal(board, i, j) == 0) {
					continue;
				}
				no_legal_move = 0;
				board[i][j] = 'o';
				value = min_max(board, 1);				
				board[i][j] = '_';				
				if (value < best_value) {
					best_value = value;
				}
			}
		}
		if (no_legal_move == 0){
			return best_value;
		} else {
			return 0;
		}
	}
}