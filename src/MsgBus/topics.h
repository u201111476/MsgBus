#ifndef _MSG_BUS_TOPICS_H_
#define _MSG_BUS_TOPICS_H_

#pragma once

enum TOPICS
{
//同步消息从这里开始
    SYNC_VOID_LAMBDA = 0x0001,
    SYNC_INT_LAMBDA,
    SYNC_CHAR_LAMBDA,
    SYNC_DOUBLE_LAMBDA,
    SYNC_POINTER_LAMBDA,
    SYNC_CUSTOM_CLASS_LAMBDA,           //好像必须传引用
    SYNC_CUSTOM_CLASS_REF_LAMBDA,                 
    
//异步消息从下面开始
    ASYNC_VOID_LAMBDA = 0x4001,
    ASYNC_CUSTOM_CLASS_REF_LAMBDA
};

#endif  //!_MSG_BUS_TOPICS_H_
