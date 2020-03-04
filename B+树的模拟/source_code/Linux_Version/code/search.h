#ifndef search_h
#define search_h
#include "node.h"

bool search(node*root,keytype the_key ,record & target){
    node*temp = root;
    while(!(temp->leaf)){
        int pos = -1;
        for(int i = 0;i < temp->number;i++){
            if((temp->key)[i] >= the_key){
                temp = (temp->child)[i];
                pos = i;
                break;
            }
        }
        if(pos == -1)return false;
    }
    for(int i = 0;i < temp->number;i++){
        if((temp->block)[i].key == the_key){
            target = (temp->block)[i];
            return true;
        }
    }
    return false;
}


#endif
