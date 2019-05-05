#include <iostream>
#include <condition_variable>
#include <thread>
#include <mutex>

using namespace std;

mutex m;
condition_variable c;
int n=5;

void pingpong(bool& Ping)
{
    for(int i=0; i<n; i++)
    {
        lock_guard<mutex> lock(m);
        if(!Ping){
            cout << "pong\n";
            Ping = true;
            m.unlock();
            c.notify_one();
            continue;

        }
        if(Ping){
            cout << "ping\n";
            Ping = false;
            m.unlock();
            c.notify_one();
            continue;
        }        
    }
}

int main()
{
    bool Ping = true;

    thread ping(pingpong, ref(Ping));
    thread pong(pingpong, ref(Ping));
    
    ping.join();
    pong.join();
   
    return 0;
}
