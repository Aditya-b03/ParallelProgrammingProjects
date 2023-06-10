#include <stdio.h> 
#include <sys/mman.h>
#include <stdlib.h> 
#include <unistd.h>
#include <fcntl.h> 
#include <sys/shm.h> 
#include <sys/stat.h>


// return 1 if a number is perfect else 0
int Perfect_no(int n){

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


int main(int argc , char *argv[]){

    int N , K;

    // input_handling
    FILE *input_fp = NULL;
    input_fp = fopen(argv[1] , "r");
    if(input_fp == NULL){
        printf("Error : Unable to open file");
        exit(1);
    }
    else{
        fscanf(input_fp , "%d %d" , &N , &K);
    }


   

    // initial process number
    int  pro_no= 1;

    // initializing pid_t
    pid_t pro_id = -1;
   
    
    while(pro_no <= K){

        // forking child processes
        if(pro_id != 0){
            pro_id = fork();
            
        }

        // processing (child process)
        if(pro_id == 0){

            // file_handling for each child
            FILE *child_fp = NULL;
            char File_name[20];
            sprintf(File_name , "OutFile%d.txt" , pro_no);
            child_fp = fopen(File_name, "w" );

            // Max required Size of each shared buffer
            int SIZE = sizeof(int)*(N/K + 1);

            // name of shared memory object
            char * name = "Sharing";

            /* shared memory file descriptor */
            int fd;

            /* pointer to shared memory obect */
            int *ptr;

            /* create the shared memory object */
            fd = shm_open(name,O_CREAT | O_RDWR,0666);

            /* configure the size of the shared memory object */ 
            ftruncate(fd, SIZE);

            /* memory map the shared memory object */
            ptr = (int *)mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
            if(ptr == MAP_FAILED){
                printf("Map Failed_1");
                exit(1);
            }


            fprintf(child_fp,"%dth process.... :\n" , pro_no);

                             // j , i are used as iterators
            int j = pro_no;  // j varies with process number 
            int i = 0;       // i is index of buffer array ptr

            while(j <= N && i <= N/K + 1){

                // here j is number to be checked
                ptr[i] = Perfect_no(j);

                // Printing in OutFiles(For each child process)
                if(ptr[i] == 1){
                    fprintf(child_fp , "%d : Is a perfect Number\n" , j);
                }
                else{
                    fprintf(child_fp , "%d : Is not a perfect Number\n" , j);
                }

                j += K;
                i++;

            }  

            fclose(child_fp); // closing Outfiles
            _exit(0);
        }
        else if(pro_id > 0){

            wait(NULL); // Continue Parent when child are complete
            
            // Similar initializations as in child process for shared memory
            char *name = "Sharing";
            int *ptr;
            int fd;
            int SIZE = sizeof(int)*(N/K + 1);

            // Creating/Opening MainOut file
            FILE *MainOut_fp = NULL;
            MainOut_fp = fopen("OutMain.txt" , "a");
            if(MainOut_fp == NULL){
                printf("Error : Unable to open file");
                exit(1);
            }

            /* open the shared memory object */ 
            fd = shm_open(name, O_RDONLY, 0666);
            
            /* memory map the shared memory object */
            ptr = (int *)mmap(0, SIZE, PROT_READ, MAP_SHARED, fd, 0);
            if(ptr == MAP_FAILED){
                printf("Map Failed_2\n");
                exit(1);
            }

            // j , i are used as iterators
            int j = pro_no;  // j varies with process number 
            int i = 0;       // i is index of buffer array ptr

            /* read from the shared memory object 
               Write Desired out in OutMain file */
            fprintf(MainOut_fp , "Process %d : " , pro_no);
            while(j <= N){
                if(ptr[i] == 1){
                    // Printing in OutFiles
                    fprintf(MainOut_fp, "%d " , j);
                }
                i++;
                j += K;
            }  
            fprintf(MainOut_fp , "\n");

            
            shm_unlink(name);   // Unlink shared memory map
            fclose(MainOut_fp); // closing main file
            
        }
        pro_no++;   //Update process number
    }

    fclose(input_fp); // closing input.txt

    return 0;
}
