#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <mutex>
#include <condition_variable>
#include <thread>

using namespace std;

size_t max_size = 1000;
//size_t max_size = 8E6/sizeof(uint64_t);

mutex m;
condition_variable c;

void merge_sort(string);
void split(ifstream&, size_t, ofstream&, ofstream&); //split file into out1 and out2
void sort(string input_name, size_t size, string output_name); //simple sort file with small enough size
void merge_(ifstream&, ofstream&, int64_t&); // merge sorted files

int main()
{
    merge_sort("input.bin");

    return 0;
}

void split(ifstream& in, size_t size, ofstream& out1, ofstream& out2) 
{
    uint64_t a;
    int i=0;

    while(1){
        m.lock();
        if(!(in >> a)){
            m.unlock();
            break;
        }
        if(i < size)
            out1 << a << ' ';
        else
            out2 << a << ' ';   
        i++;     
        m.unlock();
    }
}

void merge_(ifstream& in, ofstream& out, int64_t& bar)
{
    int64_t a;
    int it;

    while(1){
        unique_lock<mutex> lock(m);

        if(!(in >> a)){
            if(bar != -1){
                out << bar << ' ';
                bar = -1;
            }
            c.notify_one();
            break;
        }
        if(bar == -1){
            out << a << ' ';
            continue;
        }
        if(a <= bar)
            out << a << ' ';
        else{
            out << bar << ' ';
            bar = a;
            c.notify_one();
            if(bar==a)
                c.wait(lock);
        }        
    }
}


int iter=0;
void merge_sort(string filename)
{
    size_t size = 0;
    int64_t bar;

    string output_name = filename;
    
    if(iter == 0)
        output_name = "output.bin";
    iter++;

    //find size of file
    uint64_t a;

    ifstream in(filename, ios::binary | ios::in);
    while(in >> a)
        size++;
    
    if (size <= max_size)
        sort(filename, size, output_name);
    else{
        
        string fn1 = to_string(iter) + "1.bin";
        string fn2 = to_string(iter) + "2.bin";
        
        ifstream in(filename, ios::binary | ios::in);
        ofstream out1(fn1, ios::binary | ios::out);
        ofstream out2(fn2, ios::binary | ios::out);

        thread s1(split, ref(in), max_size, ref(out1), ref(out2));
        thread s2(split, ref(in), max_size, ref(out1), ref(out2));

        s1.join();
        s2.join();
        in.close();
        out1.close();
        out2.close();

        merge_sort(fn1);
        merge_sort(fn2);
        
        ifstream in1(fn1, ios::binary | ios::in);
        ifstream in2(fn2, ios::binary | ios::in);
        ofstream out(output_name, ios::binary | ios::out);            
        
        in2 >> bar;
        thread m1(merge_, ref(in1), ref(out), ref(bar));
        thread m2(merge_, ref(in2), ref(out), ref(bar));
        
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


void sort(string input_name, size_t size, string output_name)
{
    ifstream in(input_name, ios::binary | ios::in);
    
    vector<uint64_t> a(size);

    for(int i=0; i<size; i++)
        in >> a[i];
    
    in.close();

    std::sort(a.begin(), a.end());

    ofstream out(output_name, ios::binary | ios::out);

    for(int i=0; i<size; i++)
        out << a[i] << ' ';

    out.close();
}
