# `thread_queue`: A multi-threaded job queue

Do you have a lot of jobs that need to run?
Running jobs in parallel will make them take less time,
but if you have a thousand jobs to run, you can't just start a new thread for each one.
This thread queue is the solution:
the jobs will be started in the order they were added to the queue,
running in parallel, with a healthy cap on the number of concurrent processes.

[//]: # (TODO: Finish.)
