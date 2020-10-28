/* 
	Tic-tac-toe using short look-ahead ordering
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
#include <time.h>

#define ARBITRARILY_LOW_VALUE -10000
#define ARBITRARILY_HIGH_VALUE 10000

int print_board(const char board[3][3]);

int is_legal(const char board[3][3], int row_choice, int col_choice);

int is_victorious(const char board[3][3], char player);

int is_draw(const char board[3][3]);

int player_choose(const char board[3][3], int *row_choice, int *col_choice);

int computer_choose(char board[3][3], int depth, int *row_choice, int *col_choice);

int heuristic_function(const char board[3][3]);

int num_of_three_rows(const char board[3][3], char player);

int num_of_two_rows(const char board[3][3], char player);

int num_of_one_rows(const char board[3][3], char player);

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
	int depth = 0;
	srand(time(NULL));

	while (1){
		printf("\n\n");
		print_board((const char (*)[3]) board);
		if (is_maximizer == 1){						
			printf("Computer's turn (x). Choose row and column: \n");
			computer_choose(board, depth, &row_choice, &col_choice);
			board[row_choice][col_choice] = 'x';			
			if (is_victorious((const char (*)[3]) board, 'x')){
				printf("\n\n");
				print_board((const char (*)[3]) board);
				printf("THE COMPUTER WON! \n");
				break;
			}		
			if (is_draw((const char (*)[3]) board)){
				printf("\n\n");
				print_board((const char (*)[3]) board);
				printf("IT'S A DRAW! \n");
				break;
			}	
			is_maximizer = 0;
			depth++;
		} else {
			player_choose((const char (*)[3]) board, &row_choice, &col_choice);
			board[row_choice][col_choice] = 'o';			
			if (is_victorious((const char (*)[3]) board, 'o')){
				printf("\n\n");
				print_board((const char (*)[3]) board);
				printf("YOU WON! \n");
				break;
			}
			if (is_draw((const char (*)[3]) board)){
				printf("\n\n");
				print_board((const char (*)[3]) board);
				printf("IT'S A DRAW! \n");
				break;
			}
			is_maximizer = 1;
			depth++;
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
int print_board(const char board[3][3]){
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
int is_legal(const char board[3][3], int row_choice, int col_choice){
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
int is_victorious(const char board[3][3], char player){
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
int is_draw(const char board[3][3]){
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
int player_choose(const char board[3][3], int *row_choice, int *col_choice){
	do {
		printf("Your turn (o). Choose row and column: \n"); 
		scanf("%d %d", row_choice, col_choice);	
		(*row_choice)--;
		(*col_choice)--;
		if (is_legal((const char (*)[3]) board, *row_choice, *col_choice)){
			return 0;
		} else {
			printf("Illegal move! Please choose again!\n");
		}
	} while (1);
}

/*
 * Function:  computer_choose 
 * --------------------
 * Run an AI routine to choose the best move for the computer 
 *    
 *  board: The board configuration   
 *  row_choice: Row index of the move (output)
 *  col_choice: Column index of the move (output)
 *
 *  returns: 0
 */
int computer_choose(char board[3][3], int depth, int *row_choice, int *col_choice,
	int player_last_row_choice, int player_last_col_choice){
	int i,j;	
	int best_value;
	int value;		
	int enemy_best_choice_row = -1;
	int enemy_best_choice_col = -1;
	int enemy_best_choice_val = ARBITRARILY_HIGH_VALUE;

	static int ordered_move_row[9];
	static int ordered_move_col[9];
	static int ordered_move_val[9];
	static int num_of_ordered_moves = 0;
	static int player_predicted_move_row = -1;
	static int player_predicted_move_col = -1;
	best_value = ARBITRARILY_LOW_VALUE;	

	if (num_of_ordered_moves == 0){
		/* Evaluate positions resulting from a 1-ply search*/
		for (i = 0; i < 3; i++){
			for (j = 0; j < 3; j++) {			
				if (is_legal(board, i, j) == 0) {
					continue;
				}		
				board[i][j] = 'x';		
				value = heuristic_function((const char (*)[3]) board);
				board[i][j] = '_';
				if (best_value < value){
					best_value = value;
					*row_choice = i;
					*col_choice = j;
				}
			}
		}			
		board[*row_choice][*col_choice] = 'x';
		/* Based on the best move found, find enemy's most possible move according to the heuristic */
		for (i = 0; i < 3; i++){
			for (j = 0; j < 3; j++) {			
				if (is_legal(board, i, j) == 0) {
					continue;
				}				
				board[i][j] = 'o';	
				value = heuristic_function((const char (*)[3]) board);
				board[i][j] = '_';
				if (enemy_best_choice_val > value){
					enemy_best_choice_val = value;
					enemy_best_choice_row = i;
					enemy_best_choice_col = j;
				}
			}
		}	
		board[enemy_best_choice_row][enemy_best_choice_val] = 'o';
		/* After the enemy has made his/her best move, we have to sort our own moves for the next search */
		for (i = 0; i < 3; i++){
			for (j = 0; j < 3; j++) {			
				if (is_legal(board, i, j) == 0) {
					continue;
				}				
				board[i][j] = 'o';	
				value = heuristic_function((const char (*)[3]) board);
				board[i][j] = '_';
				if (enemy_best_choice_val > value){
					enemy_best_choice_val = value;
					enemy_best_choice_row = i;
					enemy_best_choice_col = j;
				}
			}
		}	

		board[enemy_best_choice_row][enemy_best_choice_val] = '_';
		board[*row_choice][*col_choice] = '_';
	} else {

	}
}

int heuristic_function(const char board[3][3]){
	int c3, n2, c2, n1, c1;
	c3 = num_of_three_rows(board, 'x');
	n2 = num_of_two_rows(board, 'o');			
	c2 = num_of_two_rows(board, 'x');
	n1 = num_of_one_rows(board, 'o');
	c1 = num_of_one_rows(board, 'x');
	return c3 - n2 + c2 - n1 + c1;
}
int num_of_three_rows(const char board[3][3], char player){
	int i,j;
	int result = 0;
	int ok;
	/* Rows */
	for (i = 0; i < 3; i++){
		ok = 1;
		for (j = 0; j < 3; j++) {	
			if (board[i][j] != player){
				ok = 0;
				break;
			}
		}
		result += ok;
	}

	/* Columns */
	for (i = 0; i < 3; i++){
		ok = 1;
		for (j = 0; j < 3; j++) {	
			if (board[j][i] != player){
				ok = 0;
				break;
			}
		}
		result += ok;
	}

	/* Diagonals */
	ok = 1;
	for (i = 0; i < 3; i++){
		if (board[i][i] != player){
			ok = 0;
			break;
		}
	}
	result += ok;

	ok = 1;
	for (i = 0; i < 3; i++){
		if (board[i][2-i] != player){
			ok = 0;
			break;
		}
	}
	result += ok;

	return result;
}

int num_of_two_rows(const char board[3][3], char player){
	int i,j;
	int result = 0;
	int num_of_empty_spaces;
	int num_of_player_symbols;
	/* Row */	
	for (i = 0; i < 3; i++){		
		num_of_empty_spaces = 0;
		num_of_player_symbols = 0;
		for (j = 0; j < 3; j++) {	
			if (board[i][j] == player){
				num_of_player_symbols++;
			} 
			if (board[i][j] == '_'){
				num_of_empty_spaces++;
			} 
		}
		if ((num_of_empty_spaces == 1) && (num_of_player_symbols == 2)){
			result++;
		}
	}

	/* Column */
	for (i = 0; i < 3; i++){		
		num_of_empty_spaces = 0;
		num_of_player_symbols = 0;
		for (j = 0; j < 3; j++) {	
			if (board[j][i] == player){
				num_of_player_symbols++;
			} 
			if (board[j][i] == '_'){
				num_of_empty_spaces++;
			} 
		}
		if ((num_of_empty_spaces == 1) && (num_of_player_symbols == 2)){
			result++;
		}
	}

	/* Diagonals */
	num_of_empty_spaces = 0;
	num_of_player_symbols = 0;
	for (i = 0; i < 3; i++){
		if (board[i][i] == player){
				num_of_player_symbols++;
		} 
		if (board[i][i] == '_'){
			num_of_empty_spaces++;
		} 
	}
	if ((num_of_empty_spaces == 1) && (num_of_player_symbols == 2)){
		result++;
	}

	num_of_empty_spaces = 0;
	num_of_player_symbols = 0;
	for (i = 0; i < 3; i++){
		if (board[i][2-i] == player){
				num_of_player_symbols++;
		} 
		if (board[i][2-i] == '_'){
			num_of_empty_spaces++;
		} 
	}
	if ((num_of_empty_spaces == 1) && (num_of_player_symbols == 2)){
		result++;
	}
	return result;
}

int num_of_one_rows(const char board[3][3], char player){
	int i,j;
	int result = 0;
	int num_of_empty_spaces;
	int num_of_player_symbols;
	/* Row */	
	for (i = 0; i < 3; i++){		
		num_of_empty_spaces = 0;
		num_of_player_symbols = 0;
		for (j = 0; j < 3; j++) {	
			if (board[i][j] == player){
				num_of_player_symbols++;
			} 
			if (board[i][j] == '_'){
				num_of_empty_spaces++;
			} 
		}
		if ((num_of_empty_spaces == 2) && (num_of_player_symbols == 1)){
			result++;
		}
	}

	/* Column */
	for (i = 0; i < 3; i++){		
		num_of_empty_spaces = 0;
		num_of_player_symbols = 0;
		for (j = 0; j < 3; j++) {	
			if (board[j][i] == player){
				num_of_player_symbols++;
			} 
			if (board[j][i] == '_'){
				num_of_empty_spaces++;
			} 
		}
		if ((num_of_empty_spaces == 2) && (num_of_player_symbols == 1)){
			result++;
		}
	}

	/* Diagonals */
	num_of_empty_spaces = 0;
	num_of_player_symbols = 0;
	for (i = 0; i < 3; i++){
		if (board[i][i] == player){
				num_of_player_symbols++;
		} 
		if (board[i][i] == '_'){
			num_of_empty_spaces++;
		} 
	}
	if ((num_of_empty_spaces == 2) && (num_of_player_symbols == 1)){
		result++;
	}

	num_of_empty_spaces = 0;
	num_of_player_symbols = 0;
	for (i = 0; i < 3; i++){
		if (board[i][2-i] == player){
				num_of_player_symbols++;
		} 
		if (board[i][2-i] == '_'){
			num_of_empty_spaces++;
		} 
	}
	if ((num_of_empty_spaces == 2) && (num_of_player_symbols == 1)){
		result++;
	}
	return result;
}

