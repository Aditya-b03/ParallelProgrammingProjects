#include <omp.h> 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <time.h>

// clean up
// output correction
// indent and comment
// final check


int main(int argc, char *argv[]){

    // input variables
    int** mat;
    int N,K;

    int INVALID = 0; // Notes Validity of whole Sudoku
    
    //input handling
    FILE *input_fp = NULL;
    input_fp = fopen(argv[1] , "r");
    if(input_fp == NULL){
        printf("unable to open input file");
        exit(-1);
    }
    else{
        fscanf(input_fp ,"%d %d" , &K , &N);
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

    // creating/opening Out file
    FILE* MainOut_fp = fopen("OutMain.txt" , "w");
    if(MainOut_fp == NULL){
        printf("Error : Unable to open file");
        exit(1);
    }

    // timer starts
    clock_t begin = clock();
    int sqrt_N = sqrt(N);

    // OpenMp parallel region
    omp_set_num_threads(K);
    #pragma omp parallel
    {   


        int thread_no = omp_get_thread_num() + 1;

        // row check
        #pragma omp for
        for(int i = 0; i < N; i++){

            //works as a boolean array, keeps track of numbers already found
            int *valid = (int *)calloc(N , sizeof(int));

            //keeps track of validity of particular section 
            int invalid = 0;

            for(int j = 0 ; j < N; j++){
                
                if(valid[mat[i][j] - 1] == 1){
                    invalid = 1;
                    INVALID = 1;
                    break;
                }
            valid[mat[i][j] - 1] = 1;
            }
            
            // prints valid output for each row
            if(invalid == 0){
                fprintf(MainOut_fp, "Thread %d checks the row %d and is valid.\n", thread_no  , i + 1);
            }
            else{
                fprintf(MainOut_fp, "Thread %d checks the row %d and is invalid.\n", thread_no  , i + 1);
            }
            free(valid);
        }

        // column checker
        #pragma omp for
        for(int i = 0; i < N; i++){

            int *valid = (int *)calloc(N , sizeof(int));
            int invalid = 0;

            for(int j = 0 ; j < N; j++){
                if(valid[mat[j][i] - 1] == 1){
                    invalid = 1;
                    INVALID = 1;
                    break;
                }
                valid[mat[j][i] - 1] = 1;
            }
            
            // prints valid output for each column
            if(invalid == 0){
                fprintf(MainOut_fp, "Thread %d checks the column %d and is valid.\n", thread_no , i + 1);
            }
            else{
                fprintf(MainOut_fp, "Thread %d checks the column %d and is invalid.\n", thread_no  , i + 1);
            }
            free(valid);
        }

        #pragma omp for
        for(int grid_no = 0 ; grid_no < N ; grid_no++){
            
            int *valid = (int *)calloc(N , sizeof(int));
            int invalid = 0;

            //start indices for each grid
            int grid_row = (grid_no/(int)sqrt_N)*sqrt_N;
            int grid_col = (grid_no%(int)sqrt_N)*sqrt_N;

            for(int r = 0 + grid_row ; r < sqrt_N + grid_row ; r++){
                for(int c = 0  + grid_col; c < sqrt_N  + grid_col ; c++){
                    if(valid[mat[r][c] - 1] == 1){
                        invalid = 1;
                        INVALID = 1;
                        break;                        

                    }
                    valid[mat[r][c] - 1] = 1;
                }
            }

            // prints valid output for each grid
            if(invalid == 0){
                fprintf(MainOut_fp, "Thread %d checks the grid %d and is valid.\n" , thread_no , grid_no + 1);
            }
            else{
                fprintf(MainOut_fp, "Thread %d checks the grid %d and is invalid.\n" , thread_no, grid_no + 1);
            }
            free(valid);
        } 
    }

    //timer ends
    clock_t end = clock();

    //Sudoku validity Output
    if(INVALID == 1){
        fprintf(MainOut_fp, "Sudoku is invalid.\n");
    }
    else{
        fprintf(MainOut_fp, "Sudoku is valid.\n");
    }

    //Time output
    fprintf(MainOut_fp, "The time elapsed is %lf seconds", (double)(end - begin) / CLOCKS_PER_SEC);

    // freeing allocated memory
    for(int i = 0; i < N; i++){
        free(mat[i]);
    }

     return 0;
}
