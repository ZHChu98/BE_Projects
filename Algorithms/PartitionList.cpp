#include <iostream>
#include <stdio.h>

using namespace std;

template <class T>
class ListNode {
    public:
      T data;
      ListNode *next;
      ListNode() { data = 0; next = NULL; }
      ListNode(T x) { data = x; next = NULL; }
};

template <class T>
class List {
    private:
      ListNode<T> *head;
      ListNode<T> *tail;
      int length;
      void init() {
          head = new ListNode<T>;
          tail = head;
          length = 0;
      }
      void removeAll();
    
    public:
      List() { init(); }
      ~List() { removeAll(); }
      void clear() { removeAll(); init(); }
      void pushBack(T x);
      void printAll();
      void partition(T x);
};

template <class T>
void List<T>::removeAll()
{
    ListNode<T> *tmp;
    while (head != NULL) {
        tmp = head;
        head = head->next;
        delete tmp;
    }
    tail = NULL;
}

template <class T>
void List<T>::pushBack(T x)
{
    tail->next = new ListNode<T>(x);
    tail = tail->next;
    length++;
}

template <class T>
void List<T>::printAll()
{
    ListNode<T> *tmp = head->next;
    for (int i = 0; i < length - 1; i++) {
        cout << tmp->data << ' ';
        tmp = tmp->next;
    }
    cout << tmp->data << endl;
}

template <class T>
void List<T>::partition(T x)
{
    ListNode<T> *curr = head->next;
    ListNode<T> *prev = head;
    ListNode<T> *tmp;
    for (int i = 0; i < length; i++) {
        if (curr->data < x) {
            prev = curr;
            curr = curr->next;
        }
        else {
            pushBack(curr->data);
            tmp = curr;
            prev->next = curr->next;
            curr = prev->next;
            delete tmp;
            length--;
        }
    }
}

int main()
{
    int N;
    cin >> N;
    getchar();
    List<int> L;
    for (int i = 1; i <= N; i++) {
        int x;
        char c = ' ';
        while (c != '\n') {
            cin >> x;
            L.pushBack(x);
            c = getchar();
        }
        cin >> x;
        getchar();
        L.partition(x);
        L.printAll();
        L.clear();
    }

    return 0;
}