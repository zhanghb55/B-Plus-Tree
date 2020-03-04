#ifndef Retarder_h
#define Retarder_h
#include "node.h"
#include <unistd.h>

//定义的时间延迟类
class delay_count{
    long long int count;
public:
    delay_count(){
        count = 0;
    }
    void delay(){
        usleep(20000);//Linux系统调用，暂停20000微秒，即20ms
        count++;    //对暂停的次数进行计数，便于最后统计暂停的总时间
    }
    long long int get_count(){
        return count;
    }
};

#endif /* Retarder_h */
