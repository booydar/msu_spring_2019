
using namespace std;

#pragma once

enum class Error
{
    NoError,
    CorruptedArchive
};

struct Data
{
    uint64_t a;
    bool b;
    uint64_t c;

    template <class Serializer>    
    Error serialize(Serializer& serializer)
    {
        return serializer(a, b, c);
    }

    template <class Deserializer>
    Error deserialize(Deserializer& deserializer)
    {
        return deserializer(a, b, c);
    }
};

class Serializer
{
    static constexpr char Separator = ' ';
public:
    explicit Serializer(std::ostream& out)
        : out_(out)
    {
    }

    
    template <class T>
    Error save(T& object)
    {
        return object.serialize(*this);
    }
    
    template <class... ArgsT>
    Error operator()(ArgsT... args)
    {
        return process(args...);
    }
    
private:

    std::ostream& out_;

    template <class T>
    Error process(T&& arg)
    {
        print(arg);
        return Error::NoError;
    }

    template<class T, class... Args>
    Error process(T&& val, Args&&... args)
    {
        print(val);
        out_ << val;
        process(std::forward<Args>(args)...);
        return Error::NoError;
        
    }

    template<class T>
    void print(T& arg)
    {
        out_ << arg;
    }

    void print(bool& arg)
    {
        if(arg)
            out_ << "true";
        else
            out_ << "false";
        
    }
};


class Deserializer
{
    static constexpr char Separator = ' ';

public:
    explicit Deserializer(std::istream& in)
        : in_(in)
    {
    }
    
    template <class T>
    Error load(T& object)
    {
        return object.deserialize(*this);
    }
    
    template <class... ArgsT>
    Error operator()(ArgsT... args)
    {
        return process(args...);
    }
 
private:
    std::istream& in_;

    template <class T>
    Error process(T&& arg)
    {
        if(read(arg) == Error::CorruptedArchive)
            return Error::CorruptedArchive;
        else
            return Error::NoError;
    }

    template<class T, class... Args>
    Error process(T&& val, Args&&... args)
    {
        if (read(val) == Error::CorruptedArchive)
            return Error::CorruptedArchive;
        else
            return process(std::forward<Args>(args)...);
        
    }
    

    Error read(long unsigned int& value)
    {
        string str;
        in_ >> str;
        value = stoul(str);
        return Error::NoError;
    }

    Error read(bool& value)
    {
        string str;
        in_ >> str;
        
        if(str == "true")
            value = true;
        else if(str == "false")
            value = false;
        else
            return Error::CorruptedArchive;
        
        return Error::NoError;
    }
};