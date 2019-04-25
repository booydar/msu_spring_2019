#include <iostream>
#include <condition_variable>
#include <thread>
#include <mutex>

using namespace std;

int main()
{

    mutex m;
    condition_variable c;
    bool Ping = true;
    //int n=500000;
    int n = 10;

    thread ping([&]()
    {
        unique_lock<mutex> lock(m);
        for(int i=0; i<n; i++)
        {
            while(!Ping)
                c.wait(lock);
            cout << "ping\n";
            Ping = false;
            c.notify_one();
        }
    }); 

    thread pong([&]()
    {
        unique_lock<mutex> lock(m);
        for(int i=0; i<n; i++)
        {
            while(Ping)
                c.wait(lock);
            cout << "pong\n";
            Ping = true;
            c.notify_one();
        }
    });
    
    ping.join();
    pong.join();
   
   return 0;
}
