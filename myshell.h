#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pwd.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#define MAX_LENGTH 80
#define MAX_PATH_LENGTH 1024
#define MAX_USERNAME_LENGTH 1024
#define MAX_COMMOND_NUM 50
#define CYAN "\e[0;36m"
#define HISTORY_NUM 11


void show_prompt();
void init_global();
int get_number();
int get_history_commond();
void get_commond();
void execute_commond();
void add_history();
int split_commond();
int cd_func();
void exec_func();
void exec_backgroundfunc();
void show_history(int n);
