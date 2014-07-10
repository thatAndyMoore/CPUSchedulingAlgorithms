#include <stdio.h>
#include <stdlib.h>

typedef struct{
		int id;
		int priority;
		int arrivalTime;
		int cpuBurstDur;
} Process; // this struct will store each line from an input file in a way that the information is easy to handle

typedef int (*compfn)(const void*, const void*);

Process procArray[1000];
int numberOfProcesses;


//function prototypes
int comparePriority(Process *elem1, Process *elem2);
int compareBurst(Process *elem1, Process *elem2);
int compareArrival(Process *elem1, Process *elem2);
void prioritySch();
void firstComeFirstServe();
void shortestJobFirst();
void printProcessList();

float calculateAvgWaitTime();
int calculateTimeLastProcFinished();
float calculateAvgTurnaroundTime();
float calculateThroughput();

int main(){
	FILE *fp;
	char* input;
	ssize_t read; //number of characters getdelim() puts into the buffer
	size_t len = 0;
	
	//Process test;
	//test.id = 1;
	//test.priority = 4;
	//test.arrivalTime = 75;
	//test.cpuBurstDur = 310;
	//procArray[0].id = 7;
	//procArray[1].id = 56;
	
	
	//printf("The process id is: %d\n", procArray[0].id);
	//printf("The process id is: %d\n", procArray[1].id);
	
	
	if((fp = fopen("input.txt", "r")) == NULL)
	{
		printf("Could not open file");
		exit(EXIT_FAILURE);
	}
	
	/*
	while((read = getdelim(&input,&len,' ',fp)) != -1){
		printf("%s\n", input);
	}
	*/
	
	if ((read = getline(&input,&len,fp)) != -1){
		numberOfProcesses = atoi(input);
	}
	
	/* test to parse input delimited by space
	if ((read = getdelim(&input,&len,' ',fp)) != -1){
		testV = atoi(input);
	}
	
	if ((read = getdelim(&input,&len,' ',fp)) != -1){
		testV2 = atoi(input);
	}
	*/
	
	int i;
	for(i=0; i<numberOfProcesses; i++){
		if ((read = getdelim(&input,&len,' ',fp)) != -1){
			procArray[i].id = atoi(input);
		}
		
		if ((read = getdelim(&input,&len,' ',fp)) != -1){
			procArray[i].priority = atoi(input);
		}
		
		if ((read = getdelim(&input,&len,' ',fp)) != -1){
			procArray[i].arrivalTime = atoi(input);
		}
		
		if ((read = getdelim(&input,&len,'\n',fp)) != -1){
			procArray[i].cpuBurstDur = atoi(input);
		}
		
	}
	
	fclose(fp);
	
	prioritySch();
	printf("\n");
	firstComeFirstServe();
	printf("\n");
	shortestJobFirst();
	
	
	
	
	
	printf("Okay we compiled\n");
	
	return 0;
}

void firstComeFirstServe(){
	float avgWaitTime = 0;
	qsort((void *) &procArray, numberOfProcesses, sizeof(Process), (compfn)compareArrival);
	printf("--- First Come First Serve ---\n");
	printProcessList();
	
	avgWaitTime = calculateAvgWaitTime();
	printf("Average wait time: %f ms\n", avgWaitTime);
	printf("Time last process finished: %d ms\n", calculateTimeLastProcFinished());
	printf("Average turnaround time: %f ms\n", calculateAvgTurnaroundTime());
	printf("Throughput: %f procs / second\n", calculateThroughput());
	
}

void shortestJobFirst(){
	float WaitTime;
	qsort((void *) &procArray, numberOfProcesses, sizeof(Process), (compfn)compareBurst);
	printf("--- Shortest Job First ---\n");
	printProcessList();
	WaitTime = calculateAvgWaitTime();
	
	printf("Average Wait Time: %f ms\n", WaitTime);
	printf("Time last process finished: %d ms\n", calculateTimeLastProcFinished());
	printf("Average turnaround time: %f ms\n", calculateAvgTurnaroundTime());
	printf("Throughput: %f procs / second\n", calculateThroughput());
}

void prioritySch(){
	float avgWaitTime;
	
	qsort((void *) &procArray, numberOfProcesses, sizeof(Process), (compfn)comparePriority);
	printf("--- Priority ---\n");
	printProcessList();
	avgWaitTime = calculateAvgWaitTime();
	
	printf("Average wait time: %f ms\n", avgWaitTime);
	printf("Time last process finished: %d ms\n", calculateTimeLastProcFinished());
	printf("Average turnaround time: %f ms\n", calculateAvgTurnaroundTime());
	printf("Throughput: %f procs / second\n", calculateThroughput());
	
	
}

/*
 * Function to assist qsort in sorting struct array by element priority
 * Called in the prioritySch() function
 */
int comparePriority(Process *elem1, Process *elem2){
	if(elem1->priority < elem2->priority) return -1;
	else if (elem1->priority > elem2->priority) return 1;
	else return 0;
}

int compareBurst(Process *elem1, Process *elem2){
	if(elem1->cpuBurstDur < elem2->cpuBurstDur) return -1;
	else if (elem1->cpuBurstDur > elem2->cpuBurstDur) return 1;
	else return 0;
}

int compareArrival(Process *elem1, Process *elem2){
	if(elem1->arrivalTime < elem2->arrivalTime) return -1;
	else if (elem1->arrivalTime > elem2->arrivalTime) return 1;
	else return 0;
}

float calculateAvgWaitTime(){
	int timeOffset=0;
	float avgWaitTime = 0;
	int i;
	for(i=0;i<numberOfProcesses - 1; i++){
		
		//avgWaitTime += procArray[i].cpuBurstDur;
		//avgWaitTime += (avgWaitTime + procArray[i].cpuBurstDur);
		//avgWaitTime = avgWaitTime + procArray[i].cpuBurstDur;
		timeOffset += procArray[i].cpuBurstDur;
		avgWaitTime += timeOffset;
	}
	
	return avgWaitTime = avgWaitTime/numberOfProcesses;
	
}

int calculateTimeLastProcFinished(){
	int timeLastFinished = 0;
	
	int i;
	for(i=0;i<numberOfProcesses;i++){
		timeLastFinished += procArray[i].cpuBurstDur;
	}
	
	return timeLastFinished;
}

float calculateAvgTurnaroundTime(){
	float turnaroundTime = 0;
	int totalBurst = 0;
	
	int i;
	for(i=0; i < numberOfProcesses; i++){
		totalBurst += procArray[i].cpuBurstDur;
		turnaroundTime += totalBurst - (procArray[i].arrivalTime);
	}
	
	return turnaroundTime / numberOfProcesses;
}

float calculateThroughput(){
	float totalCpuTime = 0;
	float throughput = 0;
	
	int i;
	for(i = 0; i < numberOfProcesses; i++){
		totalCpuTime += procArray[i].cpuBurstDur;
	}
	
	throughput = numberOfProcesses / (totalCpuTime / 1000);
	
	return throughput;
}

void printProcessList(){
	printf("Number of processes: %d\n", numberOfProcesses);
	int j;
	for(j=0; j<numberOfProcesses; j++){
		printf("ID: %d	Priority: %d	Arival: %d	Burst:%d\n",procArray[j].id, procArray[j].priority, procArray[j].arrivalTime, procArray[j].cpuBurstDur);
	}
	
}
