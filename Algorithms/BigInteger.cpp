#include <iostream>
#include <cstring>
 
using namespace std;
 
class bigInteger {
    friend ostream &operator<<(ostream &os, const bigInteger &obj);
    friend bigInteger operator+(const bigInteger &obj1, const bigInteger &obj2);
    friend bigInteger operator-(const bigInteger &obj1, const bigInteger &obj2);
    friend bigInteger operator*(const bigInteger &obj1, const bigInteger &obj2);
    friend bool operator==(const bigInteger &obj1, const bigInteger &obj2);
private:
    int* data; //new int[500]没法跟cin一起用...
    int length;
public:
    bigInteger() { length = 1;
        data = new int[500];
        memset(data, 0, sizeof(data));
    }
    bigInteger(const char* num);
    bigInteger(const bigInteger &obj);
    ~bigInteger() { if (data) delete[] data; } //迷之析构，薛定谔的析构
    bigInteger &operator=(const bigInteger &obj);
};

int main(void)
{
    char num1[500] = { 0 };
    char num2[500] = { 0 };
    char op;
 
    while (cin >> num1 >> op >> num2) {
        bigInteger a(num1), b(num2), c;
        switch (op) {
        case '+':
            c = a + b;
            break;
        case '-':
            c = a - b;
            break;
        case '*':
            c = a * b;
            break;
        }
        cout << c;
    }
 
    return 0;
}
 
//复制构造函数(输入>>重载函数失败的结果...)
bigInteger::bigInteger(const char* num)
{
    data = new int[500];
    memset(data, 0, sizeof(data));
    int i = 0;
    for (; num[i] != '\0'; i++) data[i] = num[i] - '0';
    length = i;
}
 
bigInteger::bigInteger(const bigInteger &obj)
{
    length = obj.length;
    memset(data, 0, sizeof(data));
    for (int i = 0; i < obj.length; i++) data[i] = obj.data[i];
}

//输出<<重载函数
ostream &operator<<(ostream &os, const bigInteger &obj)
{
    if (obj.length == 1 && obj.data[0] == 0) os << 0 << endl;
    else {
        for (int i = 0; i < obj.length; i++) os << obj.data[i];
        os << endl;
    }
    return os;
}
 
//运算符=重载函数
bigInteger &bigInteger::operator=(const bigInteger &obj)
{
    length = obj.length;
    for (int i = 0; i < length; i++) data[i] = obj.data[i];
    return *this;
}
 
//运算符+重载函数
bigInteger operator+(const bigInteger &obj1, const bigInteger &obj2)
{
    bigInteger tmp;
    int length = obj1.length > obj2.length ? obj1.length : obj2.length;
    int num1[500] = { 0 };
    int num2[500] = { 0 };
    int result[500] = { 0 };
    for (int i = 0; i < obj1.length; i++) num1[i] = obj1.data[obj1.length - 1 - i];
    for (int i = 0; i < obj2.length; i++) num2[i] = obj2.data[obj2.length - 1 - i];
 
    for (int i = 0; i < length; i++) result[i] = num1[i] + num2[i];
    for (int i = 0; i < length - 1; i++) while (result[i] >= 10) { result[i] -= 10; result[i + 1]++; }
    for (int i = 0; i < length; i++) tmp.data[i] = result[length - 1 - i];
    tmp.length = length;
    return tmp; //最高位可以是两位数，对此题没什么影响
}
 
//运算符-重载函数
bigInteger operator-(const bigInteger &obj1, const bigInteger &obj2)
{
    bigInteger tmp;
    if (obj1 == obj2) return tmp; //判断是否相等
 
    int length = obj1.length > obj2.length ? obj1.length : obj2.length;
    int num1[500] = { 0 };
    int num2[500] = { 0 };
    int result[500] = { 0 };
    for (int i = 0; i < obj1.length; i++) num1[i] = obj1.data[obj1.length - 1 - i];
    for (int i = 0; i < obj2.length; i++) num2[i] = obj2.data[obj2.length - 1 - i] * -1;
 
    for (int i = 0; i < length; i++) result[i] = num1[i] + num2[i];
    for (int i = 0; i < length - 1; i++) while (result[i] < 0) { result[i] += 10; result[i + 1]--; }
    while (result[length - 1] == 0) length--;
    //判断正负号
    if (result[length - 1] > 0) {
        for (int i = 0; i < length; i++) tmp.data[i] = result[length - 1 - i];
        tmp.length = length;
        return tmp;
    }
    else {
        for (int i = 0; i < length; i++) result[i] *= -1;
        for (int i = 0; i < length - 1; i++) if (result[i] < 0) { result[i] += 10; result[i + 1]--; }
        while (result[length - 1] == 0) length--;
        result[length - 1] *= -1;
        for (int i = 0; i < length; i++) tmp.data[i] = result[length - 1 - i];
        tmp.length = length;
        return tmp;
    }
}
 
//运算符*重载函数
bigInteger operator*(const bigInteger &obj1, const bigInteger &obj2)
{
    bigInteger tmp;
    int num1[500] = { 0 };
    int num2[500] = { 0 };
    int result[500] = { 0 };
    for (int i = 0; i < obj1.length; i++) num1[i] = obj1.data[obj1.length - 1 - i];
    for (int i = 0; i < obj2.length; i++) num2[i] = obj2.data[obj2.length - 1 - i];
 
    for (int i = 0; i < obj1.length; i++)
        for (int j = 0; j < obj2.length; j++)
            result[i + j] += num1[i] * num2[j];
    int length = 500;
    while (length > 0 && result[length - 1] == 0) length--;
    length = length + 1;
    for (int i = 1; i < length - 1; i++) {
        result[i] += result[i - 1] / 10;
        result[i - 1] %= 10;
    }
 
    if (result[0] == 0 && length == 1) return tmp; //判断是否为0
    else {
        tmp.length = length - 1;
        for (int i = 0; i < tmp.length; i++) tmp.data[i] = result[tmp.length - 1 - i];
        return tmp;
    }
}
 
//运算符==重载函数
bool operator==(const bigInteger &obj1, const bigInteger &obj2)
{
    if (obj1.length != obj2.length) return false;
    else {
        for (int i = 0; i < obj1.length; i++)
            if (obj1.data[i] != obj2.data[i]) {
                return false;
                break;
            }
    }
    return true;
}