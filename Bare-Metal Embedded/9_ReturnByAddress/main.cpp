#include "uart.h"


int * scores_setter(){

  int *ptr = new int[5];
	
	if(ptr == NULL){
	  printf("Not enough memory");
	}
	else{
	  for(int i=0;i<5;i++){
		  ptr[i] =0;
		}
	}
	
	/*Return address of allocated memory*/
	
	return ptr; 
}


int main(){
	
	  uart_init();
	
	 int *grade4_scores  = scores_setter();
	
	for(int i =0;i<5;i++){
	  printf("Enter score : ");
		scanf("%d",&grade4_scores[i]);
	}
	
	printf("The socre for grade4 are : ");
	for(int i=0;i<5;i++){
	   printf("%d , ",grade4_scores[i]);
	}
	
	


  delete[] grade4_scores;

	while(1){

	}
}

