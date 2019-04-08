//oj改成支持C++11吧，不然所有nullptr都还得专门改成NULL才能提交
#include <iostream>

using namespace std;

class node {
    public:
      int num = 0;
      node* next = nullptr;
      node(int x) { num = x; }
};

class ArrayList {
    friend ostream &operator<<(ostream &os, const ArrayList &obj);
    friend ArrayList operator+(const ArrayList &obj1, const ArrayList &obj2);

  private:
    node *start;
    node *end;
    node *tmp;

  public:
    ArrayList() {
        start = new node(0);
        end = start;
    }
    ArrayList(const char* num);
    ~ArrayList() = default;
};

int main()
{
    int n;
    cin >> n;
    char num1[200] = { 0 };
    char num2[200] = { 0 };
    for (int i = 1; i <= n; i++) {
        cin >> num1 >> num2;
        ArrayList n1(num1), n2(num2), n3;
        n3 = n1 + n2;
        cout << n3;
    }
    return 0;
}

ArrayList::ArrayList(const char* num)
{
    start = new node(0);
    end = start;
    int index = 0;
    while (num[index] != '\0') {
        end->next = new node(num[index] - '0');
        end = end->next;
        index++;
    }
}

ostream &operator<<(ostream &os, const ArrayList &obj)
{
    node *tmp = obj.start->next;
    while (tmp != nullptr) {
        os << tmp->num;
        tmp = tmp->next;
    }
    os << endl;
    return os;
}

ArrayList operator+(const ArrayList &obj1, const ArrayList &obj2)
{
    ArrayList tmpArr;
    node *tmp = tmpArr.start;
    node *trav = obj1.start->next;
    while (trav != nullptr) {
        tmp->next = new node(trav->num);
        tmp = tmp->next;
        trav = trav->next;
    }
    trav = obj2.start->next;
    while (trav != nullptr) {
        tmp->next = new node(trav->num);
        tmp = tmp->next;
        trav = trav->next;
    }
    tmpArr.end = tmp;
    return tmpArr;
}