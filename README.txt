-----------------------------------------------------------------
                                     _      _   ___    __ ____
                                    | |    /_\ | _ )  /  \__ /
                                    | |__ / _ \| _ \ | () |_ \
                                    |____/_/ \_\___/  \__/___/
                          
-----------------------------------------------------------------
Author: 	Matt W. Martin, 4374851
		kaethis@tasmantis.net

Project:	CS3790, Lab 03:
		Producer-Consumer Solution

Date Issued:	28-Oct-2015
Date Archived:	XX-Nov-2015

File:		README.txt


Comments:	This assignment consists of two programs:

		- prodcons:	This C++ program provides a demo-
				nstration of the classic multi-
				process syncronization problem
				known as the producer-consumer
				(or bounded-buffer) problem.

		- ProdCons:	Virtually identical to "prodcons"
				except implemented in the Java
				language.

The problem describes two processes, a producer and a consumer,
who share a common fixed-size buffer used as a queue.  The prod-
ucer generates an item and places it in the buffer, meanwhile
the consumer removes items from the buffer one at a time.  The
solution is to ensure the producer won't try to add items to the
buffer if it is full, and that the consumer won't try to remove
an item if the buffer is empty.  We arrive at this solution with
the use of semaphores, namely "avail" which signifies the number
of items available in the buffer, and "empty" signifying the
number of empty spaces that can written to.  When a new item is
added to the buffer, "avail" is incremented and "empty" is
decremented.  For the purpose of this demonstration, the buffer
can hold up to 10 items.

The problem can be generalized to have multiple producers and
consumers, as is the case with this assignment.  However, this
can yield a serious race condition that could result in two or
more threads reading to (in the case of the consumer) or writing
from (the producer)  the same shared memory location at the same
time.  Consider the following:

1. Two producers decrement the "empty" semaphore.
2. One of the producers determines the next available location
   in the buffer.
3. The other determines the next empty loc and arives at the same
   result as the first producer.
4. Both producers write into the same memory location.  Whoops!

An analogous scenario might also occur w/ two or more consumers.
To overcome this, a means of mutual-exclusion is implemented
whenever a produce() or consume() occurs via a mutex (C++) or
lock (Java) object, named "mut".

(Of course, we don't actually have to worry about the latter
happening in this demo, as the bounded-buffer is only hypoth-
etical and thereby no actual items are being written and/or read.
To remain faithful to the demonstration, however, mutual-
exclusion is implemented anyway!)


Instructions:	Executing either program requires two integer 
		command-line arguments: the first is the number
		of threads executing the producer function, the
		second being the number of consumers. 

		  prodcons:	./cthreads 5 4

		  ProdCons:	./java ProdCons 5 4

		In both cases above, there will be 5 producers
		and 4 consumers.  Because the programs run
		indefinitely, the user (that means YOU!) must
		terminate the process manually.

		NOTE:	Both source files, as well as their
			respective MAKEFILES, are located in
			their own separate directories.


MAKE:		In ./c directory:

		  ./prodcons	Compiles the prodcons program.
		  ./clean	Removes the program.

		In ./java directory:

		  ./prodcons	Compiles the ProdCons program.
		  ./clean	Removes program and class files.

		In top-level (.) directory:
		
		  ./archive	Creates archive in parent-dir.


GIT Repository:	 https://github.com/kaethis/CS3790_Lab03.git
