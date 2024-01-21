#include <iostream>

#ifndef hstring_h
#define hstring_h

class hstring {
   private:
    char* buffer;
    int length = 0;
    int capacity = 0;
    void getNext(int*) const;
    bool kmp(int& p, const hstring& sub) const;

   public:
    // getter
    int getLength() const;
    int getCapacity() const;
    char* getBuffer() const;
    // setter
    void setLength(int);
    void setCapacity(int);

   public:
    hstring();
    hstring(const char*);
    hstring(const int);
    hstring(const hstring&);
    ~hstring();

    friend hstring operator+(const hstring& s1, const hstring& s2);
    friend hstring operator-(const hstring& s1, const hstring& s2);
    hstring operator=(const hstring& s);
    bool alter(const hstring& sub, const hstring& replace);
    int query(const hstring& sub) const;

    void out();
};

#endif