/////////////////////////////////////          TEST GRADE          /////////////////////////////////////////
// description:                                                                                           //
/////////////////////////////////////           DEFINITIONS         ////////////////////////////////////////
#define _CRT_SECURE_NO_WARNINGS
#define MAX_STUDENT_NUMBER 10
#define MAX_GRADES_NUMBER 13
#define HW_GRADES_N 10
#define BRUTAL_TERMINATION_CODE 0x55
#define MAX_LOCAL_PATH_LEN 35
#define NUM_THREADS 1
#define ERROR_CODE ((int)(-1))
#define SUCCESS_CODE ((int)(0))

/////////////////////////////////////          INCLUSIONS          /////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <windows.h>
#include <stdbool.h>

///////////////////////////////////////       DECLARATIONS    //////////////////////////////////////////////3
static const int STATUS_CODE_FAILURE = -1;
static HANDLE CreateThreadSimple(LPTHREAD_START_ROUTINE p_start_routine, LPVOID p_thread_parameters, LPDWORD p_thread_id);
DWORD WINAPI getGradeThread(LPVOID lpParam);


void sort_numbers_of_array(int *number);
double clc_avg_8(int number[MAX_GRADES_NUMBER - 3]);
int grade_per_student(char *student_path);
int get_grade(char *path);
int creatThreadMain(char *student_path);

///////////////////////////////////////       MAIN        //////////////////////////////////////////////3

int main(int argc, char* argv[]) //argv[1]=input\\grades_123456789
{
	int res = 0;
	FILE *fp = NULL;
	
	//res= grade_per_student(argv[1]); 
	res = creatThreadMain(argv[1]);
	
	printf("test grade, back to main, student result: %d\n", res);
	return res;
}


///////////////////////////////////////       THREAD RELATED FUNCTIONS        //////////////////////////////////////////////3


int creatThreadMain(char *student_path)
{
	char path_cpy[MAX_LOCAL_PATH_LEN] = { "0" };
	HANDLE thread_handle;
	DWORD thread_id;
	DWORD wait_code;
	DWORD exit_code;
	BOOL ret_val;
	
	strcpy(path_cpy, student_path); //path_cpy=input\\grades_123456789
	strcat(path_cpy, "\\ex07.txt");


	/* Create thread */
	thread_handle = CreateThreadSimple(getGradeThread, path_cpy, &thread_id);
	if (NULL == thread_handle)
	{
		printf("Error when creating thread\n");
		return ERROR_CODE;
	}


	/* Wait */
	wait_code = WaitForSingleObject(thread_handle, INFINITE);
	if (WAIT_OBJECT_0 != wait_code)
	{
		printf("Error when waiting\n");
		return ERROR_CODE;
	}

	/* Check the DWORD returned by myBeautifulThread */
	ret_val = GetExitCodeThread(thread_handle, &exit_code);
	if (0 == ret_val)
	{
		printf("Error when getting thread exit code\n");
	}

	printf("exit code %d\n", exit_code);

	/* Close thread handle */
	ret_val = CloseHandle(thread_handle);
	if (false == ret_val)
	{
		printf("Error when closing\n");
		return ERROR_CODE;
	}

	return exit_code;
}

static HANDLE CreateThreadSimple(LPTHREAD_START_ROUTINE p_start_routine, 
	LPVOID p_thread_parameters, 
	LPDWORD p_thread_id)
{
	HANDLE thread_handle;

	if (NULL == p_start_routine)
	{
		printf("Error when creating a thread");
		printf("Received null pointer");
		exit(ERROR_CODE);
	}

	if (NULL == p_thread_id)
	{
		printf("Error when creating a thread");
		printf("Received null pointer");
		exit(ERROR_CODE);
	}

	thread_handle = CreateThread(
		NULL,                /*  default security attributes */
		0,                   /*  use default stack size */
		p_start_routine,     /*  thread function */
		p_thread_parameters, /*  argument to thread function */
		0,                   /*  use default creation flags */
		p_thread_id);        /*  returns the thread identifier */

	return thread_handle;
}



DWORD WINAPI getGradeThread(LPVOID lpParam)
{
	char *ptr = (char *)lpParam;
	int grade = 0;
	/* Check if lpParam is NULL */
	if (NULL == lpParam)
	{
		printf("Thread Param is null.\n");
		return ERROR_CODE;
	}
	
	grade=get_grade(ptr);
	
	return grade;
}




///////////////////////////////////////       "normal" functions       //////////////////////////////////////////////3


int get_grade(char *path)
{
	FILE *f_ptr = NULL;
	int grade = 0;
	f_ptr = fopen(path, "r");
	if (f_ptr == NULL) {
		printf("Failed to open file.\n");
		return STATUS_CODE_FAILURE;
	}
	fscanf(f_ptr, "%d", &grade);
	
	printf("got grade %d\n", grade);
	if (fclose(f_ptr) != 0)
	{
		printf("failed to close file.\n");
		return STATUS_CODE_FAILURE;
	}

	return grade;
}

void sort_numbers_of_array(int* number)
//input:A random array 
//funtionality:sorting an array 
//output:sorted array
{
	int i, j, a;

	for (i = 0; i < HW_GRADES_N; ++i)

	{

		for (j = i + 1; j < HW_GRADES_N; ++j)

		{

			if (number[i] < number[j])

			{

				a = number[i];

				number[i] = number[j];

				number[j] = a;

			}

		}

	}



	//printf("The numbers arranged in descending order are given below\n");



	/*for (i = 0; i < HW_GRADES_N; ++i)

	{

		printf("%d\n", number[i]);

	}*/



}

double clc_avg_8(int number[MAX_GRADES_NUMBER - 3])
{
	int i;
	double sum = 0;
	for (i = 0; i < 8; i++) {
		if (number[i] >= 60) {
			sum = sum + number[i];
		}

	}
	return(sum / 8);
}

int final_test(int Agrade, int Bgrade)
{
	if ((Agrade >= 60) && (Bgrade >= 60))
		return(Bgrade);
	else if ((Agrade > 60) && (0 < Bgrade) && (Bgrade < 60))
		return(0);
	else if ((Agrade < 60) && (Bgrade >= 60))
		return(Bgrade);
	else if ((Agrade < 60) && (0 < Bgrade) && (Bgrade < 60))
		return(0);
	else if ((Agrade >= 60) && (Bgrade == 0))
		return(Agrade);
	else if ((Agrade == 0) && (Bgrade == 0))
		return(0);
}

int mid_test(int grade) {
	if (grade >= 60)
		return(grade);
	return 0;
}

int grade_per_student(char *student_path)
{

	FILE *file_p;
	int i = 0, k = 0, grade = 0, ex_avg = 0, best_test = 0, mid_grade = 0, path_len = 0, final_int = 0;
	int hw_grades[HW_GRADES_N] = { 0 }, final_exam[2] = { 0 };
	double final_grade = 0;
	char path2[MAX_LOCAL_PATH_LEN], file_name[MAX_GRADES_NUMBER][MAX_GRADES_NUMBER-1] = { "ex01.txt", "ex02.txt","ex03.txt","ex04.txt","ex05.txt","ex06.txt","ex07.txt","ex08.txt","ex09.txt","ex10.txt","midterm.txt","moedA.txt","moedB.txt" };
	char *p_path = NULL;
	strcpy(path2, student_path);// path2=input\\grades_<id> 
	strcat(path2, "\\");// path2=input\\grades_<id>\\ 
	path_len = strlen(path2);


	while (k < MAX_GRADES_NUMBER)
	{
		path2[path_len] = '\0';
		strcat(path2, file_name[k]);
		file_p = fopen(path2, "r");
		if (file_p == NULL) {
			printf("Failed to open file.\n");
			return STATUS_CODE_FAILURE;
		}
		fscanf(file_p, "%d", &grade);
		if (k < 10) //HOMEWORK
			hw_grades[k] = grade;
		else if (k == 10) //MIDTERM
			mid_grade = grade;
		else //FINALS
		{
			final_exam[k - 11] = grade;
		}

		if (fclose(file_p) != 0)
		{
			printf("failed to close file.\n");
			return STATUS_CODE_FAILURE;
		}
		k++;
	}
	sort_numbers_of_array(&hw_grades);//here we should have a sorted array of ex0 to ex10
	ex_avg = clc_avg_8(hw_grades);//the average of the best 8 HWS 
	best_test = final_test(final_exam[0], final_exam[1]);
	mid_grade = mid_test(mid_grade);
	i++;
	final_grade = (0.2*ex_avg) + (0.2*mid_grade) + (0.6*best_test);
	final_grade = ceil(final_grade);
	// we need to save and update the final great of each student in a file. later.
	final_int = (int)final_grade;
	printf("final grade of student_id %s is: %f. %d.\n", student_path, final_grade,final_int);
	return final_int;

} // will not be used. should be delteted after Threads are completed.





