#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "job.h"

void help() {
	printf("List of operation codes:\n");
	printf("\t'h' for help;\n");
	printf("\t'a' for adding a job to the scheduler;\n");
	printf("\t'p' for removing a job from the scheduler;\n");
	printf("\t'u' for searching jobs per user;\n");
	printf("\t'j' for searching jobs per capacity;\n");
	printf("\t'l' for listing all jobs;\n");
	printf("\t'q' for quit.\n");
}

void read(char *job_name, char *user_name, int *num_cpus, int *num_gpus, int *memory, double *time, int *priority) {
	if(job_name != NULL) {
		printf("Enter the name of the job: ");
		scanf("%s", job_name);
	}
	if(user_name != NULL) {
		printf("Enter the name of the user: ");
		scanf("%s", user_name);
	}
	if(num_cpus != NULL) {
		printf("Enter the number of CPUs: ");
		scanf("%d", num_cpus);
	}
	if(num_gpus != NULL) {
		printf("Enter the number of GPUs: ");
		scanf("%d", num_gpus);
	}
	if(memory != NULL) {
		printf("Enter the amount of memory: ");
		scanf("%d", memory);
	}
	if(time != NULL) {
		printf("Enter the amount of time: ");
		scanf("%lf", time);
	}
	if(priority != NULL) {
		printf("Enter the priority: ");
		scanf("%d", priority);
	}
}

struct job * add_job(struct job *scheduler, char *job_name, char *user_name, int num_cpus, int num_gpus, int memory, double time, int priority) {
	struct job *new_job;
    struct job *findNextPriority;
    struct job *higherPriorities;
	new_job = malloc(sizeof(struct job));
	if (new_job == NULL) {
		printf("Out of memory!\n");
		exit(1);
	}
	//Assign all the parameters to tmp
	strcpy(new_job->job_name, job_name);
	strcpy(new_job->user_name, user_name);
	new_job->num_cpus = num_cpus;
	new_job->num_gpus = num_gpus;
	new_job->memory = memory;
	new_job->time = time;
	new_job->priority = priority;
	new_job->next = NULL;

	//Check if the new_job has higher priority than the currect first item in the list or if scheduler is empty
    if (scheduler == NULL || new_job->priority > scheduler->priority) {
        new_job->next = scheduler;
		return new_job;
	} else {
		//Iterate through the linked list to find the proper priority spot for new_job
		findNextPriority = scheduler->next;
		higherPriorities = scheduler;
    	while (findNextPriority != NULL && new_job->priority <= findNextPriority->priority) {
    	    higherPriorities = higherPriorities->next;
        	findNextPriority = findNextPriority->next;
    	}
		//place new_job in its new home :)
    	higherPriorities->next = new_job;
    	new_job->next = findNextPriority;
	}
	return scheduler;


}

struct job * pop_job(struct job *scheduler) {
	if (scheduler == NULL) {
		return scheduler;
	}

	struct job *nextJob = scheduler;

	//Print next Job to be completed
	printf("|----------------------|----------------------|------|------|------|--------|----------|\n");
	printf("| Job name             | User name            | CPUs | GPUs | Mem. | Time   | Priority |\n");
	printf("|----------------------|----------------------|------|------|------|--------|----------|\n");
	printf("| %-20s | %-20s | %4d | %4d | %4d | %6.2f | %8d |\n", nextJob->job_name, nextJob->user_name, nextJob->num_cpus, nextJob->num_gpus, nextJob->memory, nextJob->time, nextJob->priority);
	printf("|----------------------|----------------------|------|------|------|--------|----------|\n");


	//remove the Job
	scheduler = nextJob->next;
	free(nextJob);
	return scheduler;

}

void list_user(struct job *scheduler, char *user_name) {
	struct job *userJob = scheduler;
	int hasAJob = 0;

	//iterate through userJob
	while (userJob != NULL) {
		//Check if user_name matches
		if (strcmp(userJob->user_name, user_name) == 0) {
			if (hasAJob == 0) {
				printf("|----------------------|----------------------|------|------|------|--------|----------|\n");
				printf("| Job name             | User name            | CPUs | GPUs | Mem. | Time   | Priority |\n");
				printf("|----------------------|----------------------|------|------|------|--------|----------|\n");
				hasAJob = 1;
			}
			printf("| %-20s | %-20s | %4d | %4d | %4d | %6.2f | %8d |\n", userJob->job_name, userJob->user_name, userJob->num_cpus, userJob->num_gpus, userJob->memory, userJob->time, userJob->priority);
			printf("|----------------------|----------------------|------|------|------|--------|----------|\n");
		}
		userJob = userJob->next;
	}

}

void list_jobs(struct job *scheduler, int num_cpus, int num_gpus, int memory, double time) {
	struct job *jobsCriteria;
	jobsCriteria = scheduler;
	int meetsCriteria = 0;

	//Iterate through jobCriteria
	while(jobsCriteria != NULL) {
		//Check if the job meets the criteria
		if (jobsCriteria->num_cpus <= num_cpus && jobsCriteria->num_gpus <= num_gpus && jobsCriteria->memory <= memory && jobsCriteria->time <= time) {
			if(meetsCriteria == 0) {
				printf("|----------------------|----------------------|------|------|------|--------|----------|\n");
				printf("| Job name             | User name            | CPUs | GPUs | Mem. | Time   | Priority |\n");
				printf("|----------------------|----------------------|------|------|------|--------|----------|\n");
				meetsCriteria = 1;
			}
			printf("| %-20s | %-20s | %4d | %4d | %4d | %6.2f | %8d |\n", jobsCriteria->job_name, jobsCriteria->user_name, jobsCriteria->num_cpus, jobsCriteria->num_gpus, jobsCriteria->memory, jobsCriteria->time, jobsCriteria->priority);
			printf("|----------------------|----------------------|------|------|------|--------|----------|\n");

		}
		jobsCriteria = jobsCriteria->next;
	}


}

void list_all_jobs(struct job *scheduler) {
	struct job *listJobs;
	listJobs = scheduler;
	//Does nothing if listJobs is empty
	if (listJobs == NULL) {
		return;
	}

	//prints all jobs on listJobs
	printf("|----------------------|----------------------|------|------|------|--------|----------|\n");
	printf("| Job name             | User name            | CPUs | GPUs | Mem. | Time   | Priority |\n");
	printf("|----------------------|----------------------|------|------|------|--------|----------|\n");
	while(listJobs != NULL) {
			printf("| %-20s | %-20s | %4d | %4d | %4d | %6.2f | %8d |\n", listJobs->job_name, listJobs->user_name, listJobs->num_cpus, listJobs->num_gpus, listJobs->memory, listJobs->time, listJobs->priority);
			printf("|----------------------|----------------------|------|------|------|--------|----------|\n");
			listJobs = listJobs->next;
	}


}

struct job * clear_jobs(struct job *scheduler) {
	struct job *deleteAllJobs;
	//Iterates through deleteAllJobs, removing them and assigning it to scheduler
	while(scheduler != NULL) {
		deleteAllJobs = scheduler;
		scheduler = deleteAllJobs->next;
		free(deleteAllJobs);
	}
	return scheduler;
}

