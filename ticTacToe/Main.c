#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

//ENUMs
enum t_field_state {EMPTY, NOUGH, CROSS}; 
typedef enum t_field_state field_state;

enum t_game_state{MOVE,          //gra rozpoczęta, gracze wykonują ruchy
				  WON,           //gra skończona, wygrana jednego z graczy
				  DRAW};         //gra skończona, remis
typedef enum t_game_state game_state;

enum t_sign{SGN_NOUGH, SGN_CROSS};
typedef enum t_sign sign;


//FUNCTIONS declarations
field_state **initiate_game(const int board_size);
sign choose_sign();
int get_board_size();
void display_board(const field_state **board, const int board_size);
void move(field_state **board, const int board_size, const sign player_sign, const sign current_sign);
game_state check_board(const field_state **board, const int board_size);
sign change_player(sign current_player);
void computers_move(field_state **board, const int board_size, sign current_sign);
int is_winning_move(const field_state **board, const int board_size, const sign checked_sign, int *winnig_row, int *winnig_col);
int board_is_empty(const field_state **board, const int board_size);
void destroy_board(field_state** board, int board_size);


int main(void){
	int board_size;
	game_state state = MOVE;
	sign current_sign = SGN_CROSS;
	sign player_sign;
	field_state **board;
	printf("Welcome to TicTacToe game!\n\n");
	board_size = get_board_size();
	player_sign = choose_sign();
	board = initiate_game(board_size);
	display_board(board, board_size);
	while(state == MOVE){
		move(board, board_size, player_sign, current_sign);
		display_board(board, board_size);
		state = check_board(board, board_size);
		current_sign = change_player(current_sign);
	}
	if(state == WON){
		printf("\n%s won! \n", current_sign == player_sign ? "Computer" : "You");
	}
	else if(state == DRAW){
		printf("\nNoone won. \n");
	}
	destroy_board(board, board_size);
	getchar();
	getchar();
}

field_state **initiate_game(const int board_size){
	int row, col;
	field_state **board = (field_state**) malloc(sizeof(field_state*) * board_size);
	for(row = 0; row < board_size; row++){
		board[row] = (field_state*) malloc(sizeof(field_state) * board_size);
		for(col = 0; col < board_size; col++){
			board[row][col] = EMPTY;
		}
	}
	srand(time(NULL));
	return board;
}

int get_board_size(){
	int board_size;
	while(1){
		printf("Choose board size [3-9]: ");
		scanf_s("%d", &board_size);
		if(board_size < 3 || board_size > 9){
			printf("Incorrect number!\n");
			continue;
		}
		break;
	}
	return board_size;
}

sign choose_sign(){
	sign player_sign;
	int player_sign_num;
	do{
		printf("Choose sign: for CROSS press 1, for NOUGH press 2. CROSS always begins ");
		scanf_s("%d", &player_sign_num);
		if(player_sign_num == 1){
			player_sign = SGN_CROSS;
			printf("Your sign is CROSS\n");
		}
		else if(player_sign_num == 2){
			player_sign = SGN_NOUGH;
			printf("Your sign is NOUGH\n");
		}
		else{
			printf("Incorrect number\n");
		}
	}
	while(player_sign_num != 1 && player_sign_num != 2);
	getchar();
	return player_sign;
}

void display_board(const field_state **board, const int board_size){
	int row, col;	
	putchar('\n');
	for(row = 0; row < board_size; row++){
		printf(" %d", row+1);
	}
	putchar('\n');
	for(row = 0; row < board_size; row++){
		printf("%d", row+1);
		for(col = 0; col < board_size; col++){
			if(col > 0){
				printf("|");
			}
			switch(board[row][col]){
			case EMPTY:
				putchar(' ');
				break;
			case NOUGH:
				putchar('O');
				break;
			case CROSS:
				putchar('X');
				break;
			}
		}

		if(row < board_size-1){
			printf("\n ");
			for(col = 0; col < board_size*2-1; col++){
				putchar('-');
			}
			putchar('\n');
		}
	}
	putchar('\n');
}

void move(field_state **board, const int board_size, const sign player_sign, const sign current_sign){
	int x, y;
	if(current_sign == player_sign){
		while(1){
			printf("\nYour turn \n");
			printf("Choose row: ");
			scanf_s("%d", &x);
			printf("Choose column: ");
			scanf_s("%d", &y);
			if(x > board_size || y > board_size || x < 1 || y < 1){
				printf("Move out ot board!\n");
				continue;
			}
			if(board[x-1][y-1] != EMPTY){
				printf("This fileld is not empty!\n");
				continue;
			}
			break;
		}
		board[x-1][y-1] = (player_sign == SGN_CROSS) ? CROSS :  NOUGH;
	}
	else{
		printf("\nComputer's turn");
		computers_move(board, board_size, current_sign);	
	}
}

game_state check_board(const field_state **board, const int board_size){
	//horizontal
	int row, col;
	game_state state = MOVE;
	int ctr = 0;
	for(row = 0; row < board_size; row++){
		ctr = 0;
		for(col = 0; col < board_size-1; col++){
			if(board[row][col] != EMPTY){
				if(board[row][col] == board[row][col+1]){
					ctr++;
				}
			}
		}
		if(ctr == board_size-1){
			return WON;
		}
	}
	//vertical
	for(col = 0; col < board_size; col++){
		ctr = 0;
		for(row = 0; row < board_size-1; row++){
			if(board[row][col] != EMPTY){
				if(board[row][col] == board[row+1][col]){
					ctr++;
				}
			}
		}
		if(ctr == board_size-1){
			return WON;
		}
	}
	//diagonal left to right
	ctr = 0;
	for(row = 0; row < board_size-1; row++){
		if(board[row][row] != EMPTY){
			if(board[row][row] == board[row+1][row+1]){
				ctr++;
			}
		}
	}
	if(ctr == board_size-1){
		return WON;
	}
	//diagonal right to left
	ctr = 0;
	for(row = 0; row < board_size-1; row++){
		if(board[row][board_size-1-row] != EMPTY){
			if(board[row][board_size-1-row] == board[row+1][board_size-row-2]){
				ctr++;
			}
		}
	}
	if(ctr == board_size-1){
		return WON;
	}

	ctr = 0;
	for(row = 0; row < board_size; row++){
		for(col = 0; col < board_size; col++){
			if(board[row][col] == EMPTY)
				return MOVE;
		}
	}
	return DRAW;
}

sign change_player(sign current_player){
	return current_player == SGN_CROSS ? SGN_NOUGH : SGN_CROSS;
}

int board_is_empty(const field_state **board, const int board_size){
	int row, col;
	for(row = 0; row < board_size; row++){
		for(col = 0; col < board_size; col++){
			if(board[row][col] != EMPTY){
				return 0;
				}
			}
		}
	return 1;
}

void computers_move(field_state **board, const int board_size, const sign current_sign){
	int row, col;
	int board_empty = board_is_empty(board, board_size);
	field_state current_state = (current_sign == SGN_CROSS) ? CROSS : NOUGH;
	if(board_empty == 1){
		int chosen_col, chosen_row;
		chosen_col = rand() % board_size;
		chosen_row = rand() % board_size;
		board[chosen_row][chosen_col] = current_state;
		return;
	}
	else{
		sign player_sign = change_player(current_sign);
		int winning;
		winning = is_winning_move(board, board_size, current_sign, &row, &col);
		if(winning == 1){
			board[row][col] = current_state; 
		}
		else{
			int to_block = is_winning_move(board, board_size, player_sign, &row, &col);
			if(to_block == 1){
				board[row][col] = current_state;
			}
			else{
				do{
					row = rand() % board_size;
					col = rand() % board_size;
				}
				while(board[row][col] != EMPTY);
				board[row][col] = current_state;
			}
		}
	}
	printf("\nComputer put sign on field %d, %d \n", row+1, col+1);
}
int is_winning_move(const field_state **board, const int board_size, const sign checked_sign, int *winning_row, int *winning_col){
	int row, col;
	int ctr = 0;
	int empty_index = -1;
	field_state checked_state = (checked_sign == SGN_CROSS) ? CROSS : NOUGH;
	//horizontal
	for(row = 0; row < board_size; row++){
		empty_index = -1;
		ctr = 0;
		for(col = 0; col < board_size; col++){
			if(board[row][col] == checked_state){
				ctr++;
			}
			if(board[row][col] == EMPTY){
				empty_index = col;
			}
		}
		if(ctr == board_size-1 && empty_index >= 0){
			*winning_row = row;
			*winning_col = empty_index;
			return 1;
		}
	}
	//vertical
	for(col = 0; col < board_size; col++){
		empty_index = -1;
		ctr = 0;
		for(row = 0; row < board_size; row++){
			if(board[row][col] == checked_state){
				ctr++;
			}
			if(board[row][col] == EMPTY){
				empty_index = row;
			}
		}
		if(ctr == board_size-1 && empty_index >= 0){
			*winning_row = empty_index;
			*winning_col = col;
			return 1;
		}
	}
	//diagonal left to right
	empty_index = -1;
	ctr = 0;
	for(col = 0; col < board_size; col++){
		if(board[col][col] == checked_state){
			ctr++;
		}
		if(board[col][col] == EMPTY){
				empty_index = col;
			}
		if(ctr == board_size-1 && empty_index >= 0){
			*winning_row = empty_index;
			*winning_col = empty_index;
			return 1;
		}
	}
	//diagonal right to left
	empty_index = -1;
	ctr = 0;
	for(col = 0; col < board_size; col++){
		if(board[col][board_size-1-col] == checked_state){
			ctr++;
		}
		if(board[col][board_size-1-col] == EMPTY){
				empty_index = col;
			}
		if(ctr == board_size-1 && empty_index >= 0){
			*winning_row = empty_index;
			*winning_col = board_size-1-empty_index;
			return 1;
		}
	}
	return 0;
}

void destroy_board(field_state** board, int board_size){
	int row;
	for(row = 0; row < board_size; row++){
		free(board[row]);
	}
	free(board);
}
