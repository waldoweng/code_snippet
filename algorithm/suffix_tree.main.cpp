#include "suffix_tree.h"

int main(int argc, char *argv[])
{
    SuffixTree tree("abcabxabcd");
    tree.build();
    if(tree.isSuffix("abcd"))
        printf("test pass\n");
    else
        printf("test not pase\n");
    return 0;
}

