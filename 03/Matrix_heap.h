#include<iostream>
using namespace std;

[[noreturn]] void error() { throw invalid_argument("error"); }

class Row
    {
    public:
        Row(const int l): len(l), sdata_(new int(l)){
            for(int i=0; i<l; i++)
                sdata_[i] = 0;
        };
        
        int* sdata_;
        int len;
        
        int& operator[](size_t pos)
        {
            if(pos >= len)
                throw out_of_range("");
            return sdata_[pos];
        }
        
        const int& operator[](size_t pos) const
        {
            if(pos >= len)
                throw out_of_range("");
            return sdata_[pos];
        }
        
        Row& operator *= (int mult)
        {
            for(int i=0; i<len; i++)
                sdata_[i] *= mult;
        }
        bool operator == (Row& other)
        {
            for(int i=0; i<len; i++)
                if(sdata_[i] != other.sdata_[i])
                    return false;
            return true;
        }
        bool operator != (Row& other)
        {
            return !(*this == other);
        }
        
    };

class Matrix
{
public:
    Matrix(size_t r, size_t c)
        : rows(r), cols(c), data_(new Row*[r]){
            for(int i = 0; i < rows; i++)
                data_[i] = new Row(c);
        };

    const size_t rows;
    const size_t cols;
    Row** data_;

public:
    Row operator[](int row)
    {
        if(row >= rows)
            throw out_of_range("");
        return *data_[row];
    }
    
    const Row operator[](int row) const
    {
        if(row >= rows)
            throw out_of_range("");
        return *data_[row];
    }
    
    bool operator==(const Matrix& other) const      
    {
        
        if(this == &other)
            return true;
        
        for(int i=0; i<rows; i++)
            if(data_[i] != other.data_[i])
                return false;
        return true;            
    }

    bool operator!=(const Matrix& other) const
    {
        return !(*this == other);
    }
    
    Matrix& operator*=(int mult)
    {
        for(int i=0; i<rows; i++)
            *data_[i] *= mult;
        return *this;
    }

    size_t getRows()
    {
        return rows;
    }

    size_t getColumns()
    {
        return cols;
    }
    ~Matrix()
    {
        delete data_;
    };
    
};
