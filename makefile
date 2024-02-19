# Build job.o
job.o: job.c
	gcc -c job.c

# Build project10_scheduler.o
project10_scheduler.o: project10_scheduler.c
	gcc -c project10_scheduler.c

# Build project10_scheduler by linking job.o and project10_scheduler.o
project10_scheduler: job.o project10_scheduler.o
	gcc -o project10_scheduler job.o project10_scheduler.o

clean:
	rm job.o project10_scheduler.o project10_scheduler
