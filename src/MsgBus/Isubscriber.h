#ifndef _MSG_BUS_ISUBSCRIBER_H_
#define _MSG_BUS_ISUBSCRIBER_H_

#pragma once

#include <stdint.h>

class ISubscriber
{
    virtual void Subscribe(uint32_t topic) = 0;
};

#endif
