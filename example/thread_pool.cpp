// thread_pool.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "thread_pool.hpp"

using namespace std::chrono_literals;

void run()
{
    thread_pool::pool pl{};

    {
        thread_pool::schedule(pl, []
            {
                std::cout << "[" << std::this_thread::get_id() << "]" << "hello world -- 1" << std::endl;
            });

        //std::this_thread::sleep_for(2s);
        // pl.schedule([] { std::cout << "hello world -- 1" << std::endl; });

        //pl.schedule([] { std::cout << "hello world -- 2" << std::endl; });
        thread_pool::schedule(pl, []
            {
                int i = 20;
                while(i--)
                {
                    std::cout << "[" << std::this_thread::get_id() << "]" << "hello world -- 2" << std::endl;
                    //std::this_thread::sleep_for(10s);
                }
            });

        //std::this_thread::sleep_for(2s);
        //pl.schedule([] { std::cout << "hello world -- 3" << std::endl; });
       thread_pool::schedule(pl, []
            {
                std::cout << "[" << std::this_thread::get_id() << "]" << "hello world -- 3" << std::endl;
            });

        //std::this_thread::sleep_for(2s);

        //pl.schedule([] { std::cout << "hello world -- 4" << std::endl; });
        thread_pool::schedule(pl, []
            {
                std::cout << "[" << std::this_thread::get_id() << "]" << "hello world -- 4" << std::endl;
            });

        //std::this_thread::sleep_for(2s);

        //pl.schedule([] { std::cout << "hello world -- 5" << std::endl; });
        thread_pool::schedule(pl, []
            {
                std::cout << "[" << std::this_thread::get_id() << "]" << "hello world -- 5" << std::endl;
            });

        //std::this_thread::sleep_for(2s);

        //pl.schedule([] { std::cout << "hello world -- 6" << std::endl; });
        thread_pool::schedule(pl, []
            {
                std::cout << "[" << std::this_thread::get_id() << "]" << "hello world -- 6" << std::endl;
            });
    }

    //pl.close_thread();
}


int main()
{
 
    std::thread t(run);
    t.join();

    std::cin.get();
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
