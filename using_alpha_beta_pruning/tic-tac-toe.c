/* 
	Tic-tac-toe using alpha-beta pruning
	Here we assume that the player is the minimizer and the computer is the maximizer
	Also the computer always moves first ('X')	

	The heuristic evaluation function is:
		+) 1 when 'X' wins
		+) -1 when 'O' wins
		+) 0 in case of a draw

	Reference: 
		[1] Computer Gamesmanship: The Complete Guide to Creating 
		and Structuring intelligent game programs - David N.L.Levy

	To compile with gcc, use:
	gcc -ansi -pedantic -W -Wall -o tic-tac-toe  tic-tac-toe.c -pg
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

int alpha_beta_routine(char board[3][3], int alpha, int beta, int is_maximizer);

int main()
{
	char board[3][3] =
    {
        { '_', '_', '_'},
        { '_', '_', '_'},
        { '_', '_', '_'}
    };
	int is_maximizer = 1; /* The computer always moves first */
	int row_choice, col_choice;		
		
	while (1){
		printf("\n\n");
		print_board(board);
		if (is_maximizer == 1){	
			/* The computer */		
			printf("Computer's turn (x). Choose row and column: \n");
			computer_choose(board, &row_choice, &col_choice);
			board[row_choice][col_choice] = 'x';			
			if (is_victorious(board, 'x')){
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
			is_maximizer = 0;
			
		} else {
			player_choose(board, &row_choice, &col_choice);
			board[row_choice][col_choice] = 'o';			
			if (is_victorious(board, 'o')){
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
		printf("Your turn (o). Choose row and column: \n"); 
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
	best_value = ARBITRARILY_LOW_VALUE;		

	for (i = 0; i < 3; i++){
		for (j = 0; j < 3; j++) {			
			if (is_legal(board, i, j) == 0) {
				continue;
			}			
			board[i][j] = 'x';	
			value = alpha_beta_routine(board, ARBITRARILY_LOW_VALUE, ARBITRARILY_HIGH_VALUE, 0);	
			board[i][j] = '_';			
			if (value > best_value) {
				best_value = value;
				*row_choice = i;
				*col_choice = j;
			}
		}
	}
			
	return 0;
}

int alpha_beta_routine(char board[3][3], int alpha, int beta, int is_maximizer){
	int i,j;
	int value, temp;	
	if (is_victorious(board, 'x')){
		return 1;
	}
	if (is_victorious(board, 'o')){
		return -1;
	}
	if (is_draw(board)){
		return 0;
	}
	if (is_maximizer){	
		value = ARBITRARILY_LOW_VALUE;	
		for (i = 0; i < 3; i++){
			for (j = 0; j < 3; j++){
				if (is_legal(board, i, j) == 0) {
					continue;
				}			
				board[i][j] = 'x';
				temp = alpha_beta_routine(board, alpha, beta, 0);
				board[i][j] = '_';
				if (temp > value){
					value = temp;
				}				
				if (value > alpha){
					alpha = value;
				}				
				if (alpha >= beta){
					goto THE_END;
				}
			}
		}
		
	} else {
		value = ARBITRARILY_HIGH_VALUE;
		for (i = 0; i < 3; i++){
			for (j = 0; j < 3; j++){
				if (is_legal(board, i, j) == 0) {
					continue;
				}			
				board[i][j] = 'o';
				temp = alpha_beta_routine(board, alpha, beta, 1);
				board[i][j] = '_';
				if (temp < value){
					value = temp;
				}
				if (value < beta){
					beta = value;
				}
				if (alpha >= beta){
					goto THE_END;
				}
			}
		}		
	}
	THE_END: return value;
}

