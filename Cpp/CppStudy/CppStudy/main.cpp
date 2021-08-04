
#include "stdafx.h"


int main()
{
    std::cout << "Hello World!\n";


    staticCast_NullPtr();


    auto test = Ctest::GetInstance()->GetData();
    Ctest::Instance().method();


    std::thread t1(producer, 10);
    std::thread t2(consumer);
    t1.join();	// wait for the t1 thread to finish.
    t2.join();	// wait for the t2 thread to finish.
    std::cout << "finished!" << std::endl;


    DanglingPointer();

    return 0;
}

