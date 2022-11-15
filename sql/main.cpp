#include <iostream>
#include "../../!includes/sql_class/sql.h"
#include "../../!includes/vector_heap_sort/heap_sort.h"
using namespace std;

int main()
{

    SQL sql("_!select-1.txt");
    sql.run();

    cout<<endl;
    cout<<"=====END======"<<endl;
    return 0;
}
