#include <iostream>
#include <string>
#include "hstring.h"

int main() {
    std::string s1 = "aabaaf", s2 = "aabaabaaf";
    const char* c_str1 = s1.c_str();
    const char* c_str2 = s2.c_str();

    hstring str1(c_str1);
    hstring str2(c_str2);
    str1.out();
    str2.out();

    // 测试重载+
    hstring str3 = str1 + str2;
    str3.out();

    // 测试重载=
    hstring str4 = 12345678;
    str4.out();

    // 测试重载-
    hstring str5 = str2 - str1;
    str5.out();

    // 测试查询
    int place = str2.query(str1);

    // 测试修改
    str2.alter(str1, str4);
    str2.out();

    std::cout<<"我是一个joker！"<<std::endl;

    return 0;
}