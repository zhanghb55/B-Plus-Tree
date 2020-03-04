#ifndef remove_h
#define remove_h
#include "node.h"

//两个调整函数
void leaf_balance(node*& root,delay_count&retarder)
{
    //叶的情况
    node* pare = root->parents;
    if (root->number < (l + 1) / 2)//结点太少，需要调整
    {
        if (!pare)//若父节点为nullptr，则为根叶节点
        {
            if (!(root->number))root = nullptr;//若全删空，则置为nullptr
            return;//不存在借节点和合并的可能，直接返回
        }
        //获取待操作结点在其双亲结点的第几个孩子位置
        int pos = get_position(root);
        if (pos == 0)//若为最左端的节点
        {
            node* bro = (pare->child)[1];//仅可能借哥哥的节点或和哥哥合并
            retarder.delay();
            if (bro->number == (int)((l + 1) / 2))//若哥哥也只是刚刚好满足，则两节点合并
            {
                for (int i = 0; i < bro->number; i++)//将哥哥的所有节点都复制到root原节点后
                {
                    (root->block)[root->number + i].key = (bro->block)[i].key;
                    (root->block)[root->number + i].value = (bro->block)[i].value;
                }
                root->number += bro->number;//更改root节点数
                delete bro;//删除哥哥节点
                (pare->key)[0] = (pare->key)[1];//更改父节点中root的key值
                for (int i = 1; i < pare->number - 1; i++)//将父节点的key和指针依次前移一位
                {
                    (pare->key)[i] = (pare->key)[i + 1];
                    (pare->child)[i] = (pare->child)[i + 1];
                }
                pare->number--;//更改父节点数
            }
            else {//若哥哥有多余的节点，则借一个
                //将哥哥最小的节点复制到root原节点后
                (root->block)[root->number].key = (bro->block)[0].key;
                (root->block)[root->number].value = (bro->block)[0].value;
                root->number++;//更改root节点数
                (pare->key)[0] = (bro->block)[0].key;//更改父节点中root的key值
                left_move(bro, 0);//将哥哥节点中的块依次左移一位
            }
        }
        else if (pos == pare->number - 1)//若为最右端的节点，操作类似最左端情况
        {
            retarder.delay();
            node* bro = (pare->child)[pos - 1];
            if (bro->number == (int)((l + 1) / 2))
            {
                for (int i = 0; i < root->number; i++)
                {
                    (bro->block)[bro->number + i].key = (root->block)[i].key;
                    (bro->block)[bro->number + i].value = (root->block)[i].value;
                }
                bro->number += root->number;
                delete root;
                root = nullptr;
                (pare->key)[pos - 1] = (pare->key)[pos];
                pare->number--;
            }
            else {
                right_move(root, 0);
                (root->block)[0].key = (bro->block)[bro->number - 1].key;
                (root->block)[0].value = (bro->block)[bro->number - 1].value;
                bro->number--;
                (pare->key)[pos - 1] = (bro->block)[bro->number - 1].key;
            }
        }
        else {//若为中间节点
            node* bbro = (pare->child)[pos + 1];
            node* lbro = (pare->child)[pos - 1];
            retarder.delay();
            retarder.delay();
            if (lbro->number > (int)((l + 1) / 2))//若弟弟节点有多，则借一个
            {
                right_move(root, 0);
                (root->block)[0].key = (lbro->block)[lbro->number - 1].key;
                (root->block)[0].value = (lbro->block)[lbro->number - 1].value;
                lbro->number--;
                (pare->key)[pos - 1] = (lbro->block)[lbro->number - 1].key;
            }
            else if (bbro->number > (int)((l + 1) / 2))//若弟弟节点刚刚好，又哥哥节点有多，则借一个
            {
                (root->block)[root->number].key = (bbro->block)[0].key;
                (root->block)[root->number].value = (bbro->block)[0].value;
                root->number++;
                (pare->key)[pos] = (bbro->block)[0].key;
                left_move(bbro, 0);
            }
            else {//若弟弟哥哥节点都刚刚好，则和弟弟节点合并
                for (int i = 0; i < root->number; i++)
                {
                    (lbro->block)[lbro->number + i].key = (root->block)[i].key;
                    (lbro->block)[lbro->number + i].value = (root->block)[i].value;
                }
                lbro->number += root->number;
                delete root;
                root = nullptr;
                (pare->key)[pos - 1] = (pare->key)[pos];
                for (int i = pos; i < pare->number - 1; i++)
                {
                    (pare->key)[i] = (pare->key)[i + 1];
                    (pare->child)[i] = (pare->child)[i + 1];
                }
                pare->number--;
            }
        }
    }
}

void nleaf_balance(node*& root,delay_count&retarder)
{
    //非叶的情况
    node* pare = root->parents;
    if (pare)//不是根的情况
    {
        if (root->number < ((m + 1) / 2))//结点太少，需要调整
        {
            int pos = get_position(root);
            if (pos == 0)//若为最左端的节点
            {
                node* bro = (pare->child)[1];//仅可能借哥哥的节点或和哥哥合并
                retarder.delay();
                if (bro->number == (int)((m + 1) / 2))//若哥哥也只是刚刚好满足，则两节点合并
                {
                    for (int i = 0; i < bro->number; i++)
                    {
                        (root->key)[root->number + i] = (bro->key)[i];
                        (root->child)[root->number + i] = (bro->child)[i];
                        (bro->child)[i]->parents = root;//与叶不同的是需要修改经移动的子节点的父节点
                    }
                    root->number += bro->number;
                    delete bro;
                    (pare->key)[0] = pare->key[1];
                    for (int i = 1; i < pare->number - 1; i++)
                    {
                        (pare->key)[i] = (pare->key)[i + 1];
                        (pare->child)[i] = (pare->child)[i + 1];
                    }
                    pare->number--;
                }
                else {//若哥哥有多余的节点，则借一个
                    (root->key)[root->number] = (bro->key)[0];
                    (root->child)[root->number] = (bro->child)[0];
                    (bro->child)[0]->parents = root;//与叶不同的是需要修改经移动的子节点的父节点
                    root->number++;
                    (pare->key)[0] = (bro->key)[0];
                    for (int i = 0; i < bro->number - 1; i++)
                    {
                        (bro->key)[i] = (bro->key)[i + 1];
                        (bro->child)[i] = (bro->child)[i + 1];
                    }
                    bro->number--;
                }
            }
            else if (pos == pare->number - 1)//若为最右端的节点，操作类似最左端情况
            {
                node* bro = (pare->child)[pos - 1];
                retarder.delay();
                if (bro->number == (int)((l + 1) / 2))
                {
                    for (int i = 0; i < root->number; i++)
                    {
                        (bro->key)[bro->number + i] = (root->key)[i];
                        (bro->child)[bro->number + i] = (root->child)[i];
                        (root->child)[i]->parents = bro;
                    }
                    bro->number += root->number;
                    delete root;
                    root = nullptr;
                    (pare->key)[pos - 1] = (pare->key)[pos];
                    pare->number--;
                }
                else {
                    for (int i = 0; i < root->number; i++)
                    {
                        (root->key)[i + 1] = (root->key)[i];
                        (root->child)[i + 1] = (root->child)[i];
                    }
                    (root->key)[0] = (bro->key)[bro->number - 1];
                    (root->child)[0] = (bro->child)[bro->number - 1];
                    (bro->child)[bro->number - 1]->parents = root;
                    root->number++;
                    bro->number--;
                    (pare->key)[pos - 1] = (bro->key)[bro->number - 1];
                }
            }
            else {//若为中间节点
                node* bbro = (pare->child)[pos + 1];
                node* lbro = (pare->child)[pos - 1];
                retarder.delay();
                retarder.delay();
                if (lbro->number > (int)((m + 1) / 2))//若弟弟节点有多，则借一个
                {
                    for (int i = 0; i < root->number; i++)
                    {
                        (root->key)[i + 1] = (root->key)[i];
                        (root->child)[i + 1] = (root->child)[i];
                    }
                    (root->key)[0] = (lbro->key)[lbro->number - 1];
                    (root->child)[0] = (lbro->child)[lbro->number - 1];
                    (lbro->child)[lbro->number - 1]->parents = root;//与叶不同的是需要修改经移动的子节点的父节点
                    root->number++;
                    lbro->number--;
                    (pare->key)[pos - 1] = (lbro->key)[lbro->number - 1];
                }
                else if (bbro->number > (int)((l + 1) / 2))//若弟弟节点刚刚好，又哥哥节点有多，则借一个
                {
                    (root->key)[root->number] = (bbro->key)[0];
                    (root->child)[root->number] = (bbro->child)[0];
                    (bbro->child)[0]->parents = root;//与叶不同的是需要修改经移动的子节点的父节点
                    root->number++;
                    (pare->key)[pos] = (bbro->key)[0];
                    for (int i = 0; i < bbro->number - 1; i++)
                    {
                        (bbro->key)[i] = (bbro->key)[i + 1];
                        (bbro->child)[i] = (bbro->child)[i + 1];
                    }
                    bbro->number--;
                }
                else {//若弟弟哥哥节点都刚刚好，则和弟弟节点合并
                    for (int i = 0; i < root->number; i++)
                    {
                        (lbro->key)[lbro->number + i] = (root->key)[i];
                        (lbro->child)[lbro->number + i] = (root->child)[i];
                        (root->child)[i]->parents = lbro;//与叶不同的是需要修改经移动的子节点的父节点
                    }
                    lbro->number += root->number;
                    delete root;
                    root = nullptr;
                    (pare->key)[pos - 1] = (pare->key)[pos];
                    for (int i = pos; i < pare->number - 1; i++)
                    {
                        (pare->key)[i] = (pare->key)[i + 1];
                        (pare->child)[i] = (pare->child)[i + 1];
                    }
                    pare->number--;
                }
            }
        }
    }
    else {//为根的情况
        if (root->number == 1)//若仅有一个儿子
        {
            root = (root->child)[0];//向下成为新根
            delete root->parents;//删除原根
            root->parents = nullptr;//将新根的父节点设为nullptr
        }
    }
}

bool remove(node* &root, keytype target,delay_count&retarder)
{
    //使用bottom-up模式，先向下到叶，删除叶中节点，再通过递归向上进行调整
    if (!(root->leaf))
    {
        for (int i = 0; i < root->number; i++)
        {
            if (target <= (root->key)[i])
            {
                retarder.delay();
                bool tem = remove((root->child)[i], target,retarder);//向下
                nleaf_balance(root,retarder);//调整
                return tem;
            }
        }
        return 0;
    }
    else
    {
        for (int i = 0; i < root->number; i++)
        {
            if ((root->block)[i].key == target)//找到了
            {
                if (i > 0 && i == root->number - 1)//如果是最后一个（最大的）子节点，则需要调整父的key
                {                                   //若为根叶节点，可能出现只有一个子节点的情况导致下方语句段错误，故需i>0
                    keytype tem = (root->block)[i - 1].key;//暂存新的最大值，即新的key
                    node* curr = root;
                    while (curr->parents)//若不为根，可能需要调整父节点的key
                    {
                        (curr->parents->key)[get_position(curr)] = tem;//修改父节点key值
                        if (curr->parents->number - 1 == get_position(curr)) curr = curr->parents;//若修改的节点是父节点的最大儿子，则还需上溯修改爷爷的key值
                        else break;
                    }
                }
                left_move(root, i);//删除节点
                leaf_balance(root,retarder);//调整
                return 1;
            }
        }
        return 0;//没找到
    }
}

#endif
