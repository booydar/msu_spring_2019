#include<iostream>
#include<string>

using namespace std;

class Result
{
    public: Result(double v, string r)
		: acc(v), rest(r) {}

	double acc;
    string rest;
};

class parcerpm
{
	public: parcerpm(){}
	
	public:
		double parce(string s)
		{
			for(int i=0; i<s.length(); i++)
				if(!(isdigit(s[i]) || s[i]=='-' || s[i]=='+' || s[i]=='*' || s[i]=='/' || s[i]==' '))
				{
					cout << "error";
					return -1;
				}
			Result result = pm(s);
			if(result.rest.length() != 0)
				cout << "error !" << result.rest << "!";
			return result.acc;
		};

	private: 
		Result pm(string s)
		{
			while(s[0] == ' ')
				s = s.substr(1);
			
			Result current = term(s);
			double acc = current.acc;
			
			while(current.rest[0] == ' ')
				current.rest = current.rest.substr(1);

			while(current.rest.length() > 0)
			{
				
				if(!(current.rest[0] == '-' || current.rest[0] == '+'))
					break;

				string next = current.rest.substr(1);
				acc = current.acc;
				char sign = current.rest[0];

				current = term(next);
				if(sign == '+')
					acc += current.acc;
				else if(sign == '-')
					acc -= current.acc;
				current.acc = acc;				

			}
			return Result(current.acc, current.rest);
		};

		Result Num(string s)
		{
			int i = 0;
			bool negative = false;
			while(s[0] == ' ')
				s = s.substr(1);

			if(s[0] == '-')
			{
				negative = true;
				s = s.substr(1);
			}

			while(i < s.length() && isdigit(s[i]))
				i++;
			double num = stod(s.substr(0,i));
			if(negative)
				num = -num;
			string rest = s.substr(i);
			while(s[0] == ' ')
				s = s.substr(1);
			
			return Result(num, rest);
		}

        Result term(string s)
        {
            int i=0;
            double num1=0, num2=0;
			Result current = Num(s);
            num1 = current.acc;
            s = current.rest;
			while(s[0] == ' ')
				s = s.substr(1);
			
			if(s[0] == '*' || s[0] == '/')
				while(s.length() > 0)
				{
					if(isdigit(s[0]) || s[0] == '+' || s[0] == '-')
						break;
					
					while(s[0] == ' ')
						s = s.substr(1);

					if(s[0] == '*')
					{
						s = s.substr(1);
						Result curr = Num(s);
						num1 = num1 * curr.acc;
						s = curr.rest;
					}

					if(s[0] == '/')
					{
						s = s.substr(1);
						Result curr = Num(s);
						num1 = num1 / curr.acc;
						s = curr.rest;
					}
				}
			                       
            return Result(num1, s);
        }


};



int main(int argc, char** argv)
{
	string str = argv[1];
	parcerpm pm;

	cout << pm.parce(str);
	return 0;	
}


