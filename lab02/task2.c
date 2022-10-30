#include <stdio.h>
#include <pthread.h>

typedef struct{
    int st;
    int ed;
}parameters;

int array[10] = {0,1,2,3,4,5,6,7,8,9};

void* my_sort(void *data)
{
    int st = data->st;
    int ed = data->ed;
    for(int i = st; i < ed; i ++ )
    {
        for(int j = st + 1; j < ed; j ++ )
        {
            if(array[i] > array[j]) {
                int tmp = array[i];
                array[i] = array[j];
                array[j] = tmp;
            }
        }
    }
}

void* merge_array(void *data)
{
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
}
    
int main()
{
    parameters* data = (parameters *)malloc(sizeof (parameters));
    data->st = 0;
    data->ed = 10 / 2;
    pthread_t sort1,sort2,merge;
    pthread_create(&sort1, NULL, my_sort, (void *)data);
    data->st = data->ed;
    data->ed = 10;
    pthread_create(&sort2, NULL, my_sort, (void *)data);
    data->st = 0;
    data->ed = 10;
    pthread_create(&merge, NULL, merge_array, (void *)data);
    pthread_join(sort1);
    pthread_join(sort2);
    pthread_join(merge);
    for(int i = 0; i < 10; i ++ ){
        printf("%d ", array[i]);
    }
    return 0;
}
