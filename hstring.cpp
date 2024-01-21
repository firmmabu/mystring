#include "hstring.h"
#define N 100

/*
author: firm_mabu
date: Jan 21 2024
*/


/// @brief 计算kmp算法中子串的next数组
/// @param next next数组
void hstring::getNext(int* next) const {
    int i, j = 0;  // i是后缀末尾，j是前缀末尾
    next[0] = 0;
    for (i = 1; i < this->length; ++i) {
        while (j > 0 && buffer[i] != buffer[j])
            j = next[j - 1];
        if (buffer[i] == buffer[j])
            ++j;
        next[i] = j;
    }
}

/// @brief kmp算法查找主串中第一次出现的子串
/// @param p 子串首字符在主串中的索引
/// @param sub 子串
/// @return 是否查找到子串
bool hstring::kmp(int& p, const hstring& sub) const {
    // l1:主串长度 l2:子串长度
    int l1 = this->getLength(), l2 = sub.getLength();
    char* subBuffer = sub.getBuffer();  // 子串的缓冲区
    int* next = new int[l2];
    sub.getNext(next);  // 得到了子串的next数组
    int i = 0, j = 0;
    p = i;
    while (i >= 0 && i < l1 && p < l1 - l2) {
        if (buffer[i] == subBuffer[j]) {
            ++i;
            ++j;
        } else {
            j = j == 0 ? 0 : next[j - 1];
        }

        if (j == l2) {
            p = i - l2;
            delete[] next;
            return true;
        }
    }
    delete[] next;
    return false;
}

int hstring::getLength() const {
    return this->length;
}
int hstring::getCapacity() const {
    return this->capacity;
}
char* hstring::getBuffer() const {
    return this->buffer;
}

void hstring::setLength(int _len) {
    this->length = _len;
}
void hstring::setCapacity(int _c) {
    this->capacity = _c;
}

hstring::hstring() {
}

hstring::hstring(const char* in) {
    while (in[length])
        ++length;
    capacity = length > N ? (length / N + 1) * N : N;
    buffer = new char[capacity];
    memcpy(buffer, in, length);
}

hstring::hstring(const int num) {
    int n = num, tmp = 0;
    while (n != 0) {
        ++this->length;
        tmp = 10 * tmp + n % 10;
        n /= 10;
    }
    this->capacity = this->length > N ? (this->length / N + 1) * N : N;
    this->buffer = new char[capacity];
    int i = 0;
    while (tmp != 0) {
        this->buffer[i] = tmp % 10 + '0';
        tmp /= 10;
        ++i;
    }
}

hstring::hstring(const hstring& str) {
    this->length = str.getLength();
    this->capacity = str.getCapacity();
    this->buffer = new char[this->capacity];
    memcpy(this->buffer, str.buffer, this->length);
}

hstring::~hstring() {
    delete[] buffer;
    buffer = nullptr;
}

hstring operator+(const hstring& s1, const hstring& s2) {
    int l1 = s1.getLength(), l2 = s2.getLength(), c = s1.getCapacity();
    hstring* s3 = new hstring(s1);
    if (l1 + l2 > c) {
        int len = ((l1 + l2) / N + 1) * N;
        char* temp = new char[len];
        memcpy(temp, s1.buffer, l1);
        memcpy(temp + l1, s2.buffer, l2);
        s3->buffer = temp;
        s3->setLength(l1 + l2);
        s3->setCapacity(len);
    } else {
        memcpy(s3->buffer + l1, s2.buffer, l2);
        s3->setLength(l1 + l2);
    }
    return *s3;
}

hstring operator-(const hstring& s1, const hstring& s2) {
    hstring* s = new hstring(s1);
    int place = 0;
    char *b = s->getBuffer(), *b2 = s2.getBuffer();
    int l = s->getLength(), l2 = s2.getLength();
    if (s->kmp(place, s2)) {
        for (int i = place; i < l; ++i) {
            memset(b + i, b[i + l2], sizeof(char));
            memset(b + i + l2, 0, sizeof(char));
        }
        s->setLength(l - l2);
        return *s;
    } else {
        std::cout << "减法结果:找不到子串！" << std::endl;
    }
    return hstring("");
}

hstring hstring::operator=(const hstring& s) {
    return *new hstring(s);
}

/// @brief 实现字符串修改数据
/// @brief 例如将"123456789"中的"34"修改为"abc"得到"12abc56789"
/// @param sub 要修改的子串
/// @param replace 替代串
/// @return 是否修改成功
bool hstring::alter(const hstring& sub, const hstring& replace) {
    int place = -1;
    if (this->kmp(place, sub)) {
        int l1 = this->getLength(), l2 = sub.getLength(), l3 = replace.getLength();
        // 如果超过了缓冲区，就重新申请一块
        if (l1 + l3 - l2 > capacity) {
            int c = ((l1 - l2 + l3) / N + 1) * N;
            char* newBuffer = new char[c];
            // 原串=前面的+子串+后面的
            // 新串=前面的+替代串+后面的
            memcpy(newBuffer, this->buffer, place);                                                                // 前面的拷贝过去
            memcpy(newBuffer + place, replace.getBuffer(), l3);                                                    // 拷贝新串过去
            memcpy(newBuffer + place + l3, this->buffer + place + l2, l1 - l2 - place > 0 ? l1 - l2 - place : 0);  // 拷贝后面的过去
            delete this->buffer;
            this->buffer = newBuffer;
            this->length = l1 - l2 + l3;
            this->capacity = c;
        } else {  // 如果没有超过缓冲区
            int shift = l3 - l2;
            if (l3 > l2) {
                // 如果替代串比子串要长
                // 要将后面的整体后移，再把替代串塞进去
                for (int i = l1 - 1; i >= place + l2; --i) {
                    this->buffer[i + shift] = this->buffer[i];
                }
            } else {
                // 如果替代串比子串要短
                // 则要将后面的整体前移，再把替代串塞进去
                for (int i = place + l2; i < l1; ++i) {
                    this->buffer[i + shift] = this->buffer[i];
                }
                for (int i = l1 + shift; i < l1; ++i) {
                    this->buffer[i] = 0;
                }
            }
            memcpy(buffer + place, replace.getBuffer(), l3);
            this->length = l1 + shift;
        }
        std::cout << "修改结果:修改成功！" << std::endl;
        return true;
    } else {
        std::cout << "修改结果:未找到子串！" << std::endl;
        return false;
    }
}

/// @brief 查询子串
/// @param sub 子串
/// @return 子串首字符在主串中的索引
int hstring::query(const hstring& sub) const {
    int place = -1;
    if (this->kmp(place, sub)) {
        std::cout << "查询结果:查询成功，子串所在的位置为:" << place << std::endl;
    } else {
        std::cout << "查询结果:查询失败，不存在这个子串！" << std::endl;
    }
    return place;
}

/// @brief 打印一个字符串
void hstring::out() {
    if (length == 0) {
        std::cout << "打印结果:这是一个空字符串";
    } else {
        std::cout << "打印结果:";
        for (int i = 0; i < length; ++i) {
            std::cout << buffer[i];
        }
    }
    std::cout << std::endl;
}