Program evaluates of Perfect number between 1 to N.
for INPUT management:
    Program takes INPUT FILE NAME as a command line argument argv[1]
    it takes N and K as input for input file
    N - upto which numbers to evaluate
    K - number of child process to make
    INPUT FILE should contain N as first interger ,
    And space separated integer K as second integer
    (example : "100 10" where 100 = N & 10 = K)
for Running the Program:
    use a C compiler to compile the ".C" file (example : clang Assign1Src-Es21btch11003.c)
    Provide input file name as command line argument while excuting(example : ./a.out "INPUT_FILE_NAME")

Program uses multithreading to creates K threads.
Each thread is used to evaluate a subset of values of number between 1 to N.
