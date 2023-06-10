Program estimates value o π using monte-carlo method
for INPUT management:
    Program takes INPUT FILE NAME as a command line argument argv[1]
    it takes N and K as input for input file
    N - number of points to generate and evaluate 
    K - number of threads to make
    INPUT FILE should contain N as first interger ,
    And space separated integer K as second integer
    (example : "100 10" where 100 = N & 10 = K)
for Running the Program:
    use a C++ compiler to compile the ".cpp" file (example : clang++ Assign1_Src_(Es21btch11003.cpp))
    Provide input file name as command line argument while excuting(example : ./a.out "INPUT_FILE_NAME")

Program uses multithreading to creates K threads.
Each thread is used to evaluate N/K points for each to be inside/outside a unit circle.
