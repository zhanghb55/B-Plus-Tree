#include <iostream>
#include <algorithm>
#include <queue>
#include "Retarder.h"
#include "node.h"
#include "insert.h"
#include "search.h"
#include "remove.h"
#include <time.h>
#define test_num 50
record get_rand()
{
    return record(rand() % 201, rand() % 201);
}

void test(node*& root)
{
    delay_count insert_time;
    delay_count remove_time;
    record tem_rec;
    int count = 0;
    for (int i = 0; i < test_num; i++)
    {
        count += insert(root, get_rand(), insert_time);
    }
    int tem = insert_time.get_count();
    cout << "尝试插入" << test_num << "个随机记录，共" << 0.02 * tem << "s" << endl;
    cout << "成功插入" << count << "个记录，全树层序遍历如下" << endl;
    display(root);
    while (!insert(root, tem_rec=get_rand(), insert_time))
    {
        tem = insert_time.get_count();
    }
    cout << "此时再插一个随机记录（" << tem_rec.key << "," << tem_rec.value << "），花费" << 0.02 * (insert_time.get_count() - tem) << "s" << endl;
    cout << "新树如下" << endl;
    display(root);
    
    count = 0;
    for (int i = 0; i < test_num*2; i++)
    {
        count += remove(root, get_rand().key, remove_time);
    }
    cout << "尝试删除" << test_num*2 << "个随机记录，共" << 0.02 * remove_time.get_count() << "s" << endl;
    cout << "成功删除" << count << "个记录，新树层序遍历如下" << endl;
    display(root);
    while (!remove(root, tem_rec.key=get_rand().key, remove_time))
    {
        tem = remove_time.get_count();
    }
    cout << "此时再删除一个key为"<<tem_rec.key<<"的随机记录，花费" << 0.02 * (remove_time.get_count() - tem) << "s" << endl;
    cout << "新树如下" << endl;
    display(root);
    
    clear(root);
}

int main()
{
    node* root = NULL;
    srand(time(NULL));
    
    test(root);
    
    return 0;
}

