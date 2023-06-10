#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <time.h>



// output correction
// final checks

//input variables
int N,K;
FILE *MainOut_fp;
int INVALID = 0; // Notes Validity of whole Sudoku
    



// data to be passed as the parameter with each thread
typedef struct{
    int thread_no;
    int **sudoku;  
}data_thread;




/*for each thread - 
checks for validity of specific row , column or grid*/
void *runner(void *thread_){


    data_thread *thread_data = (data_thread *)thread_;
    
    
    

    // i,j are iterators
    int j = thread_data->thread_no;
    int i;
    
    int sqrt_N = (int)sqrt(N);


   
    while(j <= 3*N){

        //works as a boolean array, keeps track of numbers already found 
        int *valid = (int *)calloc(N , sizeof(int));

        //keeps track of validity of particular section 
        int invalid = 0;
        
        //row checker
        if(j%3 == 1){

            
            for(i = 0; i < N; i++){
                if(valid[thread_data->sudoku[i][j/3] - 1] == 1){

                    INVALID = 1;
                    invalid = 1;
                    break;

                }
                valid[thread_data->sudoku[i][j/3] - 1] = 1;
            }

            // prints valid output for each row
            if(invalid == 0){
                fprintf(MainOut_fp, "Thread %d checks the row %d and is valid.\n" , thread_data->thread_no , j/3 + 1);
            }
            else{
                fprintf(MainOut_fp, "Thread %d checks the row %d and is invalid.\n" , thread_data->thread_no , j/3 + 1);

            }
        }

        //column checker
        else if(j%3 == 2){

            //printf("col %d\n\n" , j/3);
            for(i = 0; i < N; i++){
                if(valid[thread_data->sudoku[j/3][i] - 1 ] == 1){
                     
                     INVALID = 1;
                     invalid = 1;
                     break;

                }
                valid[thread_data->sudoku[j/3][i] - 1] = 1;
            }

            // prints valid output for each column
            if(invalid == 0){
                fprintf(MainOut_fp, "Thread %d checks the column %d and is valid.\n" , thread_data->thread_no , j/3 + 1);
            }
            else{
                fprintf(MainOut_fp, "Thread %d checks the column %d and is invalid.\n" , thread_data->thread_no , j/3 + 1);
            }
        }

        //grid checker
        else{

            
            int grid_no = j/3 - 1;

            //start indices for each grid
            int grid_row = (grid_no/(int)sqrt_N)*sqrt_N;
            int grid_col = (grid_no%(int)sqrt_N)*sqrt_N;


            for(int r = 0 + grid_row ; r < sqrt_N + grid_row ; r++){
                for(int c = 0  + grid_col; c < sqrt_N  + grid_col ; c++){
                    if(valid[thread_data->sudoku[r][c] - 1] == 1){

                        INVALID = 1;
                        invalid = 1;
                        break;

                    }
                    valid[thread_data->sudoku[r][c] - 1] = 1;
                }
            }

            // prints valid output for each grid
            if(invalid == 0){
                fprintf(MainOut_fp, "Thread %d checks the grid %d and is valid.\n" , thread_data->thread_no , grid_no + 1);
            }
            else{
                fprintf(MainOut_fp, "Thread %d checks the grid %d and is invalid.\n" , thread_data->thread_no , grid_no + 1);
            }
        } 

        free(valid);
        j += K;
    }

    pthread_exit(0);
}


int main(int argc , char *argv[]){

    
    int** mat;
    
    // input handling
    FILE *input_fp = NULL;

    input_fp = fopen(argv[1] , "r");
    if(input_fp == NULL){
        printf("unable to open input file");
        exit(-1);
    }
    else{

        fscanf(input_fp ,"%d %d" , &K , &N);

        //input for sudoku is taken in form of matrix
        mat = (int**)malloc(N * sizeof(int*));
        for(int i = 0; i < N; i++ )
        {
            mat[i] = (int*)malloc(N * sizeof(int));
        }
        for(int i = 0; i < N; i++){
            for(int j = 0; j < N; j++){
                fscanf(input_fp,"%d " , &mat[i][j]);
            }
        }

    }

    

    // Opening MainOut file
    MainOut_fp = fopen("OutMain.txt" , "w");
    if(MainOut_fp == NULL){
        printf("Error : Unable to open file");
        exit(1);
    }

    

    pthread_t tid[K];           /* the thread identifier */
    pthread_attr_t attr;        /* set of thread attributes */

    /* set the default attributes of the thread */ 
    pthread_attr_init(&attr);

    data_thread thread_data_arr[K];     //stores data for each thread

    // time reading starts
    clock_t begin = clock();
    
    //creating K threads
    for(int i = 1 ; i <= K ; i++){

        thread_data_arr[i-1].thread_no = i;
        thread_data_arr[i-1].sudoku = mat;
        
        /* create the thread */
        pthread_create(&tid[i-1], &attr, runner, (void*)&thread_data_arr[i-1]);
       
    }

  

    //joining the threads
    for(int i = 1 ; i <= K ; i++){
        /* wait for the thread to exit */ 
        pthread_join(tid[i-1],NULL);
       
    }

    // time reading ends
    clock_t end = clock();

    //Sudoku validity Output
    if(INVALID == 1){
        fprintf(MainOut_fp, "Sudoku is invalid.\n");
    }
    else{
        fprintf(MainOut_fp, "Sudoku is valid.\n");
    }

    //Time output
    //printf("%lf ", ((double)(end - begin) / CLOCKS_PER_SEC)*1000);
    fprintf(MainOut_fp, "The time elapsed is %lf seconds", (double)(end - begin) / CLOCKS_PER_SEC);


    fclose(MainOut_fp);
    
    // freeing allocated memory
    for(int i = 0; i < N; i++){
        free(mat[i]);
    }

}