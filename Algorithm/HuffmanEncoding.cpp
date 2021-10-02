#include <iostream>
#include <string>
#include <map>
#include <list>
#include <stdio.h>
#include <algorithm>

using namespace std;

class HuffmanNode {
    public:
        HuffmanNode(const char c, const int f) {
            freq = f;
            char_list.insert(char_list.begin(), c);
        }
        ~HuffmanNode();
        int freq;
        list<char> char_list;
};

class HuffmanNode_comp {
    public:
        bool operator() (const HuffmanNode n1, const HuffmanNode n2) { return n1.freq > n2.freq; }
};

class HuffmanEncoding {
    private:
        list<HuffmanNode> node_list;
        int sen_len; // total number of input sentence
        int encoded_bit; // total bits of output after Huffman encoding

    public:
        HuffmanEncoding(string str) {
            sen_len = 0;
            encoded_bit = 0;
            read(str);
            node_list.sort(HuffmanNode_comp()); // bubble sort
            build();
        }
        ~HuffmanEncoding();
        void read(string str); // store the characters' data in node_list and Huffman_table
        void swap(list<HuffmanNode>::iterator it1, list<HuffmanNode>::iterator it2);
        void build(); // calculate the Huffman_table
        void display(); // this function is determined by the purpose
        map<char, string> Huffman_table; // store every character with its Huffman code
};

HuffmanNode::~HuffmanNode()
{
    for (list<char>::iterator it = char_list.begin(); it != char_list.end();)
        char_list.erase(it++);
}

HuffmanEncoding::~HuffmanEncoding()
{
    for (list<HuffmanNode>::iterator it = node_list.begin(); it != node_list.end();)
        node_list.erase(it++);
    for (map<char, string>::iterator it = Huffman_table.begin(); it != Huffman_table.end();)
        Huffman_table.erase(it++);
}

void HuffmanEncoding::read(string str)
{
    map<char, int> tot;
    map<char, int>::iterator it;
    for (string::iterator i = str.begin(); i != str.end(); i++) {
        if ((it = tot.find(*i)) == tot.end())
            tot.insert(pair<char, int>(*i, 1));
        else
            it->second += 1;
    }
    for (it = tot.begin(); it != tot.end(); it++) {
        Huffman_table.insert(pair<char, string>(it->first, {}));
        node_list.insert(node_list.end(), HuffmanNode(it->first, it->second));
    }
    sen_len = str.length();
}

void HuffmanEncoding::swap(list<HuffmanNode>::iterator it1, list<HuffmanNode>::iterator it2)
{
    int freq_tmp = it1->freq;
    list<char> char_list_tmp(it1->char_list);
    it1->freq = it2->freq;
    it1->char_list = it2->char_list;
    it2->freq = freq_tmp;
    it2->char_list = char_list_tmp;
}

void HuffmanEncoding::build()
{
    list<char>::iterator it_char;
    map<char, string>::iterator it_table;
    list<HuffmanNode>::iterator it1, it2;
    list<HuffmanNode> node_list_tmp(node_list);
    while (node_list_tmp.begin() != (--node_list_tmp.end())) {
        it1 = it2 = (--node_list_tmp.end());
        it1--;
        for (it_char = (*it1).char_list.begin(); it_char != (*it1).char_list.end(); it_char++) {
            it_table = Huffman_table.find(*it_char);
            (it_table->second).insert(0, "0");
        }
        for (it_char = (*it2).char_list.begin(); it_char != (*it2).char_list.end(); it_char++) {
            it_table = Huffman_table.find(*it_char);
            (it_table->second).insert(0, "1");
        }
        (*it1).freq += (*it2).freq;
        (*it1).char_list.merge((*it2).char_list);
        node_list_tmp.pop_back();
        it2 = it1--;
        while (it2 != node_list_tmp.begin() && it2->freq >= it1->freq) {
            swap(it1, it2);
            it2 = it1--;
        }
    }
}

void HuffmanEncoding::display()
{
    double encode_ratio;
    map<char, string>::iterator it_table;
    list<HuffmanNode>::iterator it;
    for (it_table = Huffman_table.begin(); it_table != Huffman_table.end(); it_table++) {
        it = node_list.begin();
        while (it != node_list.end() && *((it->char_list).begin()) != it_table->first)
            it++;
        encoded_bit += (it_table->second).length() * it->freq;
    }
    encode_ratio = (sen_len * 8.0) / encoded_bit;
    cout.setf(ios::fixed);
    cout.precision(1);
    cout << sen_len * 8 << ' ' << encoded_bit << ' ' << encode_ratio << endl;
}

int main()
{
    string str;
    while (cin >> str) {
        if (str == "END")
            break;
        else {
            HuffmanEncoding HE(str);
            HE.display();
        }
    }
    return 0;
}
