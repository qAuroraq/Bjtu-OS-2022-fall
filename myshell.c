#include "myshell.h"

int len;    //store the length
int arguments_count;
int history_size;
char args[MAX_LENGTH];
char backup[MAX_LENGTH];
char commond_s[MAX_COMMOND_NUM][MAX_LENGTH];
char *argv[MAX_LENGTH];
char history_save[HISTORY_NUM][MAX_LENGTH];
int head,tail;
int count_commond,is_history;

void show_prompt(){
    char path[MAX_PATH_LENGTH];    // 存储当前路径
    char username[MAX_USERNAME_LENGTH];
    getcwd(path, MAX_PATH_LENGTH);
    struct passwd *pwd;
    pwd = getpwuid(getuid());
    strcpy(username, pwd->pw_name);
    printf(CYAN "[myshell]%s:%s$ ",username,path);
}

void init_global(){
    len = 0;
    arguments_count = 0;
    is_history = 0;
}

void get_commond(){
    fgets(args, MAX_LENGTH, stdin);
    len = strlen(args);
    if(args[len - 1] == '\n')    {
        args[len - 1] = '\0';
        len --;
    }
    strcpy(backup, args);
}

int split_commond(char str[]){
    if(len == 0)    return 0;
    else{ 
        int i = 0;
        char *token;
        char *p;
        token = strtok_r(str, " ", &p); //利用strtok函数分割字符串
        if(token != NULL)   strcpy(commond_s[i], token);
        while(token != NULL){
            strcpy(commond_s[i], token);
            i ++;
            token = strtok_r(NULL, " ", &p);
        }
        arguments_count = i;
        if(arguments_count == 1){ 
             if(strcmp(commond_s[0], "!!") == 0)
             {
                is_history = 1;
                int is_success = get_history_commond(0);
                if(is_success != -1)    split_commond(args);
                else    return 0;
             }
             else if(commond_s[0][0] == '!')
             {
                 int number = get_number(commond_s[0]);
                 if(number != -1)
                 {
                     int is_success = get_history_commond(number);
                     if(is_success != -1)   {
                         split_commond(args);
                         is_history = 1;
                    }
                     else    return 0;
                 }
                 else{
                     printf("输入历史指令格式错误!");
                     return 0;
                 }
             }
        }

        for(int j = 0; j < arguments_count; j ++ )
        {
            argv[j] = commond_s[j];
        }     
    }
    return 1;
}

int get_history_commond(int n)
{
    if(n == 0)
    {
        if(head != tail)    
        {
            strcpy(args, history_save[tail - 1]);
            strcpy(backup,args);
        }
        else{
            printf("历史指令不存在!");
            return -1;
        }   
    }
    else{
        if(n > count_commond || n <= count_commond - 10){
            printf("历史指令不存在!");
            return -1;
        }
        else{
            int history_size = (tail + HISTORY_NUM - head) % HISTORY_NUM;
            int i = head;
            int now = count_commond - history_size + 1;
            while(i != tail)
            {
                if(now == n)
                {
                    strcpy(args, history_save[i]);
                    strcpy(backup, args);
                    break;
                }
                now ++;
                i = (i + 1) % HISTORY_NUM;   
            }
            return 0;
        }
    }
    return 0;
}

void execute_commond(){
    if(strcmp(commond_s[0], "cd") == 0) { 
        cd_func();
    }
    else if(strcmp(commond_s[0], "history") == 0){
        show_history(0);
    }
    else if(strcmp(commond_s[0], "exit") == 0){
        exit(0);
    }
    else if(strcmp(commond_s[arguments_count - 1], "&") == 0){
        exec_backgroundfunc();
    }
    else{
        exec_func();
    }
}

int get_number(char str[])
{
    int len = strlen(str);
    int res = 0;
    for(int i = len - 1; i > 0; i -- )
    {
        if(str[i] >= '0'&& str[i] <= '9')
        {
            int x = str[i] - '0';
            res = res * 10 + x;
        }
        else    return -1;
    }
    return res;
}

void add_history(char commond[])
{
    if((tail + 1) % HISTORY_NUM == head)    head ++;
    strcpy(history_save[tail], commond);
    tail = (tail + 1) % HISTORY_NUM;
}

void show_history(int n)
{
     int i = head;
     int cnt = (tail + HISTORY_NUM - head) % HISTORY_NUM;
     while(i != tail){
         printf("%d %s\n",count_commond - cnt + 1, history_save[i]);
         i = (i + 1) % HISTORY_NUM;
         cnt --;
     }
}

int cd_func()
{
    if(arguments_count > 2)   
    {
         puts("参数过多");
         return -1;
    }
    else{
        char old_dir[MAX_PATH_LENGTH];
        getcwd(old_dir, MAX_PATH_LENGTH);
        if(arguments_count == 1)
        {
             puts(old_dir);
             return 0;
        }
        else{
            int ret = chdir(commond_s[1]);
            if(!ret)
            {
                getcwd(old_dir, MAX_PATH_LENGTH);
                return 0;
            }
            else{
                puts("文件路径不存在！");
                return -1;
            }
        }
    }
}

void exec_func()
{
    int pid = fork();
    if(pid == -1)   puts("创建子进程未成功");
    else if(pid == 0){
        execvp(argv[0], argv);
        printf("输入命令错误:%s\n",backup);
        exit(1);
    }
     int status;
     wait(&status);
     if(!WIFEXITED(status)){
        int err = WEXITSTATUS(status);
        printf("Error:%s\n",strerror(err));
    }
}

void exec_backgroundfunc()
{
    int pid = fork();
    argv[arguments_count - 1] = NULL;
    arguments_count --;
    if(pid < 0)   puts("创建子进程失败");
    else if(pid == 0){
        //将标准输入输出重定向到/dev/null
        // freopen("/dev/null", "w", stdout);
        // freopen("/dev/null", "r", stdin);
        signal(SIGCHLD, SIG_IGN);
        execvp(argv[0], argv);
        printf("输入命令错误:%s\n",argv[0]);
        exit(1);
    }  
}


int main()
{
    while(1){
        init_global();
        show_prompt();
        get_commond();
        int flag = split_commond(args);
        if(flag){
            count_commond ++;
            add_history(backup);
            execute_commond();
            if(is_history)  puts(backup);
        }
    }
    return 0;    
}
