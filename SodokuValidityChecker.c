#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

int board[9][9] = { {8, 3, 4, 6, 7, 8, 9, 1, 2}, {6, 7, 2, 1, 9, 5, 3, 4, 8}, {1, 9, 8, 3, 4, 2, 5, 6, 7}, {8, 5, 9, 7, 6, 1, 4, 2, 3}, {4, 2, 6, 8, 5, 3, 7, 9, 1}, {7, 1, 3, 9, 2, 4, 8, 5, 6}, {9, 6, 1, 5, 3, 7, 2, 8, 4}, {2, 8, 7, 4, 1, 9, 6, 3, 5}, {3, 4, 5, 2, 8, 6, 1, 7, 9} }; //Same as the valid board in the lab menu

int validityCount = 0; //Counter that should add up to 11 if the board is valid

typedef struct{ //Given from the lab menu
	int row;
	int column;
} parameters;


void *checkBoardColumns(void *structDataInput){ //Function to check the validity of the board's columns
	//Initialize variables
	int currentColumn, currentRow, startColumn, startRow;
	
	//Boolean variable set to true if there is redundancy in a column
	bool isColumnInvalid = false;
	
	//Retrieve data from the inputted struct and store them in local variables
	parameters *columnData = (parameters *) structDataInput;
	startColumn = columnData->column;
	startRow = columnData->row;
	
	//Iterate through every column
	for (currentColumn = startColumn; currentColumn < 9; currentColumn++){
		//Create an array where each zero's position starting from 1 correlates to the number of occurences of that number found in the current column
		int numberCount[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
		
		//Iterate through every row in a column
		for (currentRow = startRow; currentRow < 9; currentRow++){

			if (numberCount[board[currentRow][currentColumn] - 1] == 0){ //If there are zero repitions of the current number in the column
				numberCount[board[currentRow][currentColumn] - 1] = numberCount[board[currentRow][currentColumn] - 1] + 1; //Increment that number's value in the count array by 1
				 
			} else { //If there are repitions of the current number in the column
			
				isColumnInvalid = true; //Set the redudancy variable to true
				break; //Break out of the loop
			} 
			
		}
		
		if (isColumnInvalid == true){ //If the variable that keeps track of redundancy is true
			break; //Break out of the loop
		} else if (isColumnInvalid == false && currentColumn == 8){ //Else if every column has been iterated through and the variable that keeps track of redundancy is still false
			++validityCount; //Increment the global validityCount variable by 1
		} 
	}
}


void *checkBoardRows(void *structDataInput){
	//Initialize variables
	int currentColumn, currentRow, startColumn, startRow;
	
	//Boolean variable set to true if there is redundancy in a row
	bool isRowInvalid = false;
	
	//Retrieve data from the inputted struct and store them in local variables
	parameters *columnData = (parameters *) structDataInput;
	startColumn = columnData->column;
	startRow = columnData->row;
	
	//Iterate through every row
	for (currentRow = startRow; currentRow < 9; currentRow++){
		//Create an array where each zero's position starting from 1 correlates to the number of occurences of that number found in the current row
		int numberCount[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
		
		//Iterate through every column in a row
		for (currentColumn = startColumn; currentColumn < 9; currentColumn++){
			
			if (numberCount[board[currentRow][currentColumn] - 1] == 0){ //If there are zero repitions of the current number in the row
				numberCount[board[currentRow][currentColumn] - 1] = numberCount[board[currentRow][currentColumn] - 1] + 1; //Increment that number's value in the array by 1
				 
			} else { //If there are repitions of the current number in the row
			
				isRowInvalid = true; //Set the redudancy variable to true
				break; //Break out of the loop
			}
			
		}
		
		if (isRowInvalid == true){ //If the variable that keeps track of redundancy is true
			break; //Break out of the loop
		} else if (isRowInvalid == false && currentRow == 8){ //If every row has been iterated through and the variable that keeps track of redundancy is still false
			++validityCount; //Increment the global validityCount variable by 1
		}
	}
}


void *checkBoardSubGrid(void *structDataInput){
	//Initialize variables
	int currentColumn, currentRow, startColumn, startRow;
	
	//Boolean variable set to true if there is redundancy in the subgrid
	bool isSubGridInvalid = false;
	
	//Create an array where each zero's position starting from 1 correlates to the number of occurences of that number found in the current subgrid
	int numberCount[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
	
	//Retrieve data from the inputted struct and store them in local variables
	parameters *columnData = (parameters *) structDataInput;
	startColumn = columnData->column;
	startRow = columnData->row;
	
	//Iterate through every row in the subgrid
	for (currentRow = startRow; currentRow < startRow + 3; currentRow++){
		
		//Iterate through every column in a row in the subgrid
		for (currentColumn = startColumn; currentColumn < startColumn + 3; currentColumn++){

			if (numberCount[board[currentRow][currentColumn] - 1] == 0){ //If there are zero repitions of the current number in the subgrid
				numberCount[board[currentRow][currentColumn] - 1] = numberCount[board[currentRow][currentColumn] - 1] + 1; //Increment that number's value in the array by 1
				 
			} else { //If there are repitions of the current number in the subgrid
				isSubGridInvalid = true; //Set the redudancy variable to true
				break; //Break out of the loop
			}
			
		}
		
		if (isSubGridInvalid == true){ //If the variable that keeps track of redundancy is true
			break; //Break out of the loop
		} else if (isSubGridInvalid == false && (currentRow + 1)%3 == 0){ //If every number in the subgrid has been iterated through and the variable that keeps track of redundancy is still false			
			++validityCount; //Increment the global validityCount variable by 1
		}
	} 
}


int main(){
	
	//Initialize threads (11 in total)
	pthread_t columnThread, rowThread, subGridThread0, subGridThread1, subGridThread2, subGridThread3, subGridThread4, subGridThread5, subGridThread6, subGridThread7, subGridThread8;
	
	//--Create structs with data to pass onto threads--
	//Struct with data about the columns
	parameters *boardColumns = (parameters *) malloc(sizeof(parameters));
	boardColumns->row = 0;
	boardColumns->column = 0;
	
	//Struct with data about the rows
	parameters *boardRows = (parameters *) malloc(sizeof(parameters));
	boardRows->row = 0;
	boardRows->column = 0;
	
	//Struct with data about sub grid 0
	parameters *boardSubGrid0 = (parameters *) malloc(sizeof(parameters));
	boardSubGrid0->row = 0;
	boardSubGrid0->column = 0;
	
	//Struct with data about sub grid 1
	parameters *boardSubGrid1 = (parameters *) malloc(sizeof(parameters));
	boardSubGrid1->row = 0;
	boardSubGrid1->column = 3;
	
	//Struct with data about sub grid 2
	parameters *boardSubGrid2 = (parameters *) malloc(sizeof(parameters));
	boardSubGrid2->row = 0;
	boardSubGrid2->column = 6;

	//Struct with data about sub grid 3
	parameters *boardSubGrid3 = (parameters *) malloc(sizeof(parameters));
	boardSubGrid3->row = 3;
	boardSubGrid3->column = 0;

	//Struct with data about sub grid 4
	parameters *boardSubGrid4 = (parameters *) malloc(sizeof(parameters));
	boardSubGrid4->row = 3;
	boardSubGrid4->column = 3;

	//Struct with data about sub grid 5
	parameters *boardSubGrid5 = (parameters *) malloc(sizeof(parameters));
	boardSubGrid5->row = 3;
	boardSubGrid5->column = 6;

	//Struct with data about sub grid 6
	parameters *boardSubGrid6 = (parameters *) malloc(sizeof(parameters));
	boardSubGrid6->row = 6;
	boardSubGrid6->column = 0;
	
	//Struct with data about sub grid 7
	parameters *boardSubGrid7 = (parameters *) malloc(sizeof(parameters));
	boardSubGrid7->row = 6;
	boardSubGrid7->column = 3;

	//Struct with data about sub grid 8
	parameters *boardSubGrid8 = (parameters *) malloc(sizeof(parameters));
	boardSubGrid8->row = 6;
	boardSubGrid8->column = 6;
	
	
	//--Create threads to execute validity functions--
	//Thread to check board columns through function checkBoardColumns
	pthread_create(&columnThread, NULL, checkBoardColumns, boardColumns);

	//Thread to check board rows through function checkBoardRows
	pthread_create(&rowThread, NULL, checkBoardRows, boardRows);

	//Thread to check board subgrid 0 through function checkBoardSubGrid
	pthread_create(&subGridThread0, NULL, checkBoardSubGrid, boardSubGrid0);
	
	//Thread to check board subgrid 1 through function checkBoardSubGrid
	pthread_create(&subGridThread1, NULL, checkBoardSubGrid, boardSubGrid1);
	
	//Thread to check board subgrid 2 through function checkBoardSubGrid
	pthread_create(&subGridThread2, NULL, checkBoardSubGrid, boardSubGrid2);
	
	//Thread to check board subgrid 3 through function checkBoardSubGrid
	pthread_create(&subGridThread3, NULL, checkBoardSubGrid, boardSubGrid3);
	
	//Thread to check board subgrid 4 through function checkBoardSubGrid
	pthread_create(&subGridThread4, NULL, checkBoardSubGrid, boardSubGrid4);
	
	//Thread to check board subgrid 5 through function checkBoardSubGrid
	pthread_create(&subGridThread5, NULL, checkBoardSubGrid, boardSubGrid5);
	
	//Thread to check board subgrid 6 through function checkBoardSubGrid
	pthread_create(&subGridThread6, NULL, checkBoardSubGrid, boardSubGrid6);
	
	//Thread to check board subgrid 7 through function checkBoardSubGrid
	pthread_create(&subGridThread7, NULL, checkBoardSubGrid, boardSubGrid7);
	
	//Thread to check board subgrid 8 through function checkBoardSubGrid
	pthread_create(&subGridThread8, NULL, checkBoardSubGrid, boardSubGrid8);
	
	
	//--Join threads to wait for each thread's termination--
	pthread_join(columnThread, NULL);	
	pthread_join(rowThread, NULL);
	pthread_join(subGridThread0, NULL);
	pthread_join(subGridThread1, NULL);
	pthread_join(subGridThread2, NULL);
	pthread_join(subGridThread3, NULL);
	pthread_join(subGridThread4, NULL);
	pthread_join(subGridThread5, NULL);
	pthread_join(subGridThread6, NULL);
	pthread_join(subGridThread7, NULL);
	pthread_join(subGridThread8, NULL);
	
	
	//Check if the subgrid is valid by checking if the validityCount variable equals 11
	if (validityCount < 11) {
		printf("The suduko board is not valid. \n");
	} else {
		printf("The suduko board is valid. \n");
	}
	
	return 0;
}
