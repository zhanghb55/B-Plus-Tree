#ifndef insert_h
#define insert_h
#include"node.h"
#include "Retarder.h"

//判断结点是不是满的
bool is_full_node(node* obj)
{
    if (obj->leaf) return obj->number == l + 1;
    else return obj->number == m + 1;
}

//如果结点满，分裂叶结点，否则不操作
void spile(node*& root, node*& target)
{
    if (!is_full_node(target))return;
    node* pare = target->parents;
    if (target->leaf)
    {
        int position = get_position(target);
        right_shift(target->parents, position);
        (pare->child)[position + 1] = new node;
        (pare->child)[position + 1]->leaf = true;
        (pare->child)[position + 1]->parents = pare;
        int target_new_number = (target->number + 1) / 2;
        int new_leaf_number = target->number - target_new_number;
        keytype midkey = ((pare->child)[position]->block)[target_new_number - 1].key;
        target->number = target_new_number;
        (pare->child)[position + 1]->number = new_leaf_number;
        for (int i = 0; i < new_leaf_number; i++) {
            ((pare->child)[position + 1]->block)[i] = ((pare->child)[position]->block)[i + target_new_number];
        }
        
        (pare->key)[position + 1] = (pare->key)[position];
        (pare->key)[position] = midkey;
        if (is_full_node(pare)) spile(root, pare);
    }
    else
    {
        if (pare == NULL)
        {
            target->parents = new node;
            pare = target->parents;
            pare->parents = NULL;
            pare->number = 2;
            (pare->child)[0] = target;
            (pare->child)[1] = new node;
            (pare->child)[1]->parents = pare;
            (pare->child)[1]->leaf = (pare->child)[0]->leaf;
            int target_new_number = (target->number + 1) / 2;
            int new_node_number = target->number - target_new_number;
            keytype midkey = (target->key)[target_new_number - 1];
            (pare->key)[0] = midkey;
            keytype lastkey = (target->key)[target->number - 1];
            (pare->key)[1] = lastkey;
            target->number = target_new_number;
            (pare->child)[1]->number = new_node_number;
            for (int i = 0; i < new_node_number; i++) {
                ((pare->child)[1]->key)[i] = ((pare->child)[0]->key)[i + target_new_number];
                ((pare->child)[1]->child)[i] = ((pare->child)[0]->child)[i + target_new_number];
                ((pare->child)[0]->child)[i + target_new_number]->parents = (pare->child)[1];
            }
            root = pare;
        }
        else {
            int position = get_position(target);
            right_shift(target->parents, position);
            (pare->child)[position + 1] = new node;
            (pare->child)[position + 1]->leaf = (pare->child)[position]->leaf;
            (pare->child)[position + 1]->parents = pare;
            int target_new_number = (target->number + 1) / 2;
            int new_leaf_number = target->number - target_new_number;
            keytype midkey = ((pare->child)[position]->key)[target_new_number - 1];
            target->number = target_new_number;
            (pare->child)[position + 1]->number = new_leaf_number;
            for (int i = 0; i < new_leaf_number; i++)
            {
                ((pare->child)[position + 1]->key)[i] = ((pare->child)[position]->key)[i + target_new_number];
                ((pare->child)[position + 1]->child)[i] = ((pare->child)[position]->child)[i + target_new_number];
                ((pare->child)[position]->child)[i + target_new_number]->parents = (pare->child)[position + 1];
            }
            (pare->key)[position + 1] = (pare->key)[position];
            (pare->key)[position] = midkey;
            if (is_full_node(pare))
            {
                spile(root, pare);
            }
        }
    }
}

//将记录添加到叶中
void add_to_leaf(node* leaf, record target) {
    if (!leaf->leaf)return;
    (leaf->block)[leaf->number] = target;
    leaf->number++;
    sort(leaf->block, leaf->block + leaf->number);
}

//插入操作
bool insert(node*& root, record target, delay_count& retarder) {
    if (root == NULL) {
        root = new node;
        root->leaf = false;
        root->number = 1;
        root->parents = NULL;
        (root->key)[0] = target.key;
        (root->child)[0] = new node;
        node* temp = (root->child)[0];
        temp->number = 1;
        temp->leaf = true;
        temp->parents = root;
        (temp->block)[0] = target;
        return true;
    }
    node* temp = root;
    if (target.key > (root->key)[root->number - 1]) {
        while (!temp->leaf) {
            (temp->key)[temp->number - 1] = target.key;
            temp = (temp->child)[temp->number - 1];
            retarder.delay();
        }
        add_to_leaf(temp, target);
        
    }
    else {
        while (!temp->leaf) {
            int position = 0;
            for (int i = 0; i < temp->number; i++) {
                if ((temp->key)[i] > target.key) {
                    position = i;
                    break;
                }
            }
            temp = (temp->child)[position];
            retarder.delay();
        }
        for (int i = 0; i < temp->number; i++) {
            if (temp->block[i].key == target.key) {
                //    cout << "The keyword " << target.key << " already exists in the B + tree." << endl;
                return false;
            }
        }
        add_to_leaf(temp, target);
    }
    spile(root, temp);
    return true;
}

#endif
