#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>

typedef struct{
    int row;
    int column;
}parameters;

int g[9][9] = {{0, 1, 2, 3, 4, 5, 6, 7, 8},
               {1, 2, 3, 4, 5, 6, 7, 8, 0},
               {2, 3, 4, 5, 6, 7, 8, 9, 1},
               {3, 4, 5, 6, 7, 8, 0, 1, 2},
               {4, 5, 6, 7, 8, 0, 1, 2, 3},
               {5, 6, 7, 8, 0, 1, 2, 3, 4},
               {6, 7, 8, 0, 1, 2, 3, 4, 5},
               {7, 8, 0, 1, 2, 3, 4, 5, 6},
               {8, 0, 1, 2, 3, 4, 5, 6, 7},
              };

void *check_Row(void *param);
void *check_Col(void *param);
void *check_Nine(void *param);

void *check_Row(void *params)
{
    int vis[9];
    parameters* p = (parameters *) params;
    for(int i = p->row; i < 9; i ++ ){
        memset(vis, 0, sizeof(vis));
        for(int j = p->column; j < 9; j ++)
        {   
            vis[g[i][j]] ++;
            if(vis[g[i][j]] > 1)  return (void*)0;
        }
    }
    return (void*)1;
    
}

void *check_Col(void *params)
{
    int vis[9];
    parameters *p = (parameters *) params;
    for(int j = p->column; j < 9; j ++ ){
        memset(vis, 0, sizeof(vis));
        for(int i = p->row; i < 9; i ++ ){
            vis[g[j][i]] ++;
            if(vis[g[j][i]] > 1 )   return (void *)0;
        }
    }
    return (void *)1;
}

void *check_Nine(void *params)
{
    int vis[9];
    parameters *p = (parameters *)params;
    int row = p->row;
    int col = p->column;
    for(int i = row; i < row + 3; i ++ ){
        for(int j = col; j < col + 3; j ++) {
            vis[g[i][j]] ++;
            if(vis[g[i][j]] > 1)    return (void *)0;
        }
    }
    return (void *)1;
}

int main()
{
    parameters *data = (parameters *)malloc(sizeof(parameters));
    data->row = 0;
    data->column = 0;
    void *res[11];
    pthread_t t_Row,t_Col;
    //pthread_t Nine_1, Nine_2, Nine_3, Nine_4, Nine_5, Nine_6, Nine_7, Nine_8, Nine_9;
    pthread_t Nine[9];
    pthread_create(&t_Row, NULL, check_Row, (void *)data);
    pthread_create(&t_Col, NULL ,check_Col, (void *)data);
    parameters *data1 = (parameters *)malloc(sizeof(parameters) * 9);
    int k = 0;
    for(int i = 0; i < 9; i += 3){
        for(int j = 0; j < 9; j += 3){
            (data1 + k)->column = j;
            (data1 + k)->row = i;
            pthread_create(&Nine[i], NULL, check_Nine, (void *)(data1 + k));
            k ++;
        }
    }
    /*
    pthread_create(&t_Nine1, NULL, check_Nine, (void *)data);
    data->col += 3;
    pthread_create(&t_Nine2, NULL, check_Nine, (void *)data);
    data->col += 3;
    pthread_create(&t_Nine3, NULL, check_Nine, (void *)data);
    data->row += 3;
    data->col = 0;
    pthread_create(&t_Nine4, NULL, check_Nine, (void *)data);
    data->col += 3;
    pthread_create(&t_Nine5, NULL, check_Nine, (void *)data);
    data->col += 3;
    pthread_create(&t_Nine6, NULL, check_Nine, (void *)data);
    data->row +=3;
    data->col = 0;
    pthread_create(&t_Nine7, NULL, check_Nine, (void *)data);
    data->
    pthread_create(&t_Nine5, NULL, check_Nine, (void *)data);
    pthread_create(&t_Nine5, NULL, check_Nine, (void *)data);
    */
    int flag = 1;
    pthread_join(t_Row, &res[0]);
    if((int *)res[0] == 0)  flag = 0;
    if(flag){
        pthread_join(t_Col, &res[1]);
        if((int *)res[1] == 0)  flag = 0;
        if(flag){
            for(int i = 2; i < 11; i ++ ){
                pthread_join(Nine[i], &res[i]);
                if((int *)res[i] == 0) {
                    flag = 0;
                    break;
                }       
            }
        }
    }
    if(flag)    puts("success");
    else    puts("fail");
}
