#ifndef node_h
#define node_h
#define m 4
#define l 6                                                            
#include <iostream>
#include <queue>
#include<iomanip>
#include <algorithm>

#include <unistd.h>
//用sleep(20)来模拟
using namespace std;

typedef int keytype;
typedef double valuetype;

struct record {
    keytype key;//记录包括key
    valuetype value;
    record(keytype key = 0, valuetype value = 0)
    {
        this->key = key;
        this->value = value;
    }
};

//叶节点含leaf、number、block、parents，共1+1+12*6+4=78<=80                        还可以加链表+从兄弟节点塞节点
//非叶节点含leaf、number、key、child、parents，共1+1+4*4+4*4+4=38<=40
struct node {
    bool leaf;//判断是否为叶节点
    unsigned char number;//有效的key数
    //多的一个数组空间仅在分裂时使用，最大存放数仍为m，l
    keytype key[m+1];//存放key
    record block[l+1];//存放含record的块
    node* child[m+1];//孩子指针
    node* parents;//父指针
    node()
    {
        leaf = 0;
        number = 0;
        parents = nullptr;
        for (int i = 0; i < m+1; i++)
        {
            key[i] = 0;
            child[i] = nullptr;
        }
        for (int i = 0; i < l+1; i++)
        {
            block[i] = record();
        }
    }
};

bool operator < (const record& a,const record& b) {
    return a.key < b.key;
}

//获取待操作结点在其双亲结点的第几个孩子位置
int get_position(node* obj) {
    node* pare = obj->parents;
    if (pare == NULL)return 0;
    for (int i = 0; i < pare->number; i++) {
        if ((pare->child)[i] == obj) {
            return i;
        }
    }
    return 0;
}

//将i之后（不包括i）的block同步左移一位
void left_move(node* root, int i)
{
    for (int j = i; j < root->number-1; j++)
    {
        (root->block)[j].key = (root->block)[j + 1].key;
        (root->block)[j].value = (root->block)[j + 1].value;
    }
    root->number--;
}

//将i之后（包括i）的block同步右移一位
void right_move(node* root, int i)
{
    for (int j = i; j < root->number; j++)
    {
        (root->block)[j+1].key = (root->block)[j].key;
        (root->block)[j+1].value = (root->block)[j].value;
    }
    root->number++;
}

//将i之后的结点同步右移一位
void right_shift(node* parent, int position) {
    for (int i = parent->number; i > position; i--) {
        (parent->child)[i] = (parent->child)[i - 1];
        (parent->key)[i] = (parent->key)[i - 1];
    }
    parent->number++;
}

//层序遍历
void display(node* root)
{
    if (root)
    {
        int i = 0;
        queue<node*> tem;
        queue<node*> hold;
        queue<node*> emp;
        tem.push(root);
        node* curr;
        while (tem.size())
        {
            int j = 0;
            hold = tem;
            tem = emp;
            cout << "第" << i++ << "层：" << endl;
            while (hold.size())
            {
                curr =hold.front();
                hold.pop();
                int num = 0;
                if(curr) num = curr->number;
                else continue;
                if (curr->leaf)
                {
                    cout << "leaf" << j++ <<':'<< endl;
                    cout << "   key:";
                    for (int i = 0; i < num; i++) cout << setw(5) << (curr->block)[i].key;
                    puts("");
                    cout << "record:";
                    for (int i = 0; i < num; i++) cout << setw(5) << (curr->block)[i].value;
                    puts("");
                }
                else {
                    cout << "node" << j++ << ':' << endl;
                    for (int i = 0; i < num; i++)
                    {
                        tem.push(curr->child[i]);
                        cout << setw(5) << curr->key[i];
                    }
                }
                puts("");
            }
        }
    }
    else cout << "EMPTY" << endl;
}

void clear(node*& root) {
    if (root == NULL)return;
    if (!root->leaf)
    {
        for (int i = 0; i < root->number; i++) clear(root->child[i]);
        delete root;
        root = NULL;
    }
    else {
        delete root;
        root = NULL;
        return;
    }
}

#endif
