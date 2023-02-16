#include <iostream>
using namespace std;

#define DIVIDER "===========" << endl

class String
{
protected:
	char* str = nullptr;
	unsigned int len = 0;
public:
	String(String& s): str(s.str), len(s.len)
	{
		cout << "Shallow construct" << endl;
	}

	String(String&& s): str(s.str), len(s.len)
	{
		cout << "Shallow copy" << endl;
	}

	String(const unsigned int len)
	{
		*this = new char[(this->len = len) + 1];
		(*this)[len] = '\0';
	}

	String(const char chr)
	{
		*this = new char[2] {chr};
	}

	String(const char str[])
	{
		*this = str;
	}

	~String()
	{
		if (this->str)
		{
			delete this->str;
			this->str = nullptr;
			this->len = 0;
		}
	}

	unsigned int length()
	{
		return this->len;
	}

	// --------
	// Operators
	operator char* ()
	{
		return this->str;
	}

	String& operator=(String&& str) noexcept
	{
		this->len = str.len;
		this->str = str.str;

		cout << "MoveAssignment" << endl;

		return *this;
	}

	String& operator=(const char str[])
	{
		this->str = new char[(this->len = strlen(str)) + 1];

		strcpy_s(*this, this->length() + 1, str);
		return *this;
	}

	String& operator+=(const char str[])
	{
		char* newStr = new char[this->len + strlen(str)];
		
		strcpy_s(newStr, this->length() + 1, *this);
		strcpy_s(newStr + this->length(), strlen(str) + 1, str);

		return *this = newStr;
	}

	// ----------
	// Out of bounds
	String& substr(unsigned int start, unsigned int end = 0)
	{
		if (!end) {
			end = this->length();
		}

		if (start > end)
		{
			// https://peterforgacs.github.io/2017/06/25/Custom-C-Exceptions-For-Beginners/
			throw exception("substr::start larger than substr::end");
		}

		const unsigned int STR_SIZE = end - start + 1;

		String& srcStr = *this;
		String* newStr = new String(STR_SIZE);

		for (int i = 0; i < STR_SIZE; i++)
		{
			(*newStr)[i] = (srcStr + start)[i];
		}

		return *newStr;
	}

	String& substr(int start, int end = 0)
	{
		unsigned int realStart = start, realEnd = end;

		if (start < 0)
		{
			realStart = this->length() + start;
		}

		if (end < 0)
		{
			realEnd = this->length() + end;
		}

		return this->substr(realStart, realEnd);
	}
};

// In case: str1 not null terminated
String& operator+(const char chr, String& str2)
{
	String *newStr = new String(new char[2] { chr });
	return *newStr += str2;
}

// In case: str2 not null terminated
String& operator+(String& str1, const char chr)
{
	String *newStr = new String(str1);
	return *newStr += new char[2] { chr };
}

// In case: str1 null terminated
String& operator+(String& str1, String& str2)
{
	String *newStr = new String(str1);
	return *newStr += str2;
}

// In case: str1 null terminated
String& operator+(const char str1[], String& str2)
{
	String *newStr = new String(str1);
	return *newStr += str2;
}

// In case: str2 null terminated
String& operator+(String& str1, const char str2[])
{
	String *newStr = new String(str1);
	return *newStr += str2;
}

void outOfBounds()
{
	String str = "Some string";

	// Unsigned int
	cout << "Unsigned int:" << endl;
	cout 
		<< str.substr(5) << endl
		<< str.substr(5, 7) << endl;

	cout << DIVIDER;

	// Singed int
	cout << "Signed int:" << endl;
	cout 
		<< str.substr(-4) << endl
	    << str.substr(-4, -1) << endl;
}

int main()
{
	String str1 = "Hello";
	cout << str1 << endl;

	String str2 = "World";
	cout << str2 << endl;

	// Not null terminated
	String str3 = str1 + ' ' + str2 + ' ' + '(' + 'N'+'o'+'t' + ' ' + 'n'+'u'+'l'+'l' + ' ' + 't'+'e'+'r'+'m'+'i'+'n'+'a'+'t'+'e'+'d' + ')';
	cout << str3 << endl;

	// Null terminated
	String str4 = str2 + " " + str1 + " (Null terminated)";
	cout << str4 << endl;

	// Просто балуюсь с кодом
	//outOfBounds();
}