# OperatingSystems-Project1
Use pipes and forking to create child processes that communicate via pipes and run a set of hashing algorithms over a given file

CPSC 351 Project 1 (100 points)
Due by March 10, 11:59 PM

This Project can be done individually or in a group of maximum 3 people. For a group of 2 or 3 members, each of the group members will have to submit, even though the work will be the same for all the members of the group. All members of the group will get the same grade. If any group member does not submit, that person gets 0. Or if any group member submits different answer/s, that person’s work will be graded as individual submission. Indicate in an additional .txt file, the names and email ids of members in the group. If working individually, indicate in the .txt, your name and email id.
Goal:
In popen.cpp, we saw how to:
Launch the hash program md5sum to compute the MD5 hash of a file named /bin/ls. Also, to get a file pointer representing the output stream of the program.
In this project, you should read in a file name from the terminal. The given skel.cpp runs a for loop over a set of hash programs. In each iteration of this for loop, a child process is forked. The parent sends the file name to the child. The child calls a function (popen) to compute the respective hash (as specified by the hash program) of the file name. The child sends the computed hash value back to the parent.
The parent communicates with the child using pipes.
Starter code is provided to you in skel.cpp. A lot of hints in the form of comments are also provided in skel.cpp. You should add your code to the given starter code, wherever the comments guide you to do that. Over and above that, you should remember to add code where it is important for completing any task (the comments may not always be provided for everything).
Hint: A lot of the code builds upon the code in popen.cpp and double_pipe.cpp.
FILES TO BE SUBMITTED
skel.cpp (completed with your code)
.txt file with name/s and email id/s
Blurb for your resume
Use your GitHub account as a ready-to-show portfolio of your programming projects to potential employers.

