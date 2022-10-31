#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
    int st;
    int ed;
}parameters;

int array[16] = {0,1,2,3,5,6,4,7,8,9,88,99,22,33,56,48};

void* my_sort(void *data)
{
    parameters *p = (parameters *)data;
    int st = p->st;
    int ed = p->ed;
    for(int i = st; i < ed - 1; i ++ )
    {
        for(int j = st; j + 1 < ed; j ++ )
        {
            if(array[j] > array[j + 1]) {
                int tmp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = tmp;
            }
        }
    }
    printf("数组两部分分别为:");
    for(int i = st; i < ed; i ++ )  printf("%d ",array[i]);
    puts("");
    return (void*)0;
}

void* merge_array(void *data)
{
    parameters *pdata = (parameters *) data;
    int st = pdata->st, ed = pdata->ed;
    int len = ed - st;
    int mid = len / 2;
    int i = st, j = mid;
    int *p = (int *)malloc(sizeof(array));
    while(i < mid && j < ed)
    {
        if(array[i] <= array[j])   {
             *p = array[i];
             i ++;
        }
        else {
           *p = array[j];
            j ++;
        }
        p ++;
    }
    while(i < mid){
        *p = array[i];
        p ++;
        i ++;
    }
    while(j < ed){
        *p = array[j];
        j ++;
        p ++;
    }
    p = p - len;
    for(int i = 0; i < len; i ++ ){
        array[i] = *p;
        p ++;
    }
    return (void*)0;
}
    
int main()
{
    parameters* data = (parameters *)malloc(sizeof (parameters));
    data->st = 0;
    data->ed = 16;
    parameters *data1 = (parameters *)malloc(sizeof (parameters));
    data1->st = 0;
    data1->ed = data->ed / 2;
    parameters *data2 = (parameters *)malloc(sizeof (parameters));
    data2->st = data1->ed;
    data2->ed = data->ed;
    pthread_t sort1,sort2,merge;
    pthread_create(&sort1, NULL, my_sort, (void *)data1);
    pthread_create(&sort2, NULL, my_sort, (void *)data2);
    pthread_join(sort1, NULL);
    pthread_join(sort2, NULL);
    pthread_create(&merge, NULL, merge_array, (void *)data);
    pthread_join(merge, NULL);
    printf("排序后结果为:");
    for(int i = 0; i < data->ed; i ++ ){
        printf("%d ", array[i]);
    }
    puts("");
    return 0;
}
