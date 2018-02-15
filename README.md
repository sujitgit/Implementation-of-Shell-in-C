1. Execute commands with multiple arguments. -                Implemented
2. Execute commands in either foreground or background mode-  Implemented
3.Maintain multiple processes running in background mode simultaneously - Implemented
4. List all currently running background jobs using "listjobs" command    -Implemented 
5. Bring a background process to foreground using the fg command with process ID as argument   -Implemented
6. Terminate a process by pressing [Ctrl-C] -   particllay Implemented
7. The exit command should terminate your shell    - Implemented
8. The cd command must actually change the directory of your shell and the pwd command must return the current directory of your shell - Implemented


Sample output:
please enter the command you want this code to execute
==================================================================
minish>ls

Child process: pid=2950 pgid=2949

Child process after setting PGid: pid=2950 pgid=2950
Command to be run is ls
ahrs_float_invariant.c	       Minish_3_9_8_7.c
a.out			       Minish_3_9_8_8.c
Archive			       Minish_3_9_8_9.c
cars			       Minish-Archive
==================================================================
minish>ls -l -r -t

Child process: pid=2962 pgid=2949

Child process after setting PGid: pid=2962 pgid=2962
Command to be run is ls
total 728
-rw-rw-r-- 1 sujit sujit   1615 Aug 29 16:16 fork_exec_example.c
-rw-rw-r-- 1 sujit sujit   1394 Aug 29 16:28 fork_exec_example_recursive.c
-rw-rw-r-- 1 sujit sujit     42 Aug 31 19:13 options
-rw-rw-r-- 1 sujit sujit   2630 Sep  5 11:44 SingleLinkedList.cpp

==================================================================
minish>sleep 10 &

You have requested the command to be run in background
exited, status=0

please enter the command you want this code to execute

Child process: pid=2980 pgid=2949

Child process after setting PGid: pid=2980 pgid=2980
Command to be run is sleep


=================================================================
minish>joblist

List of backgrounded processes:

Command sleep with PID 2980 Status:RUNNING 

=================================================================
minish>fg 2980

Process is being put in foreground now, pID is: 2980


=================================================================

minish>pwd

Present Working Directory is:      /home/sujit/Desktop    

please enter the command you want this code to execute
minish>






