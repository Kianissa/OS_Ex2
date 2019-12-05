/////////////////////////////////////          Test Manager          /////////////////////////////////////////
/////////////////////////////////////           DEFINITIONS         ////////////////////////////////////////
#define _CRT_SECURE_NO_WARNINGS
#define MAX_STUDENT_NUMBER 10
#define MAX_GRADES_NUMBER 13
#define HW_GRADES_N 10
#define EXAM_N 2
#define MAX_PATH_LEN 300
#define TIMEOUT_IN_MILLISECONDS 5000
#define ID_LEN 11  //includes "\n\0"

/////////////////////////////////////          INCLUSIONS          /////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <windows.h>
///////////////////////////////////////       DECLARATIONS    //////////////////////////////////////////////3
static const int STATUS_CODE_FAILURE = -1;
int get_ids(char *grades_directory);

int main(int argc, char* argv[])
{
	get_ids(argv[1]); //argv[1]=input
	return 0;
	
}


int get_ids(char *grades_directory)
//gets ids and returns number of students.
{
	int i = 0, retval = 0, len = 0;
	FILE *fp;
	char curr_id[11] = "-", *ids_file = "\\student_ids.txt", ids_path[300] = "-";
	char *prefix = "\\grades_", grades_path[300] = "-", TestGrade_path[300]="-";
	
	strcpy(ids_path, grades_directory);
	strcat(ids_path, ids_file); //ids_path=input\\studnet_ids.txt
	

	fp = fopen(ids_path, "r"); 
	if (fp == NULL) {
		printf("Unable to open file! (line45 - test manager)\n");
		return STATUS_CODE_FAILURE;
	}

	strcpy(grades_path, grades_directory);
	len = strlen(grades_path);

	while ((fgets(curr_id, 11, fp)!=NULL)&&(i<=10)) { //runs until EOF or 10.
		
		curr_id[9] = '\0';
		grades_path[len] = '\0';
		printf("curr_id: %s.\n",curr_id, fp);
		strcat(grades_path, prefix);//append id  =>    input\\grades_
		strcat(grades_path, curr_id); // input\\grades_<id>
		//strcat(grades_path, ".txt"); //append .suffix   
	
		strcpy(TestGrade_path, "..\\Debug\\TestGrade.exe ");
		strcat(TestGrade_path, grades_path); //..\\Debug\\TestGrade.exe input\\grades_<id>
		
		//CREATE PROCESS 
		printf("create process with command arguments:%s.\n", TestGrade_path);
		retval=CreateProcessSimpleMain(TestGrade_path);  // this function gets the exit code and returns it.
		if (retval == STILL_ACTIVE)
		{
			printf("Test Grade is still active.\n");
		}
		else
			printf("%s %d\n", curr_id, retval); //must be printed to a file, eventually.
		i++;
	}
	if (fclose(fp) != 0)
	{
		printf("failed to close file. (line 77 test manager)\n");
		return STATUS_CODE_FAILURE;
	}
	return i;
}



int CreateProcessSimpleMain(char command[])
//description: initialzes importtand things in order to create a process
{
	PROCESS_INFORMATION procinfo;
	DWORD				waitcode;
	DWORD				exitcode;
	int					ex = 0;
	BOOL				retVal;
	/* <ISP> TCHAR is a win32  */
								  /* generic char which may be either a simple (ANSI) char or a unicode char, */
								  /* depending on behind-the-scenes operating system definitions. Type LPTSTR */
								  /* is a string of TCHARs. Type LPCTSTR is a const string of TCHARs. */

								  /*  Start the child process. */
	retVal = CreateProcessSimple(command, &procinfo);
	if (retVal == 0)
	{
		printf("Process Creation Failed!\n");
		return -1;
	}



	waitcode = WaitForSingleObject(
		procinfo.hProcess,
		TIMEOUT_IN_MILLISECONDS); /* Waiting 5 secs for the process to end*/


	GetExitCodeProcess(procinfo.hProcess, &exitcode);
	ex = exitcode;//return the son process result
	//printf("The exit code for the process is %d\n", ex);
	if (ex == STILL_ACTIVE)
	{
		printf("Test Grade is still active.\n");
	}


	CloseHandle(procinfo.hProcess); /* Closing the handle to the process */
	CloseHandle(procinfo.hThread); /* Closing the handle to the main thread of the process */
	return ex;
}

BOOL CreateProcessSimple(LPTSTR CommandLine, PROCESS_INFORMATION *ProcessInfoPtr)
{
	STARTUPINFO	startinfo = { sizeof(STARTUPINFO), NULL, 0 }; /* <ISP> here we */
															  /* initialize a "Neutral" STARTUPINFO variable. Supplying this to */
															  /* CreateProcess() means we have no special interest in this parameter. */
															  /* This is equivalent to what we are doing by supplying NULL to most other */
															  /* parameters of CreateProcess(). */

	return CreateProcess(NULL, /*  No module name (use command line). */
		CommandLine,			/*  Command line. */
		NULL,					/*  Process handle not inheritable. */
		NULL,					/*  Thread handle not inheritable. */
		FALSE,					/*  Set handle inheritance to FALSE. */
		NORMAL_PRIORITY_CLASS,	/*  creation/priority flags. */
		NULL,					/*  Use parent's environment block. */
		NULL,					/*  Use parent's starting directory. */
		&startinfo,				/*  Pointer to STARTUPINFO structure. */
		ProcessInfoPtr			/*  Pointer to PROCESS_INFORMATION structure. */
	);
}
