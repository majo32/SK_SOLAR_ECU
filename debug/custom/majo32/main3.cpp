/* 
 * File:   main.cpp
 * Author: majo32
 *
 * Created on January 25, 2016, 1:10 PM
 */

#include <cstdlib>
#include <iostream>
#include <SunStorm/Framework.h>

#include <Services/TestService/TestService.h>

using namespace std;

class L1 : public SunStorm::Listener {

    void handle(const SunStorm::Message & message)  {
        cout << "BOOOM!" << endl;
    }
};

void a(int* c) {
    *c = *c + 1;
}

class L2 : public SunStorm::Listener {
public:
    long * l;
    int * c;

    void handle(const SunStorm::Message & message)  {
        a(c);
        if (*c == 4) {
            SunStorm::System::GetClock().StopInterval(*l);
        }
        cout << "BOOM2! " << *c << endl;
    }
};

class L3 : public SunStorm::Listener {
public:
    long * l;
    int * c;

    void handle(const SunStorm::Message & message)  {
        int c = message.GetContent<int>();
        cout << "M: " << c << endl;
    }
};

template <class T, typename... _Args>
void regThread(_Args&&... __args) {
    T* ptr = new T(std::forward<_Args>(__args)...);
    delete ptr;
}

class AA {
public:

    AA(int i, int a, std::string str) {

    }
};

int main(int argc, char** argv) {


    regThread<AA>(5, 6, "dhsfuiufh");

    SunStorm::MessengerExecutor e;
    
    SunStorm::System::Initialize();
    SunStorm::System::RegisterService<Services::TestService>("TEST_SERVICE");
    
    long l;
    int cc = 0;
    L1 l1;
    L2 l2;
    L3 l3;

    l2.l = &l;
    l2.c = &cc;

    SunStorm::System::GetClock().RegisterInterval(5000, e, l1);
    l = SunStorm::System::GetClock().RegisterInterval(3000, e, l2);

    SunStorm::System::GetMessenger().RegisterMailBox(e, l3, "T");
    SunStorm::Message m3("T");

    int int4 = 415;
    m3.SetContent(int4);
    SunStorm::System::GetMessenger().SendMessage(m3);
    std::string s;
    cout << sizeof (s) << " " << sizeof (std::string) << "long: " << sizeof (long) << std::endl;
    e.Start();
    while (1) {
        cin >> s;
        SunStorm::Message m("L1_MSG");
        m.SetContent<std::string>(s);
        cout << "Sending msg:" << s << endl;
        SunStorm::System::GetMessenger().SendMessage(m);

    }
    e.Join();

    return 0;
}

