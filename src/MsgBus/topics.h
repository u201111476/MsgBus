#ifndef _MSG_BUS_TOPICS_H_
#define _MSG_BUS_TOPICS_H_

#pragma once

enum TOPICS
{
//同步消息从这里开始
    SYNC_TEST1 = 0x0001,                 
    
//异步消息从下面开始
    ASYNC_TEST1 = 0x4001
};

#endif  //!_MSG_BUS_TOPICS_H_
