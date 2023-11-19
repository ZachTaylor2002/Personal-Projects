/*
		Author: Zach Taylor
		Date: Mar/24th/2023
		 Gaussian Elimination Implementation in C++
		*/
#include <iostream>
using namespace std;
int main() {
	double divisor=0;
	double multiplier=0;
	const int NUM_ROW = 3;
	const int NUM_COL = 4;
	double aug_matrix[NUM_ROW][NUM_COL] = {{2,-3,1,-22},
	                                       {7,9,-3,14},
	                                       {6,7,2,91}};


	
	 for(int i=0;i<NUM_ROW;i++){  //Right becuase when couting divisor u get
		 divisor=aug_matrix[i][i];   // 2,9,2
		 for(int j=0;j<NUM_COL;j++){  
			 aug_matrix[i][j] /= divisor; 
		 }
		 for(int j=0;j<NUM_ROW;j++){ //For loop to iterate through j row
			 if(j!=i){                 //If j doesn;t equal i
				 multiplier = -aug_matrix[j][i]; 
				 for(int k=0;k<NUM_COL;k++){   
					 aug_matrix[j][k] = aug_matrix[j][k] + multiplier*aug_matrix[i][k];
				 }
			 }
			 
		 }
		 
		}
	for(int i = 0; i < NUM_ROW; i++)    //To print the matrix
	{
		for(int j = 0; j < NUM_COL; j++)
				cout << aug_matrix[i][j] << " ";
		cout << endl;
	}			
}
