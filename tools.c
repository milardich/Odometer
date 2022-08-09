#include<time.h>
void wait(long time_microseconds){
    time_t t1 = clock();
    while(t1 + time_microseconds >= clock());
}
