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
#include <Common/Container/TestContainer.h>
/*using namespace std;

class Th : public SunStorm::Thread {
public:
    SunStorm::Clock & c;

    Th(SunStorm::Clock & c) :
    Thread(), c(c) {
    }

    void Run() {
        while (1) {
            cout << "Tick" << endl;
            c.Tick();
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
};

class L1 : public SunStorm::Listener {

    void handle(const SunStorm::Message & message) const {
        cout << "BOOOM!" << endl;
    }
};
void a(int* c){
    *c = *c +1;
}
class L2 : public SunStorm::Listener {
public:
    long * l;
    SunStorm::Clock * cl;
    int * c;

    void handle(const SunStorm::Message & message) const {
        a(c);
        if(*c == 4){
            cl->StopInterval(*l);
        }
        cout << "BOOM2! " << *c << endl;
    }
};
class L3 : public SunStorm::Listener {
public:
    long * l;
    SunStorm::Clock * cl;
    int * c;

    void handle(const SunStorm::Message & message) const {
        int c = message.GetContent<int>();
        cout << "M: " << c << endl;
    }
};

int main(int argc, char** argv) {
    
  
    
    
    SunStorm::MessengerExecutor e;
    SunStorm::Messenger mg;

    Common::Container::TestContainer tc;
    tc.RegisterService(new Services::TestService("TEST_SERVICE"));
    
    
    SunStorm::Clock c;
    long l ;
    int cc = 0;
    Th th(c);
    L1 l1;
    L2 l2;
    L3 l3;
    l2.cl = &c;
    l2.l = &l;
    l2.c = &cc;
    th.Start();
    c.RegisterInterval(5000, e,l1)  ;
    l = c.RegisterInterval(3000, e,l2)  ;
    
    SunStorm::System::GetMessenger().RegisterMailBox(e,l3,"T");
    SunStorm::Message m3("T");
    
    int int4 = 415;
    m3.SetContent(int4);
    SunStorm::System::GetMessenger().SendMessage(m3);
    std::string s;
    cout << sizeof (s) << " " << sizeof (std::string);
    e.Start();
    while (1) {
        cin >> s;
        SunStorm::Message m("L1_MSG");
        m.SetContent<std::string>(s);
        cout << "Sending msg:" << s << endl;
        SunStorm::System::GetMessenger().SendMessage(m);

    }
    e.Join();
    th.Join();
    return 0;
}

*/