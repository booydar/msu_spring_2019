#include <iostream>
#include <condition_variable>
#include <thread>
#include <mutex>

using namespace std;

mutex m;
condition_variable c;
int n=5;

void pingpong(bool& Ping, bool& Pong)
{
    for(int i=0; i<n; i++)
    {
        unique_lock<mutex> lock(m);
        if(!Ping){
            while(!Pong)
                c.wait(lock);
            cout << "pong\n";
            Ping = true;
            Pong = false;
            m.unlock();
            c.notify_one();
            continue;

        }
        if(Ping){
            while(Pong)
                c.wait(lock);
            cout << "ping\n";
            Ping = false;
            Pong = true;
            m.unlock();
            c.notify_one();
            continue;
        }        
    }
}

int main()
{
    bool Ping = true;
    bool Pong = false;
    thread ping(pingpong, ref(Ping), ref(Pong));
    thread pong(pingpong, ref(Ping), ref(Pong));
    
    ping.join();
    pong.join();
   
    return 0;
}
