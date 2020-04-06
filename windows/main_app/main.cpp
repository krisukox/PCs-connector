#include <QApplication>
#include "gui/MainWindow.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

//#include <chrono>
//#include <iostream>
//#include <thread>

// void independentThread()
//{
//    std::cout << "Starting concurrent thread.\n";
//    std::this_thread::sleep_for(std::chrono::seconds(2));
//    std::cout << "Exiting concurrent thread.\n";
//}

// void threadCaller()
//{
//    std::cout << "Starting thread caller.\n";
//    std::thread t(independentThread);
//    t.detach();
//    std::this_thread::sleep_for(std::chrono::seconds(1));
//    std::cout << "Exiting thread caller.\n";
//}

// int main()
//{
//    threadCaller();
//    std::cout << "main1\n";
//    std::this_thread::sleep_for(std::chrono::seconds(5));
//    std::cout << "main2\n";
//}
//#include <functional>
//#include <internal_types/CommonTypes.hpp>
//#include <iostream>
//#include <memory>
//#include <string>
//#include <type_traits>

// class BaseCommand
//{
// public:
//    virtual ~BaseCommand() {}
//};

// template <class T, typename = std::enable_if_t<std::is_convertible<T, internal_types::DecodedType>::value>>
// class Command : public BaseCommand
//{
//    typedef std::function<void(T)> FuncType;
//    FuncType f_;

// public:
//    Command() {}
//    Command(FuncType f) : f_(f) {}
//    void operator()(T event)
//    {
//        if (f_) f_(event);
//    }
//};

// template <class T>
// class A
//{
// public:
//    using Fun = std::function<void(T)>;

//    Fun fun;
//};

// class Print
//{
// public:
//    void print1(std::string s, std::string s1)
//    {
//        std::cout << "print1 : "
//                  << "s : " << s << " s1 : " << s1 << std::endl;
//    }
//    int print2()
//    {
//        std::cout << "print2" << std::endl;
//        return 2;
//    }
//};

// int main()
//{
//    //    std::vector<A> Print p = Print();

//    //    std::function<void()> f1(bind(&Print::print1, &p, std::string("test1"), std::string("test2")));

//    //    std::function<int()> f2(std::bind(&Print::print2, &p));

//    auto fun = [](internal_types::Event) {};
//    auto comPtr1 = new Command<internal_types::Event>(fun);
//    BaseCommand* comPtr4 = comPtr1;
//    std::unique_ptr<BaseCommand> comPtr2 = std::make_unique<Command<internal_types::Event>>(fun);
//    //    std::unique_ptr<BaseCommand> ptr3 = comPtr2;

//    std::vector<BaseCommand*> vec1;
//    vec1.push_back(comPtr1);

//    std::vector<std::unique_ptr<BaseCommand>> vec;
//    vec.push_back(comPtr2);

//    //    std::vector<std::function<void(internal_types::DecodedType)>> vec1;
//    //    std::function<void(internal_types::Event)> fun1 = [](internal_types::Event) {};
//    //    vec1.push_back(fun1);
//}
