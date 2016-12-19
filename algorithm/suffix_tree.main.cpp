#include "suffix_tree.h"

int main(int argc, char *argv[])
{
    SuffixTree tree("abcabxabcd");
    tree.build();
    return 0;
}

