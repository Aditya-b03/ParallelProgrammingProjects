Each Program simulates Critical Section Problem

for INPUT management:
        Programs take file input:
        File Name = "inp-params.txt"

        it takes n , k , lamda_1 , lamda_2 as input from input file respectively
        INPUT FILE should contain each as space separated values
        (example : "100 10 5 20" where 100 = K , 10 = N , 5 = lamda_1 & 20 = lamda_2)
        
        n - number of threads
        k - number of times each thread will enter CS
        lamda_1 , lamda_2 - used to derive delay values using exponantial distribution

for Running the Program:
        use a Cpp compiler to compile the ".cpp" file 
        (example : clang++ --std=c++0x -pthread Assign3-Src-<ProgName>-Es21btech11003.c )

        Program execution(example : ./a.out )


TAS lock - 
    Program uses test_and_set lock to solve Critical Section Problem
    & ensure mutual exclusion between competing threads


CAS lock -
    Program uses compare_and_swap lock to solve Critical Section Problem
    & ensure mutual exclusion between competing threads

CAS-Bounded -
    Program uses compare_and_swap lock to solve Critical Section Problem
    & ensure mutual exclusion between competing threads
    also uses updated algorithm to ensure bounded waiting 
    
OUTPUT management:
    Each program Outputs an Out-file "Outfile-<ProgName>.txt":
        it contains log for each thread :
            when each thread Request to enter CS 
            when each thread enters CS
            when each thread exits CS

