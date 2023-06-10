#include <chrono>
#include <iostream>
#include <queue>
#include <random>
#include <semaphore.h>
#include <thread>
using namespace std;

// outfile pointer
FILE *thread_fp;

// Define the parameters
int P, C, k;
double lambdaP, lambdaC;
int Day_end = 0;

// Define the semaphores
sem_t mute;
sem_t passenger_sem;
sem_t car_sem;
sem_t *in_car;

// seed -- exponantial distribution
std::chrono::time_point<std::chrono::system_clock> start;

// Define the queue for passengers waiting for rides
queue<int> passenger_queue;

// Define the random number generator
default_random_engine generator;

// Define the Passenger thread function
void passenger_thread(int id) {

  // time variables
  std::chrono::time_point<std::chrono::system_clock> request, entry, ride, finish,exit;
  std::chrono::duration<double> elapsed_seconds;

  // Entry time
  entry = std::chrono::system_clock::now();
  elapsed_seconds = entry - start;
  fprintf(thread_fp, "Passenger %d enter the Museum at %lf s\n", id,elapsed_seconds.count());
  for (int i = 0; i < k; i++) {

    
    // Wander around the museum
    double wait_time = exponential_distribution<double>(1 / lambdaP)(generator);
    this_thread::sleep_for(chrono::duration<double>(wait_time));

    // Request Time
    request = std::chrono::system_clock::now();
    elapsed_seconds = request - start;
    fprintf(thread_fp, "Passenger %d request a ride at %lf s\n", id,elapsed_seconds.count());

    
    sem_wait(&mute);
    // Adding the Passenger request to queue
    passenger_queue.push(id);
    sem_post(&mute);

    //acquiring a car if available , else waiting
    sem_wait(&car_sem);
    //Signaling car thread about passenger request
    sem_post(&passenger_sem);

    
    // Ride start time
    ride = std::chrono::system_clock::now();
    elapsed_seconds = ride - start;
    fprintf(thread_fp, "passenger %d starts riding at %lf s\n", id, elapsed_seconds.count());

    // waitng for passenger to exit the car
    sem_wait(&in_car[id - 1]);

    // Finish time
    finish = std::chrono::system_clock::now();
    elapsed_seconds = finish - start;
    fprintf(thread_fp, "passenger %d finishes riding at %lf s\n", id,elapsed_seconds.count());

    // freeing the acquired car 
    sem_post(&car_sem);
    
  }

  
  // Exit Time
  exit = std::chrono::system_clock::now();
  elapsed_seconds = exit - start;
  fprintf(thread_fp, "--------passenger %d exits the Museum at %lf s------ \n",id, elapsed_seconds.count());

  return;
  
}

void car_thread(int id) {

  // time variables
  std::chrono::time_point<std::chrono::system_clock> ride, finish;
  std::chrono::duration<double> elapsed_seconds;

  while (true) {

    //waiting for passenger request
    sem_wait(&passenger_sem);
    
    if (Day_end == 1) {  
        return;
    }
    
    sem_wait(&mute);
    // handling Passenger request in front of Queue
    if ((passenger_queue.empty() || passenger_queue.front() == 0) && Day_end == 0) {
        sem_post(&mute);
        continue;
    }
    int passenger_id = passenger_queue.front();
    passenger_queue.pop();
    fprintf(thread_fp, "Car %d accepts passenger %d\n", id, passenger_id);
    sem_post(&mute);
    
    // Ride around the park
    double wait_time = exponential_distribution<double>(1 / lambdaC)(generator);
    this_thread::sleep_for(chrono::duration<double>(wait_time));


    //Finishing the tour
    fprintf(thread_fp, "Car %d finishes passenger %d's tour\n", id, passenger_id);

    //Signaling passenger thread with id = passnger_id to continue execution 
    sem_post(&in_car[passenger_id - 1]);
  }
}

int main() {

  // input handling
  FILE *input_fp = NULL;
  input_fp = fopen("inp-params.txt", "r");
  if (input_fp == NULL) {
    printf("unable to open input file");
    exit(-1);
  } else {
    fscanf(input_fp, "%d %d %lf %lf %d", &P, &C, &lambdaP, &lambdaC, &k);
    fclose(input_fp);
  }

  // Opening outfile
  thread_fp = fopen("Outfile.txt", "w");

  // starting the clock
  start = std::chrono::system_clock::now();

  // Initialize the semaphores
  sem_init(&mute, 0, 1);
  sem_init(&passenger_sem, 0, 0);
  sem_init(&car_sem, 0, C);
  in_car = (sem_t *)malloc(sizeof(sem_t) * P);
  for (int i = 0; i < P; i++) {
    sem_init(&in_car[i], 0, 0);
  }

  // Create the threads
  vector<thread> passenger_threads(P);
  vector<thread> car_threads(C);
  for (int i = 0; i < P; i++) {
    passenger_threads[i] = thread(passenger_thread, i + 1);
  }
  for (int i = 0; i < C; i++) {
    car_threads[i] = thread(car_thread, i + 1);
  }

  //Waiting for Passenger to finish all tours
  for (int i = 0; i < P; i++) {
    passenger_threads[i].join();
  }

  // terminating the threads
  Day_end = 1;    // Signal car threads - Day ended 
  for (int i = 0; i < C; i++) {
    sem_post(&passenger_sem);
  }

  //Waitng for car threads to terminate
  for (int i = 0; i < C; i++) {
    car_threads[i].join();
  }

  // Destroy the semaphores
  sem_destroy(&mute);
  sem_destroy(&passenger_sem);
  sem_destroy(&car_sem);
  for(int i = 0 ; i < P ; i++){
    sem_destroy(&in_car[i]);
  }

  return 0;
}
