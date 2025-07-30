#include <iostream>

#include "MsgBus.h"
#include "topics.h"

#include <unistd.h>
#include <sys/syscall.h>
#include <linux/unistd.h>

using namespace std;

 int main(int argc,char** argv)
 {
    cout<<"main thread tid is:"<<gettid()<<endl;
    MsgBus::GetInst().Subscribe(SYNC_TEST1,[](){
        cout<< gettid() <<":test sync topics!"<<endl;
    });
    MsgBus::GetInst().SendReq(SYNC_TEST1);

    MsgBus::GetInst().Subscribe(ASYNC_TEST1,[](){
        cout<<gettid()<<":test async topics!"<<endl;
    });
    MsgBus::GetInst().SendReq(ASYNC_TEST1);
    sleep(1);
    return 0;
 }