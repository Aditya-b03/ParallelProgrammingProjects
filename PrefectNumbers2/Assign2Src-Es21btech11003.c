#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int N , K; //Input variables

/* Keeps thread data which needs
to be passed as parameter */
typedef struct{
    int thread_no;
    int *perfect_no;
}data_thread;

// return 1 if a number is perfect else 0
int check_Perfect(int n){

    // Store sum of factors
    int Factor_sum = 0;
    for(int i = 1 ; n >= i * i; i++){


        if(n%i == 0){
            Factor_sum += (n/i + i);

            // case for perfect square
            if(n/i == i){
                Factor_sum -= i;
            }

        }
        else continue;
    }

    // removing n itself as a factor from Factor_sum
    Factor_sum -= n;

    // prefect number
    if(Factor_sum == n){
        return 1;
    }
    // otherwise
    return 0;
}

void *runner(void *thread_){

    data_thread *thread_data = (data_thread *)thread_;
    printf("%d\n" , thread_data->thread_no);
    
    // file_handling for each child
    FILE *thread_fp = NULL;
    char File_name[20];
    sprintf(File_name , "OutFile%d.txt" , thread_data->thread_no);
    thread_fp = fopen(File_name, "w" );

    // j , i are used as iterators
    int j = thread_data->thread_no; // j incremented by K
    int i = 0;                      // i is index of perfect_no arr

    fprintf(thread_fp,"%dth thread :\n" , thread_data->thread_no);
    while(j <= N && i <= N/K + 1){

        // checking and keeping track of perfect numbers
        thread_data->perfect_no[i] = check_Perfect(j);

        // Printing in OutFiles(For each thread)
        if(thread_data->perfect_no[i] == 1){
            fprintf(thread_fp , "%d : Is a perfect Number\n" , j);
        }
        else{
            fprintf(thread_fp , "%d : Is not a perfect Number\n" , j);
        }
        j += K;
        i++;
    }
    fclose(thread_fp); // closing Outfiles
    pthread_exit(0);
}

int main(int argc , char *argv[]){
    
    FILE *input_fp = NULL;
    input_fp = fopen(argv[1] , "r");
    if(input_fp == NULL){
        printf("unable to open input file");
        exit(-1);
    }
    else{
        fscanf(input_fp ,"%d %d" , &N , &K);
    }


    pthread_t tid[K]; /* the thread identifier */
    pthread_attr_t attr;  /* set of thread attributes */

    /* set the default attributes of the thread */ 
    pthread_attr_init(&attr);

    data_thread thread_data_arr[K];
    
    //creating K threads
    for(int i = 1 ; i <= K ; i++ ){

        // initializing and allocating memory to the elements of data_thread array
        thread_data_arr[i-1].thread_no = i;
        thread_data_arr[i-1].perfect_no = calloc((N/K + 1) , sizeof(int));

        /* create the thread */
        pthread_create(&tid[i-1], &attr, runner, (void*)&thread_data_arr[i-1]);
        /* wait for the thread to exit */ 
        pthread_join(tid[i-1],NULL);

    }

    // Creating/Opening MainOut file
    FILE *MainOut_fp = NULL;
    MainOut_fp = fopen("OutMain.txt" , "w");
    if(MainOut_fp == NULL){
        printf("Error : Unable to open file");
        exit(1);
    }

   
    

    // Write Desired out in OutMain file 
    for(int thread = 0; thread < K ; thread++){

        // j , i are used as iterators
        int j = thread_data_arr[thread].thread_no;  // j varies with thread number 
        int i = 0;                                  // i is index of perfect_no arr

        fprintf(MainOut_fp , "Thread %d : " , thread + 1);

        while(j <= N && i <= N/K + 1){
            if(thread_data_arr[thread].perfect_no[i] == 1){
                // Printing perfect number in mainOut file
                fprintf(MainOut_fp, "%d " , j);
            }
            i++;
            j += K;
        }  
        fprintf(MainOut_fp , "\n");
    }
    fclose(MainOut_fp); // closing main file


    // freeing allocated memory
    for(int thread = 0; thread < K ; thread++){
        free(thread_data_arr[thread].perfect_no);
    }
}