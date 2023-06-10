#include <iostream>
#include <thread>
#include <atomic>
#include <iomanip>
#include <random>
#include <fstream>
#include <unistd.h>
using namespace std;

//Globals - input parameters
int n , k , lamda_1 , lamda_2 ;

//lock parameter
atomic_flag lock_ = ATOMIC_FLAG_INIT;

//outfile pointer
FILE *thread_fp;

//Average Waiting
double **individual_wait;

//seed -- exponantial distribution
std::chrono::time_point<std::chrono::system_clock> start, end;
std::random_device rd;
std::mt19937 gen(rd());
 
   
// - simulates Critical Section problem
void testCS(int Thread_id){

    
    // time variables
    std::chrono::time_point<std::chrono::system_clock> request,entry,exit;
    std::chrono::duration<double> elapsed_seconds , wait;
    
    for(int i = 0; i < k ; i++){
        
        // Request Time
        request = std::chrono::system_clock::now();
        elapsed_seconds = request - start; 
        fprintf(thread_fp,"%dth CS Requested at %.3lf ms by thread %d\n" ,i+1,elapsed_seconds.count()*1e3, Thread_id);
        
        //Spin lock 
        while(std::atomic_flag_test_and_set(&lock_));
        // spin until the lock is acquired
        
        //Entry time
        entry = std::chrono::system_clock::now();
        elapsed_seconds = entry - start;
        fprintf(thread_fp,"%dth CS Entered at %.3lf ms by thread %d\n" ,i+1,elapsed_seconds.count()*1e3, Thread_id);
        wait = entry - request;
        individual_wait[Thread_id - 1][i] = wait.count()*1e3;
        //simulates complex process in CS
        exponential_distribution<double> T1((double)1e3/lamda_1);
        sleep(T1(gen));
        
        // Unlock 
        lock_.clear();


        //Exit Time
        exit = std::chrono::system_clock::now();
        elapsed_seconds = exit - start; 
        fprintf(thread_fp,"%dth CS Exited at %.3lf ms by thread %d\n" ,i+1,elapsed_seconds.count()*1e3, Thread_id);

        //simulates RS
        exponential_distribution<double> T2((double)1e3/lamda_2);
        sleep(T2(gen));
             
    }
}

int main(){
    
  
    // starting the clock
    start = std::chrono::system_clock::now();

    

    //input handling
    FILE *input_fp = NULL;
    input_fp = fopen("inp-params.txt" , "r");
    if(input_fp == NULL){
        printf("unable to open input file");
        exit(-1);
    }
    else{
        fscanf(input_fp ,"%d %d %d %d" , &n , &k , &lamda_1 , &lamda_2);
        fclose(input_fp);
    }

    individual_wait = (double**)malloc(sizeof(double*)*n);
    for(int i = 0 ; i < n;i++){
        individual_wait[i] = (double*)malloc(sizeof(double)*k);
    }   


    // Opening outfile
    thread_fp = fopen("Outfile-TAS.txt" , "w");

    //Constructs thread array
    thread t1[n] ;
    
    //Call testCS for each thread
    for(int i = 0 ; i < n ; i++){
         t1[i] = thread(testCS , i+1);
    }

    // joining of threads
    for(int i = 0 ; i < n ; i++){
        t1[i].join();
    }


    double sum_wait = 0;
    double max_wait = 0;
    for(int i = 0; i < n; i++){
        for(int j = 0; j < k; j++){
            sum_wait += individual_wait[i][j];
            max_wait = max(max_wait , individual_wait[i][j]);      //maximum waiting time
        }
    }

    double Avg_wait = sum_wait/(n*k);   // Average waiting time
    



    fclose(thread_fp);
    for(int i = 0 ; i < n ; i++){
        free(individual_wait[i]);
    }
    

    

    return 0;
}

/*
The Time is measured in reference to “start” 
time variable whose value is initialized to 
the start time of execution.
*/