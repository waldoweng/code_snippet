#ifndef _MY_SUFFIX_TREE_H_
#define _MY_SUFFIX_TREE_H_

#include <string>
using namespace std;

class SuffixTreeNode;
class SuffixTreeActivePoint;

class SuffixTree
{
public:
    SuffixTree();
    SuffixTree(const string &str);
    ~SuffixTree();

public:
    int build();
    int build(const string &str);

public:
    string findLongestPrefix(const string &str);

protected:
    void handleActivePoint(unsigned int current_pos, unsigned int current_charater, 
        SuffixTreeActivePoint &active_point, int &remain_count, 
        unsigned int &free_node_index, SuffixTreeNode *&suffix_link,
        bool &need_suffix_link);

private:
    SuffixTreeNode *nodes;
    string m_str;


};


#endif //_MY_SUFFIX_TREE_H_

