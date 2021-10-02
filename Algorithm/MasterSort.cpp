#include <iostream>
#include <map>
#include <string>

using namespace std;

class MasterCmp {
    private:
        map<string, int> Master_table;

    public:
        MasterCmp();
        ~MasterCmp() {
            for (map<string, int>::iterator it = Master_table.begin(); it != Master_table.end(); it++)
                Master_table.erase(it);
        }
        int operator() (string str1, string str2) {
            map<string, int>::iterator it1 = Master_table.find(str1);
            map<string, int>::iterator it2 = Master_table.find(str2);
            if (it1 == Master_table.end()) {
                cout << str1 << " not found" << endl;
                return 0;
            }
            if (it2 == Master_table.end()) {
                cout << str2 << " not found" << endl;
                return 0;
            }
            if (it1->second == it2->second)
                return 0;
            else if (it1->second > it2->second)
                return 1;
            else
                return -1;
        }
};

MasterCmp::MasterCmp()
{
    Master_table["HuangShang"] = 1;
    Master_table["DaMoLaoZu"] = 1;
    Master_table["SaoDiSeng"] = 1;
    Master_table["ZhangSanFeng"] = 2;
    Master_table["DuGuQiuBai"] = 2;
    Master_table["JueYuan"] = 2;
    Master_table["WuYaZi"] = 3;
    Master_table["DuanYu"] = 4;
    Master_table["XuZu"] = 4;
    Master_table["QiaoFeng"] = 4;
    Master_table["WangChongYang"] = 4;
    Master_table["LaoWanTong"] = 4;
    Master_table["HongQiGong"] = 5;
    Master_table["HuangYaoShi"] = 5;
    Master_table["OuYangFeng"] = 5;
    Master_table["DuanHuangYe"] = 5;
    Master_table["GuoJing"] = 5;
    Master_table["YangGuo"] = 5;
    Master_table["XiaoLongNv"] = 6;
    Master_table["ZhangWuJi"] = 6;
    Master_table["JinLunFaWang"] = 6;
    Master_table["FangZhengDaShi"] = 7;
    Master_table["LingHuChong"] = 7;
    Master_table["DongFangBuBai"] = 7;
    Master_table["RenWoXing"] = 8;
    Master_table["XieXun"] = 9;
    Master_table["ZhangCuiShan"] = 9;
    Master_table["ChongXuDaoZhang"] = 9;
    Master_table["ZuoLengChan"] = 9;
    Master_table["TaoGuLiuXian"] = 10;
    Master_table["BuJieHeShang"] = 10;
}


void QuickSort(MasterCmp Cmp, string *array, int start_index, int end_index)
{
    if (end_index > start_index) {
        int i = start_index, j = end_index;
        string key = array[start_index];
        while (i < j) {
            while (Cmp(array[j], key) <= 0 && i < j)
                j--;
            array[i] = array[j];
            while (Cmp(array[i], key) >= 0 && i < j)
                i++;
            array[j] = array[i];
        }
        array[i] = key;
        QuickSort(Cmp, array, start_index, i - 1);
        QuickSort(Cmp, array, i + 1, end_index);
    }
}


void Merge(MasterCmp Cmp, string *array, string *tmp_array, int start_index, int mid_index, int end_index)
{
    int i = start_index, j = mid_index + 1, k = start_index;
    while (i != mid_index + 1 && j != end_index + 1) {
        if (Cmp(array[i], array[j]) >= 0)
            tmp_array[k++] = array[i++];
        else
            tmp_array[k++] = array[j++];
    }
    while (i != mid_index + 1)
        tmp_array[k++] = array[i++];
    while (j != end_index + 1)
        tmp_array[k++] = array[j++];
    for (i = start_index; i <= end_index; i++)
        array[i] = tmp_array[i];
}

void MergeSort(MasterCmp Cmp, string *array, string *tmp_array, int start_index, int end_index)
{
    if (start_index < end_index) {
        int mid_index = (end_index + start_index) / 2;
        MergeSort(Cmp, array, tmp_array, start_index, mid_index);
        MergeSort(Cmp, array, tmp_array, mid_index + 1, end_index);
        Merge(Cmp, array, tmp_array, start_index, mid_index, end_index);
    }
}


void swap(string *array, int i, int j) {
    string tmp = array[i];
    array[i] = array[j];
    array[j] = tmp;
}

void HeapSort(MasterCmp Cmp, string *array, int start_index, int end_index)
{
    for (int i = start_index; i <= end_index; i++) {
        int son = i;
        int dad = start_index + ((son - start_index) - 1) / 2;
        while (son != start_index && Cmp(array[son], array[dad]) == -1) {
            swap(array, son, dad);
            son = dad;
            dad = start_index + ((dad - start_index) - 1) / 2;
        }
    }
    for (int i = 0; i < end_index - start_index; i++) {
        swap(array, start_index, end_index - i);
        int dad = start_index, son = start_index + 1;
        while (son < end_index - i) {
            if (son + 1 < end_index - i && Cmp(array[son], array[son + 1]) == 1)
                son += 1;
            if (Cmp(array[dad], array[son]) <= 0)
                break;
            else {
                swap(array, dad, son);
                dad = son;
                son = start_index + (dad - start_index) * 2 + 1;
            }
        }
    }
}


int main()
{
    string Master_array_ori[16] = { "HuangShang", "ZhangWuJi", "HongQiGong", "HuangYaoShi", 
    "SaoDiSeng", "WuYaZi", "GuoJing", "YangGuo", "RenWoXing", "ChongXuDaoZhang", 
    "XieXun", "LingHuChong", "FangZhengDaShi", "DuGuQiuBai", "DuanYu", "QiaoFeng" };
    MasterCmp Cmp;

    cout << "original array" << endl;
    for (int i = 0; i < 16; i++)
        cout << Master_array_ori[i] << " ";
    cout << endl;

    string Master_array_quick[16] = {};
    for (int i = 0; i < 16; i++)
        Master_array_quick[i] = Master_array_ori[i];
    QuickSort(Cmp, Master_array_quick, 0, 15);
    cout << "quick sort" << endl;
    for (int i = 0; i < 16; i++)
        cout << Master_array_quick[i] << " ";
    cout << endl;

    string Master_array_merge[16] = {}, tmp_array[16] = {};
    for (int i = 0; i < 16; i++)
        Master_array_merge[i] = Master_array_ori[i];
    MergeSort(Cmp, Master_array_merge, tmp_array, 0, 15);
    cout << "merge sort" << endl;
    for (int i = 0; i < 16; i++)
        cout << Master_array_merge[i] << " ";
    cout << endl;

    string Master_array_heap[16] = {};
    for (int i = 0; i < 16; i++)
        Master_array_heap[i] = Master_array_ori[i];
    HeapSort(Cmp, Master_array_heap, 0, 15);
    cout << "heap sort" << endl;
    for (int i = 0; i < 16; i++)
        cout << Master_array_heap[i] << " ";
    cout << endl;

    return 0;
}