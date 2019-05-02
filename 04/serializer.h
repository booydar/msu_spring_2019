using namespace std;
#pragma once

enum class Error
{
    NoError,
    CorruptedArchive
};

class Serializer
{
    static constexpr char Separator = ' ';

public:
    explicit Serializer(ostream& out)
        : out_(out)
    {
    }
    
    template <class T>
    Error save(T& object)
    {
        return object.serialize(*this);
    }
    
    template <class... ArgsT>
    Error operator()(ArgsT&&... args)
    {
        return process(forward<ArgsT>(args)...);
    }
    
private:
    std::ostream& out_;

    template <class T>
    Error process(T&& arg)
    {
        print(forward<T>(arg));
        return Error::NoError;
    }

    template<class T, class... Args>
    Error process(T&& val, Args&&... args)
    {
        print(val);
        out_ << Separator;
        return process(forward<Args>(args)...);       
    }

    template<class T>
    void print(T&& arg)
    {
        out_ << arg;
    }

    void print(bool arg)
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
        return object.serialize(*this);
    }
    
    template <class... ArgsT>
    Error operator()(ArgsT&&... args)
    {
        return process(forward<ArgsT>(args)...);
    }
 
private:
    std::istream& in_;

    template <class T>
    Error process(T&& arg)
    {
        if(read(forward<T>(arg)) == Error::NoError)
            return Error::NoError;
        else
            return Error::CorruptedArchive;
    }

    template<class T, class... Args>
    Error process(T&& val, Args&&... args)
    {
        if (read(forward<T>(val)) == Error::CorruptedArchive)
            return Error::CorruptedArchive;
        else
            return process(forward<Args>(args)...);
    }
    

    Error read(uint64_t& value)
    {
        string str;
        in_ >> str;
        if(str[0]=='-')
            return Error::CorruptedArchive;
        try {
            value = std::stoul(str);
            return Error::NoError;
        }
        catch (std::invalid_argument& e) { 
            return Error::CorruptedArchive; 
        }
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
