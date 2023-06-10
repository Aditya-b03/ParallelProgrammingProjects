#include <iostream>
#include <pthread.h>
#include <stdlib.h>
#include <chrono>
#include <cstdlib>
using namespace std;

// N - total no. of random points
// K - No. of the thread to create
int N , K;



typedef struct{
    double x;
    double y;
    bool inside; 
}point;

// data to be passed as the parameter with each thread
typedef struct{
    int thread_no;
    point *hits_in;
    int N_circle;
    int N_square;
}data_thread;

//checks if a point(x,y) is inside a unit circle 
bool in_circle(double x , double y){

    // if point is inside
    if((x*x + y*y) <= (1.0)){
        return 1;
    }
    //else
    return 0;

}

// returns estimate of π using monte carlo
double π_value(long inside , long total){
    return (4.0*inside/(double)total);
}

void *runner(void *thread_){

    data_thread *thread_data = (data_thread *)thread_;

    
    thread_data->N_circle = 0;  // no of points inside circle 
    thread_data->N_square = 0;  // no of points inside square

    
    for(int i = 0 ; i < N/K ; i++){
        // generate x,y
        double no1 = (double)rand();
        double no2 = (double)rand();

        //converting no1 & no2 to doubles between [-1 , 1]
        double _x = (no1/(RAND_MAX/2.0)) - 1;
        double _y = (no2/(RAND_MAX/2.0)) - 1;

        // Storing each generated point
        thread_data->hits_in[i].x = _x;
        thread_data->hits_in[i].y = _y;
        thread_data->N_square++;

        //checking if the point is inside circle or not
        if(in_circle(_x , _y)){
            thread_data->hits_in[i].inside = 1;
            thread_data->N_circle++;
        }
        else{
            thread_data->hits_in[i].inside = 0;
        }
    }
    
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

    // starting the clock
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();

    pthread_t tid[K]; /* the thread identifier */
    pthread_attr_t attr;  /* set of thread attributes */

    /* set the default attributes of the thread */ 
    pthread_attr_init(&attr);

    data_thread thread_data_arr[K];

    //seed
    srand(time(0));
    
    //creating K threads
    for(int i = 1 ; i <= K ; i++){

        // initializing and allocating memory to the elements of data_thread array
        thread_data_arr[i-1].thread_no = i;
        thread_data_arr[i-1].hits_in = (point *)calloc((N/K + 1) , sizeof(point));

        /* create the thread */
        pthread_create(&tid[i-1], &attr, runner, (void*)&thread_data_arr[i-1]);
       
    }

    long inside_circle = 0;
    long inside_square = 0;

    //joining the threads
    for(int i = 1 ; i <= K ; i++){
        /* wait for the thread to exit */ 
        pthread_join(tid[i-1],NULL);
        inside_circle += thread_data_arr[i-1].N_circle;
        inside_square += thread_data_arr[i-1].N_square;
    }

    // estimating π value
    double π = π_value(inside_circle , inside_square);
    
    //ending the cloak
    end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    
 

    

    //Creating/Opening MainOut file
    //
    FILE *MainOut_fp = NULL;
    MainOut_fp = fopen("OutMain.txt" , "w");
    if(MainOut_fp == NULL){
        printf("Error : Unable to open file");
        exit(1);
    }
    fprintf(MainOut_fp , "Time : %lf\n (in milliseconds) " , elapsed_seconds.count()*(1e3));
    fprintf(MainOut_fp , "\n");
    fprintf(MainOut_fp , "Value estimated : %lf\n" , π);
    fprintf(MainOut_fp , "\n");
    fprintf(MainOut_fp , "Log :");
    // iterating for each thread
    for(int i = 0 ; i < K ; i++){
        fprintf(MainOut_fp , "\n");
        fprintf(MainOut_fp , "Thread %d : Number of points inside the square : %d , Number of points inside the circle : %d\n" , i+1, thread_data_arr[i].N_square , thread_data_arr[i].N_circle);
        fprintf(MainOut_fp , "Point inside the Square : ");
        for(int j = 0 ; j < N/K ; j++){
            fprintf(MainOut_fp , " (%lf , %lf) " , thread_data_arr[i].hits_in[j].x ,thread_data_arr[i].hits_in[j].y);
        }
        fprintf(MainOut_fp , "\nPoint inside the Circle : ");
        for(int j = 0 ; j < N/K ; j++){
            if(thread_data_arr[i].hits_in[j].inside){
                fprintf(MainOut_fp , " (%lf , %lf) " , thread_data_arr[i].hits_in[j].x ,thread_data_arr[i].hits_in[j].y);
            }
        }
        fprintf(MainOut_fp , "\n");
    }

    fclose(MainOut_fp);
    //
    // output ends
    
    // freeing the allocated memory to the program
    for(int thread = 0; thread < K ; thread++){
        free(thread_data_arr[thread].hits_in);
    }

}