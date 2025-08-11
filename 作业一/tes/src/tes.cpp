#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring> // For memset
#include <algorithm> // For binary search
using namespace std;

int k = 1; // 文件个数，从1开始

// 定义 Node 类，用于记录单词的位置
class Node {
public:
    int files; // 第几个文件
    int row;   // 行
    int col;   // 列
    Node* next;
    Node(int f = 1, int r = -1, int c = -1) : files(f), row(r), col(c), next(NULL) {}
};

// 定义 ArrayNode 类，用于存储单词及其相关信息
class ArrayNode {
public:
    string word;
    int count;       // 出现次数
    Node* position;  // 位置指针

    ArrayNode() : word(""), count(1), position(new Node()) {}
    ArrayNode(const string& w, int f, int r, int c) : word(w), count(1), position(new Node(f, r, c)) {}
};

// 定义 ArrayList 类，使用动态分配的静态数组实现，并保持数组有序
class ArrayList {
public:
    ArrayNode** nodes; // 动态分配的数组存储指针
    int size;          // 当前存储的单词数
    int capacity;      // 当前数组容量

    // 构造函数，初始化数组和相关变量
    ArrayList(int initial_capacity = 1000) : size(0), capacity(initial_capacity) {
        nodes = new ArrayNode*[capacity];
        memset(nodes, 0, sizeof(ArrayNode*) * capacity);
    }

    // 析构函数，释放所有分配的内存
    ~ArrayList() {
        for(int i = 0; i < size; ++i) {
            // 释放位置链表
            Node* current = nodes[i]->position;
            while(current != NULL) {
                Node* temp = current;
                current = current->next;
                delete temp;
            }
            // 释放 ArrayNode
            delete nodes[i];
        }
        // 释放数组本身
        delete[] nodes;
    }

    // 插入函数
    void insert(ArrayNode* a);

    // 辅助函数：二分查找单词的位置
    // 返回值：如果找到，返回索引；如果未找到，返回应该插入的位置的负值减一
    int binary_search_word(const string& word) const;
};

// 全局 ArrayList 实例
ArrayList* arr = new ArrayList();

// 辅助函数实现
int ArrayList::binary_search_word(const string& word) const {
    int left = 0;
    int right = size - 1;
    while(left <= right) {
        int mid = left + (right - left) / 2;
        if(nodes[mid]->word == word) {
            return mid;
        }
        else if(nodes[mid]->word < word) {
            left = mid + 1;
        }
        else {
            right = mid - 1;
        }
    }
    // 未找到，返回应该插入的位置的负值减一
    return -(left + 1);
}

// 插入函数实现，保持数组有序
void ArrayList::insert(ArrayNode* a) {
    // 使用二分查找确定单词是否已存在或插入位置
    int index = binary_search_word(a->word);
    if(index >= 0) {
        // 单词已存在，更新计数和位置
        nodes[index]->count++;
        Node* tmp = nodes[index]->position;
        while(tmp->next != NULL) {
            tmp = tmp->next;
        }
        tmp->next = a->position;
        delete a; // 释放未使用的 ArrayNode
        return;
    }
    else {
        // 单词不存在，确定插入位置
        int insert_pos = - (index + 1);
        // 检查是否需要扩容
        if(size >= capacity) {
            // 扩展数组容量（通常是当前容量的两倍）
            int new_capacity = capacity * 2;
            ArrayNode** new_nodes = new ArrayNode*[new_capacity];
            memset(new_nodes, 0, sizeof(ArrayNode*) * new_capacity);

            // 复制现有元素到新数组
            for(int i = 0; i < size; ++i) {
                new_nodes[i] = nodes[i];
            }

            // 释放旧数组
            delete[] nodes;

            // 更新指针和容量
            nodes = new_nodes;
            capacity = new_capacity;
        }

        // 将元素后移，为新单词腾出空间
        for(int i = size; i > insert_pos; --i) {
            nodes[i] = nodes[i-1];
        }

        // 插入新单词
        nodes[insert_pos] = a;
        size++;
    }
}

// 将文件读取为长字符串
string getfile(string path) {
    fstream file;
    string res;
    file.open(path, ios::in);
    if (file.is_open()) {
        cout << "File opened: " << path << endl;
        char ch;
        while (file.get(ch)) {
            if (ch >= 'A' && ch <= 'Z') ch += 32; // 转为小写
            res += ch;
        }
        file.close();
    }
    else {
        cout << "Failed to open file: " << path << endl;
    }
    return res;
}

// 判断是否为标点符号或空格
bool ispunc(char c) {
    if (c >= 'a' && c <= 'z') {
        return false;
    }
    return true;
}

// 处理文本，提取单词并记录位置
void solution(string text) {
    int row = 1, col = 0, files = k;
    string temp = "";
    int i = 0, textlen = text.length();
    while (i < textlen) {
        col++;
        if (text[i] == '\n') { // 换行时行加一，列重置为零
            row++;
            col = 0;
        }
        else if (i < textlen && ispunc(text[i])) { // 如果是标点符号
            if (temp != "") {
                // 创建新的 ArrayNode 并插入
                ArrayNode* a = new ArrayNode(temp, files, row, col - (int)temp.length());
                arr->insert(a);
                cout << temp << " ";
                cout << a->position->files << " " << a->position->row << " " << a->position->col << " ";
                temp = "";
            }
            i++;
            while (i < textlen && (ispunc(text[i]) || text[i] == '\n')) { // 考虑多个标点符号的情况
                if (text[i] == '\n') {
                    row++;
                    i++;
                    col = 0;
                }
                else {
                    col++;
                    i++;
                }
            }
            continue;
        }
        else if (i >= textlen) {
            return;
        }
        // 如果是字母
        else {
            temp += text[i]; // 构建单词
        }
        i++;
    }

    // 处理文本末尾的单词
    if (temp != "") {
        ArrayNode* a = new ArrayNode(temp, files, row, col - (int)temp.length() + 1);
        arr->insert(a);
        cout << temp << " ";
        cout << a->position->files << " " << a->position->row << " " << a->position->col << " ";
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Usage: ./main <input1.txt> <input2.txt> ..." << endl;
        return 0;
    }

    vector<string> path;
    for (int i = 1; i < argc; i++) {
        path.push_back(argv[i]);
    }

    while (k <= argc - 1) { // 修改循环条件，避免越界
        ofstream file;
        string outname = "out" + to_string(k) + ".txt";
        file.open(outname);
        if (!file.is_open()) {
            cout << "Failed to open output file: " << outname << endl;
            k++;
            continue;
        }

        string text = getfile(path[k - 1]); // 修改索引，从0开始
        // cout << text;
        solution(text);

        // 输出 ArrayList 中的所有单词及其信息（已按字典序）
        for(int i = 0; i < arr->size; ++i) {
            ArrayNode* search = arr->nodes[i];
            file << search->word << ';' << search->count << ';';
            cout << search->word << ';' << search->count << ';';

            Node* temper = search->position;
            while (temper != NULL) {
                file << '(' << temper->files << "," << temper->row << ',' << temper->col << ");";
                cout << '(' << temper->files << "," << temper->row << ',' << temper->col << ");";
                temper = temper->next;
            }
            file << endl;
            cout << endl;
        }

        file.close();
        // 不清空 ArrayList，以便累积下一个文件的数据
        // 因此，移除了之前清空 ArrayList 的代码

        k++;
    }

    delete arr;
    return 0;
}
