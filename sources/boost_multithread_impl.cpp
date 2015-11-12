// ProgressDisplay.cpp : 定义控制台应用程序的入口点。
// 由于Boost.Threads采用了函数对象，而不是函数指针，因此函数对象携带线程要用到的数据是完全可以的。这种方法更加灵活，而且类型安全。
// 如果再和相关的函数库结合起来，比如Boost.Bind，这种方式可以让你轻松地将任意多的数据传递给新创建的线程。

#include "stdafx.h"
#include <boost/thread/thread.hpp>
#include <iostream>
#include <windows.h>
void hello()
{
    std::cout << "Hello world, I'm a thread!" << std::endl;
}
int main(int argc, char* argv[])
{
    boost::thread thrd(hello);        // 译注：hello前面的&符号，可要可不要
    thrd.join();

    boost::thread thrd2(hello);
    thrd2.join();

    boost::thread thrd3(hello);
    thrd3.join();

    while (1)
    {
        std::cout << "loop....." << std::endl;
        Sleep(1000);
    }
    return 0;
}


// 通过struce 构造 boost::thread对象
/*
boost::mutex io_mutex;
struct count
{
    count(int id) : id(id) { }
    void operator()()
    {
        for (int i = 0; i < 10; ++i)
        {
            boost::mutex::scoped_lock lock(io_mutex);
            std::cout << id << ": " << i << std::endl;
        }
    }
    int id;
};

int main(int argc, char* argv[])
{
    boost::thread thrd1(count(1));
    boost::thread thrd2(count(2));
    thrd1.join();
    thrd2.join();

    return 0;
}*/


// 通过Boost::Bind 构造 boost::thread对象
/*
boost::mutex io_mutex;
void count(int id)
{
    for (int i = 0; i < 10; ++i)
    {
        boost::mutex::scoped_lock lock(io_mutex);
        std::cout << id << ": " << i << std::endl;
    }
}

int main(int argc, char* argv[])
{
    boost::thread thrd1(boost::bind(&count, 1));    // 有无&符号均可
    boost::thread thrd2(boost::bind(&count, 2));    // 有无&符号均可
    thrd1.join();
    thrd2.join();

    return 0;
}*/
