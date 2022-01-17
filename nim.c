#include "nim.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int *new_board(int board_size)    //this function will create a new board
{
	int* memory = (int*) malloc(board_size * sizeof(int));

	//If memory not assigned, closing the application
	if(memory == NULL)
		exit(0);

	return memory;
}

struct node *mk_nim_hash(int max_hash, int board_size, int *start_board)     //This function should malloc an array of size max_hash struct nodes
{
	struct node* hash2 = (struct node*) malloc(max_hash * sizeof(struct node));

	if(hash2 == NULL)
		exit(0);

	for(int i=0; i<max_hash; i++){   /* It should initialize the
nodes by setting the values of each moves value to -1, move value to NULL, nimsum value to -1. */
        struct node* current = hash2 + i;

		current->nimsum = -1;
		current->board = hash2board(board_size, start_board, i);
		current->moves = -1;
		current->move = NULL;
	}

	return hash2;
}

void free_board(int *board)   /*This function should free a board array.*/
{
	free(board);
}

void free_nim_hash(int max_hash, struct node *hash2)   /*This function should free each move array and each board array in each element in the
nim_hash and finally free the nim_hash array itself.*/
{
	for(int i=0; i<max_hash; i++)
        {
		struct node* current = hash2 + i;

		free(current->move);
		free(current->board);
	}
	free(hash2);
}

int *board_from_argv(int board_size, char **argv)   /*This function should create (new_board) a board array and initialize it with the integer
equivalents (atoi) of the string array argv.*/
{
	int *new_board = (int *) malloc(board_size * sizeof(int));
	for(int i=0; i<board_size; i++){
		new_board[i] = atoi( argv[i] );
	}
	return new_board;
}

int *copy_board(int board_size, int *board)   /*This function should return a pointer to a new_board whose values are initialized with exactly
the same values as board.*/
{
	int *new_board = (int *) malloc(board_size * sizeof(int));
	for(int i=0; i<board_size; i++)
        {
		new_board[i] = board[i];
	}
	return new_board;
}

int game_over(int board_size, int *board)   //This function will return a value of 1 if the board contains exactly one match and 0 otherwise.
{
	int total = 0;
	for(int i=0; i<board_size; i++) total += board[i];

	return (total == 1) ? total : 0;
}

int compute_nimsum(int board_size, int *board)    /* The nimsum is defined as the
bitwise exclusive OR of all the integer values in board, unless there are no values in board
that are greater than 1.  In the latter case, the nimsum is the negation of the regular nimsum.*/
{
	int value = 0, counter = 0;
	for(int i=0; i<board_size; i++)
       {
		value ^= board[i];
       }
       int i=0;
       while(i<board_size)
       {
                if(board[i] >1){counter++;}
                i++;
       }
        
       if(value!=0)
       {
                value=1;
       }
       if(counter==0)
       {
                return !value;
       }
       else
       {
                return value;
       }

}

void join_graph(struct node *hash2, int hash, int board_size, int *start_board){
	struct node *current2 = (hash2 + hash);
	if(current2->moves != -1)
		return;

	int *current_board = current2->board;

	current2->nimsum = compute_nimsum(board_size, current_board);
	current2->moves = 0;
	current2->move = (struct move*) malloc(sizeof(struct move));

	for(int i=0; i<board_size; i++){
		int *updated_board = copy_board(board_size, current_board);
		struct move *possible_moves = current2->move;

		//Initializing current2->move for every possible move.
		while(updated_board[i]--){
			int index = current2->moves++;
			possible_moves = realloc(possible_moves, current2->moves * sizeof(struct move));

			struct move *current_move = (possible_moves + index);
			current_move->row = i;
			current_move->matches = current_board[i]-updated_board[i];
			current_move->hash = board2hash(board_size, start_board, updated_board);
		}
		current2->move = possible_moves;
	}

	int size = current2->moves;
	struct move *possible_moves = current2->move;

	//calling join_graph for every possible move.
	for(int i=0; i<size; i++){
        int new_hash = (possible_moves + i)->hash;
		join_graph(hash2, new_hash, board_size, start_board);
	}
}

