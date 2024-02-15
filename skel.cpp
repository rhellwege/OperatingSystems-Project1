#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

using namespace std;

/* The pipe for parent-to-child communications */
int parentToChildPipe[2];

/* The pipe for the child-to-parent communication */
int childToParentPipe[2];

/* The read end of the pipe */
#define READ_END 0

/* The write end of the pipe */
#define WRITE_END 1

/* The maximum size of the array of hash programs */
#define HASH_PROG_ARRAY_SIZE 6

/* The maximum length of the hash value */
#define HASH_VALUE_LENGTH 1000

/* The maximum length of the file name */
#define MAX_FILE_NAME_LENGTH 1000

/* The array of names of hash programs */
const string hashProgs[] = {"md5sum", "sha1sum", "sha224sum", "sha256sum", "sha384sum", "sha512sum"};

string fileName;

/**
 * The function called by a child
 * @param hashProgName - the name of the hash program
 */
void computeHash(const string& hashProgName)
{
		
	/* The hash value buffer */
	char hashValue[HASH_VALUE_LENGTH];

	/* Reset the value buffer */
	memset(hashValue, (char)NULL, HASH_VALUE_LENGTH);
	
	/* The received file name string */
	char fileNameRecv[MAX_FILE_NAME_LENGTH];
	
	/* Fill the buffer with 0's */
	memset(fileNameRecv, (char)NULL, MAX_FILE_NAME_LENGTH);

	
	
	/** TODO: Now, lets read a message from the parent **/
	/* get the filename to hash from the parent via the parent -> child read pipe */
	if (read(parentToChildPipe[READ_END], fileNameRecv, sizeof(fileNameRecv)) < 0)
	{
		perror("read");
		exit(-1);
	}
	
	/* Glue together a command line <PROGRAM NAME>. 
 	 * For example, sha512sum fileName.
 	 */
	string cmdLine(hashProgName);
	cmdLine += " ";
	cmdLine += fileNameRecv;	
	
    /* TODO: Open the pipe to the program (specified in cmdLine) 
	* using popen() and save the ouput into hashValue. See popen.cpp
    * for examples using popen.
	.
	.
	.
	*/
		
	// spawn the process and obtain the fd to read from
	FILE* progOutput = popen(cmdLine.c_str(), "r");

	/* Make sure that popen succeeded */
	if(!progOutput)
	{
		perror("popen");
		exit(-1);
	}

	// get the output of the hash program
	if(fread(hashValue, sizeof(char), sizeof(char) * HASH_VALUE_LENGTH, progOutput) < 0)
	{
		perror("fread");
		exit(-1);
	}
	
	/* Close the file pointer representing the program output */
	if(pclose(progOutput) < 0)
	{
		perror("close hashprogram");
		exit(-1);
	}
		
	/* TODO: Send a string to the parent 
 	 .
	 .
	 .
	*/
	if (write(childToParentPipe[WRITE_END], hashValue, HASH_VALUE_LENGTH) < 0)
	{
		
		perror("write to parent");
		exit(-1);
	}

	// we no longer need to send messages to parent, close the write pipe.
	if (close(childToParentPipe[WRITE_END]) < 0)
	{
		perror("close child -> parent write pipe");
		exit(-1);

	}

	/* The child terminates */
	exit(0);
	
}

void parentFunc(const string& hashProgName)
{

	/* I am the parent */

	/** TODO: close the unused ends of two pipes. **/
	/* Close the parent -> child read pipe */
	if (close(parentToChildPipe[READ_END]) < 0) 
	{
		perror("could not close parent -> child read end from child");
		exit(-1);
	}

	/* Close the read-end of the child-to-parent pipe */
	if(close(childToParentPipe[WRITE_END]) < 0)
	{
		perror("close");
		exit(-1);
	}

	/* The buffer to hold the string received from the child */
	char hashValue[HASH_VALUE_LENGTH];

	/* Reset the hash buffer */
	memset(hashValue, (char)NULL, HASH_VALUE_LENGTH);


	/* TODO: Send the string to the child
	 .
	 .
	 .
	 */
	if (write(parentToChildPipe[WRITE_END], hashProgName.c_str(), hashProgName.length()) < 0)
	{
		perror("could not send message from parent to child.");
		exit(-1);
	}

	/* TODO: Read the string sent by the child
	.
	.
	.
	*/

	// get the output of the hash program from the child -> parent read pipe
	if(fread(hashValue, sizeof(char), sizeof(char) * HASH_VALUE_LENGTH, childToParentPipe[READ_END]) < 0)
	{
		perror("could not read message from child to parent.");
		exit(-1);
	}

	/* Print the hash value */
	fprintf(stdout, "%s HASH VALUE: %s\n", hashProgName.c_str(), hashValue);
	fflush(stdout);

}

int main(int argc, char** argv)
{
	
	/* Check for errors */
	if(argc < 2)
	{
		fprintf(stderr, "USAGE: %s <FILE NAME>\n", argv[0]); 
		exit(-1);
	}	
	
	/* Save the name of the file */
	fileName = argv[1];
	
	/* The process id */
	pid_t pid;
	
	/* Run a program for each type of hashing algorithm hash algorithm */	
	for (int hashAlgNum = 0; hashAlgNum < HASH_PROG_ARRAY_SIZE; ++hashAlgNum)
	{

		/** TODO: create two pipes COMPLETE **/
		/* Create a parent-to-child pipe */
		if(pipe(parentToChildPipe) < 0)
		{
			perror("could not create p-c pipe");
			exit(-1);
		}


		/* Create a child-to-parent pipe */
		if(pipe(childToParentPipe) < 0)
		{
			perror("could not create c-p pipe");
			exit(-1);

		}

		/* Fork a child process and save the id */
		if ((pid = fork()) < 0)
		{
			perror("fork");
			exit(-1);
		}
		/* I am a child */
		else if (pid == 0)

		{
			/** TODO: close the unused ends of two pipes **/

			/* Close the parent -> child write pipe */
			if (close(parentToChildPipe[WRITE_END]) < 0) 
			{
				perror("could not close parent -> child write end from child");
				exit(-1);
			}

			/* Close the read-end of the child-to-parent pipe */
			if(close(childToParentPipe[READ_END]) < 0)
			{
				perror("close");
				exit(-1);
			}

			/* Compute the hash */
			computeHash(hashProgs[hashAlgNum]);
		}

		else
		{

			parentFunc(hashProgs[hashAlgNum]);

			/* Wait for the child to terminate */
			if (wait(NULL) < 0)
			{
				perror("wait");
				exit(-1);
			}
		}

	}
	
		
	
	return 0;
}
