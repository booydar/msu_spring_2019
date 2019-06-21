#include<iostream>
using namespace std;

[[noreturn]] void error() { throw invalid_argument("error"); }

class Row
{
private:
    int* sdata_;
    int len;

public:        
    Row(const int l, int* ptr): len(l), sdata_(ptr){};

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
};


class Matrix
{
private:
    const size_t rows;
    const size_t cols;
    int** data_;

public:
    Matrix(size_t r, size_t c)
        : rows(r), cols(c), data_(new int*[r]){
            for(int i = 0; i < rows; i++)
                data_[i] = new int[c];            
        };

    Row operator[](int row)
    {
        if(row >= rows)
            throw out_of_range("");
        return Row(cols, data_[row]);
    }
    
    const Row operator[](int row) const
    {
        if(row >= rows)
            throw out_of_range("");
        return Row(cols, data_[row]);
    }
    
    bool operator==(const Matrix& other) const      
    {
        if(this == &other)
            return true;
        
        for(int i=0; i<rows; i++)
            for(int j=0; j<cols; j++)
                if(data_[i*cols + j] != other.data_[i*cols + j])
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
            for(int j=0; j<cols; j++)
                data_[i][j] *= mult;
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
        for(int i=0; i<rows; i++)
            delete[] data_[i];
        delete[] data_;
    };
};
