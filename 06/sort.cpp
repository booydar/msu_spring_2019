#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <mutex>
#include <condition_variable>
#include <thread>
#include<exception>

using namespace std;

size_t max_size = 1000;
//size_t max_size = 8E6/sizeof(uint64_t);

mutex m;
condition_variable c;


istream& ipt(std::istream& in, uint64_t& u) {
    in.read((char*) &u, sizeof(uint64_t));
}

ostream& opt(std::ostream& out, const uint64_t& u) {
    out.write((char*) &u, sizeof(uint64_t));
}

void split(ifstream& in, size_t size, ofstream& out1, ofstream& out2, size_t& i)
{
    uint64_t a;

    while(true){      
        unique_lock<mutex> lock(m);
        
        in.seekg(i*sizeof(uint64_t));
        ipt(in, a);
        
        if(i < max_size)
            opt(out1,a);
        else 
            opt(out2,a);
        i++;

        if(in.peek() == EOF || i == size-1){
            m.unlock();
            break;
        }

    }
}

void sort(string& input_name, size_t size, string& output_name)
{
    ifstream in(input_name, ios::binary | ios::in);
    if(!in)
        throw runtime_error("");
    
    vector<uint64_t> a(size);

    for(int i=0; i<size; i++){
        in.seekg(i*sizeof(uint64_t));
        ipt(in, a[i]);        
    }
    
    in.close();

    std::sort(a.begin(), a.end());

    ofstream out(output_name, ios::binary | ios::out);
    if(!out)
        throw runtime_error("");

    for(int i=0; i<size; i++)
        opt(out, a[i]);

    out.close();
}

void merge_files(ifstream& in, ofstream& out, uint64_t& bar)
{
    uint64_t a;

    while(1){
        unique_lock<mutex> lock(m);

        if(in.peek() == EOF){
            if(bar != -1){                
                opt(out, bar);
                bar = -1;
            }
            c.notify_one();
            break;
        }
        ipt(in, a);
        
        if(a == bar)
            continue;

        if(bar == -1){            
            opt(out, a);
            continue;
        }
        if(a < bar)            
            opt(out, a);
        else{            
            opt(out, bar);
            bar = a;
            c.notify_one();
            if(bar==a)
                c.wait(lock);
        }        
    }
}


int iter=0;
void merge_sort(string& filename)
{
    size_t size = 0;
    uint64_t bar;

    string output_name = filename;
    
    if(iter == 0)
        output_name = "output.bin";
    iter++;

    //find size of file    
    uint64_t a = 0;
    
    ifstream in(filename, ios::binary | ios::in);
    
    while(ipt(in,a))
        size++;
    
    if (size <= max_size)
        sort(filename, size, output_name);
    else{
        
        string fn1 = to_string(iter) + "1.bin";
        string fn2 = to_string(iter) + "2.bin";
        
        ifstream in(filename, ios::binary | ios::in);
        ofstream out1(fn1, ios::binary | ios::out);
        ofstream out2(fn2, ios::binary | ios::out);
        if(!(in && out1 && out2))
            throw runtime_error("");

        size_t pos=0;
        thread s1(split, ref(in), size, ref(out1), ref(out2), ref(pos));
        thread s2(split, ref(in), size, ref(out1), ref(out2), ref(pos));

        s1.join();
        s2.join();
        in.close();
        out1.close();
        out2.close();

        merge_sort(ref(fn1));
        merge_sort(ref(fn2));
        
        ifstream in1(fn1, ios::binary | ios::in);
        ifstream in2(fn2, ios::binary | ios::in);
        ofstream out(output_name, ios::binary | ios::out); 
        if(!(in1 && in2 && out))
            throw runtime_error("");
        
        ipt(in2,bar);
        thread m1(merge_files, ref(in1), ref(out), ref(bar));
        thread m2(merge_files, ref(in2), ref(out), ref(bar));
        
        m1.join();
        m2.join();
        in1.close();
        in2.close();
        out.close();

        const char* file1 = fn1.c_str();
        const char* file2 = fn2.c_str();
        std::remove(file1);
        std::remove(file2);
    }    
}

int main()
{
    string fn = "input.bin";

    try {
        merge_sort(fn);
    } catch(const runtime_error&) {
        cout << "could not open file";
        return -1;
    } catch(...) {
        cout << "other error occured";
        return -2;
    }
    
    return 0;
}
