// test_lock.cpp : 定义控制台应用程序的入口点。
/*
 *不出现死锁的模型是：lock unlock必须成对出现 ，或者是不同数据使用不同的锁。
 *出现死锁的模型
 *1：lock lock unlock unlock  
 *2：lock   return unlock
 *3：A中lock 调用B而B中也lock，
 *4：注意lock的范围
*/

#include "stdafx.h"
/*
#include <iostream>
#include <mutex>
using namespace std;

int haha = 5;

int main()
{
    std::mutex mutex1, mutex2;
    std::recursive_mutex mtx1, mtx2;

    {
        //std::unique_lock<std::mutex> lock(mutex1);
        //std::lock_guard<std::mutex> lock(mutex1);
        std::lock_guard<std::recursive_mutex> lock(mtx1);
        haha++;
        {
            std::lock_guard<std::recursive_mutex> lock2(mtx2);
            //std::lock_guard<std::mutex> lock(mutex2);
            //std::unique_lock<std::mutex> lock2(mutex1);
            haha++;
        }
    }

    cout << haha << endl;
    system("pause");
    return 0;
}*/
#include<thread>
#include<mutex>
#include <iostream>
#include<windows.h>

using namespace std;

class Test
{
private:
    std::mutex m1;
    std::mutex m2;
public:
    void fun1()
    {
        std::lock_guard<std::mutex> guard1(m1);
        //休眠,使死锁更容易发生
        Sleep(1000);
        std::lock_guard<std::mutex> guard2(m2);
    }
    void fun2()
    {
        std::lock_guard<std::mutex> guard1(m2);
        //休眠,使死锁更容易发生
        Sleep(1000);
        std::lock_guard<std::mutex> guard2(m1);
    }

};
void fun1(Test *p)
{
    p->fun1();
}
void fun2(Test *p)
{
    p->fun2();
}
int main()
{
    Test t;
    std::thread A(fun1, &t);
    std::thread B(fun2, &t);
    A.join();
    B.join();
    cout << "haha" << endl;
    return 0;
}
