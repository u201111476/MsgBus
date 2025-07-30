#include <iostream>

#include "MsgBus.h"
#include "topics.h"

#include <unistd.h>
#include <sys/syscall.h>
#include <linux/unistd.h>

class A
{
public:
    A(){cout<<"A's cons!"<<endl;}
    A(const A&){cout<<"A's copy cons!"<<endl;}
    A(A&&){cout<<"A's move cons!"<<endl;}
    A& operator=(const A&){cout<<"A's operator=!"<<endl; return *this;}
    ~A(){cout<<"A's decons!"<<endl;}
    int m_a{0};
};


using namespace std;

 int main(int argc,char** argv)
 {
    cout<<"main thread tid is:"<<gettid()<<endl;
    //测试同步无参lambda
    MsgBus::GetInst().Subscribe(SYNC_VOID_LAMBDA,[](){
        cout<< gettid() <<":test sync topics!"<<endl;
    });
    MsgBus::GetInst().SendReq(SYNC_VOID_LAMBDA);
    //测试同步单一int参数lambda
    MsgBus::GetInst().Subscribe(SYNC_INT_LAMBDA,[](const int& a){
        cout<<gettid()<<":test sync topics with int arg:"<<a<<",address:"<<&a<<endl;
        //a=100;
    });
    int a=99;
    cout<<"a is:"<<a<<",a's addr is:"<<&a<<endl;
    MsgBus::GetInst().SendReq<void,const int&>(a,SYNC_INT_LAMBDA);
    cout<<"a is:"<<a<<",a's addr is:"<<&a<<endl;
    //测试同步单一char参数lambda
    MsgBus::GetInst().Subscribe(SYNC_CHAR_LAMBDA,[](char ch){
        cout<<gettid()<<":test sync topics with char arg:"<<ch<<endl;
    });
    MsgBus::GetInst().SendReq<void,char>('E',SYNC_CHAR_LAMBDA);
    //测试同步单一double参数lambda
    MsgBus::GetInst().Subscribe(SYNC_DOUBLE_LAMBDA,[](double d){
        cout<<gettid()<<":test sync topics with double arg:"<<d<<endl;
    });
    MsgBus::GetInst().SendReq<void,double>(3.14,SYNC_DOUBLE_LAMBDA);
    //测试同步单一指针参数lambda
    MsgBus::GetInst().Subscribe(SYNC_POINTER_LAMBDA,[](double* pd){
        cout<<gettid()<<":test sync topics with double pointer arg:"<<*pd<<endl;
    });
    double d1=3.14;
    MsgBus::GetInst().SendReq<void,double*>(&d1,SYNC_POINTER_LAMBDA);
    // //测试同步单一自定义类lambda
    // MsgBus::GetInst().Subscribe(SYNC_CUSTOM_CLASS_LAMBDA,[](A a){
    //     cout<<gettid()<<":test sync topics with custom class arg!"<<endl;
    // });
    //A aClass;
    // MsgBus::GetInst().SendReq<void,A>(a,SYNC_CUSTOM_CLASS_LAMBDA);      //会报错
    //测试同步单一自定义类lambda
    MsgBus::GetInst().Subscribe(SYNC_CUSTOM_CLASS_REF_LAMBDA,[](A& a){
        cout<<gettid()<<":test sync topics with custom class ref arg!"<<endl;
    });
    //MsgBus::GetInst().SendReq<void,A&>(aClass,SYNC_CUSTOM_CLASS_REF_LAMBDA);

    //异步的
    //测试异步无参lambda
    // MsgBus::GetInst().Subscribe(ASYNC_VOID_LAMBDA,[](){
    //     cout<<gettid()<<":test async topics!"<<endl;
    // });
    // MsgBus::GetInst().SendReq(ASYNC_VOID_LAMBDA);

    {
        A aClass2;
        MsgBus::GetInst().Subscribe(SYNC_CUSTOM_CLASS_LAMBDA,[](A& a){
            sleep(10);
            cout<<gettid()<<":test async topics with custom class ref arg!"<<endl;
            cout<<a.m_a<<endl;
            a.m_a=100;
        });
        MsgBus::GetInst().SendReq<void,A&>(aClass2,SYNC_CUSTOM_CLASS_LAMBDA);
        cout<<"leaving scope"<<endl;
    }
    cout<<"main is to be end"<<endl;
    sleep(20);
    return 0;
 }