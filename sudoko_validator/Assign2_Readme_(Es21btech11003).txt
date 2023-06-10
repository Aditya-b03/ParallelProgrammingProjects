Each Programs validates a completed Sudoku

for INPUT management:
    		Program takes INPUT FILE NAME as a command line argument argv[1]
    		it takes N and K as input for input file
    		N - Size of sudoku 
    		K - number of threads to make
    		INPUT FILE should contain K as first integer ,
    		And space separated integer N as second integer
    		(example : "100 10" where 100 = K & 10 = N)
		then input file should contain N*N entries(space separated) for Sudoku
		example - for 4*4 , input can be			
				
				"K" 4
				1 2 3 4
				3 4 1 2
				4 1 2 3
				2 3 4 1


PTHREAD - 
	
	for Running the Program:
		use a C compiler to compile the ".c" file (example : clang Assign2Srcpthread(Es21btch11003).c )
    		Provide input file name as command line argument while executing(example : ./a.out "INPUT_FILE_NAME")

	Program uses multithreading using Pthreads to creates K threads.
	Each thread is used to evaluate certain Rows , columns and grids.


OpenMP -

	for Running the Program:
		use a C compiler to compile the ".c" file (example : gcc Assign2SrcOpenMP(Es21btch11003).c -lm -fopenmp)
    		Provide input file name as command line argument while executing(example : ./a.out "INPUT_FILE_NAME")

	Program uses multithreading using OpenMP to creates K threads.
	Each thread is used to evaluate certain Rows , columns and grids.


Each program Outputs an Out-file "OutMain.txt":
	it contains log for each thread - which all rows , columns or grids it evaluated
	also contains if the given sudoku is valid or not
	at last contains time taken for evaluation 





