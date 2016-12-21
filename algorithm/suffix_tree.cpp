#include <cstddef>
#include <cassert>
#include "suffix_tree.h"
#include <stdio.h>
#include <map>
using std::map;

const unsigned int alphabet_size = 256;
const unsigned int charater_size = sizeof(char);

class SuffixTreeEdge
{
public:
    SuffixTreeEdge(): 
        begin(-1), end(-1), node(nullptr) 
    {
    }

public:
    unsigned int getLength(unsigned int current_pos)
    {
        if(end != -1)
            return end - begin;
        else
            return current_pos - begin;
    }

    string getEdge(const string &str, unsigned int current_pos)
    {
        if(end != -1)
            return str.substr(begin, end-begin+1);
        else
            return str.substr(begin, current_pos-begin+1);
    }

public:
    int begin;
    int end;
    SuffixTreeNode *node;
};

struct SuffixTreeNode
{
    SuffixTreeNode() : suffix_link(nullptr)
    {
    }

    SuffixTreeEdge edges[alphabet_size+1];
    SuffixTreeNode *suffix_link;
};


SuffixTree::SuffixTree()
    : nodes(nullptr)
{
}

SuffixTree::SuffixTree(const string &str)
    : nodes(nullptr), m_str(str)
{
}

SuffixTree::~SuffixTree()
{
    if(nodes != nullptr)
        delete []nodes;
}

int SuffixTree::build(const string &str)
{
    m_str = str;
    return build();
}

struct SuffixTreeActivePoint
{
    SuffixTreeActivePoint() : node(nullptr), edge(alphabet_size), length(0)
    {
    }

    SuffixTreeNode *node;
    unsigned int edge;
    unsigned int length;
};

void SuffixTree::handleActivePoint(unsigned int current_pos, unsigned int current_charater, SuffixTreeActivePoint &active_point, 
    int &remain_count, unsigned int &free_node_index, SuffixTreeNode *&prev_node, bool &did_split)
{
    if(remain_count == 1)
    {
        // if break in the begining, need a new brach
        if(active_point.node->edges[current_charater].begin == -1)
        {
            // create a new edge
            active_point.node->edges[current_charater].begin = current_pos;
            active_point.node->edges[current_charater].node = &nodes[free_node_index++];
        } 
        else 
        {
            remain_count ++;
            active_point.length ++;
            active_point.edge = current_charater;
        }
    }
    else if(remain_count > 1)
    {
        SuffixTreeEdge *current_edge = &active_point.node->edges[active_point.edge];
        assert(current_edge->begin != -1);
        
        //check the edge length, if not enough, go to the next edge
        while(current_edge->end != -1 && current_edge->getLength(current_pos) < active_point.length)
        {
            active_point.node = current_edge->node;
            active_point.edge = str[current_edge->end+1];
            active_point.length -= current_edge->getLength(current_pos)+1;
            current_edge = &(active_point.node->edges[active_point.edge]);
        }
        
        assert(current_edge->begin != -1);
        assert(current_edge->begin + active_point.length-1 < str.size());
        printf("remain count > 1, current_edge:%s active_point.length:%d current_charater:%d\n",
            current_edge->getEdge(m_str, current_pos).c_str(), active_point.length, current_charater);

        // caculate the break point
        unsigned int break_point = current_edge->begin + active_point.length;
        unsigned int break_edge = str[break_point];
        //check if break in the middle
        if(current_charater == str[break_point])
        {
            remain_count ++;
            active_point.length ++;
        } 
        else 
        {
            // create a new edge
            nodes[free_node_index].edges[break_edge].begin = break_point;
            nodes[free_node_index].edges[break_edge].end = current_edge->end;
            nodes[free_node_index].edges[break_edge].node = current_edge->node;
            nodes[free_node_index].edges[current_charater].begin = current_pos;
            nodes[free_node_index].edges[current_charater].node = &nodes[free_node_index+1];

            if(prev_node != nullptr)
                prev_node->suffix_link = &nodes[free_node_index];
            prev_node = &nodes[free_node_index];

            current_edge->node = &nodes[free_node_index++];
            current_edge->end = break_point-1;
            free_node_index++;
            did_split = true;

            printf("split into three edge #1:%s #2:%s #3:%s break_char:%d current_charater:%d\n",
                current_edge->getEdge(m_str, current_pos).c_str(),
                nodes[free_node_index-2].edges[break_edge].getEdge(m_str, current_pos).c_str(),
                nodes[free_node_index-2].edges[current_charater].getEdge(m_str, current_pos).c_str(),
                break_edge, current_charater);
        }
    }
}

int SuffixTree::build()
{
    if(nodes != nullptr) delete[] nodes;
    
    // convert the string to vector<unsigned int>
    for(string::iterator it = m_str.begin(); it != m_str.end(); ++it)
        str.push_back((unsigned int)*it);
    str.push_back(256);

    // initialize node array, in maximum 2*sizeof(str)-1
    unsigned int node_count = str.size()*2 - 1;
    nodes = new SuffixTreeNode[node_count];
    unsigned int free_node_index = 1;
    
    SuffixTreeNode &root_node = nodes[0];
    
    // initialize active_point & remain_count for building suffix tree
    int remain_count = 1;
    SuffixTreeActivePoint active_point;
    active_point.node = &root_node;

    // current_pos 
    unsigned int current_pos = 0;

    for(vector<unsigned int>::iterator it = str.begin(); it != str.end(); ++it, current_pos++)
    {
        // set suffix link 
        SuffixTreeNode *prev_node = nullptr;
        unsigned int current_charater = *it;
        bool did_split = false;
        
        // reset remain_count
        if(remain_count == 0)
            remain_count = 1;

        while(true)
        {
            handleActivePoint(current_pos, current_charater, active_point, remain_count, 
                free_node_index, prev_node, did_split);

            if(!did_split)
                break;

            if(active_point.node == &root_node)
            {
                remain_count--;
                active_point.edge = str[current_pos-remain_count+1];
                active_point.length --;
            }
            else
            {
                remain_count--;
                if(active_point.node->suffix_link != nullptr)
                    active_point.node = active_point.node->suffix_link;
                else
                    active_point.node = &root_node;
            }

            did_split = false;
        }
    }
    
    // debug 
    map<void *, int> pointer_map;
    for(int i = 0; i < free_node_index; i++)
        pointer_map[&nodes[i]] = i;

    for(int i = 0; i < free_node_index; i++)
    {
        printf("node #%d[%p] suffix_link:#%d[%p]\n", 
            i, &nodes[i], pointer_map[nodes[i].suffix_link], nodes[i].suffix_link);

        for(int j = 0; j < alphabet_size+1; j++)
        {
            if(nodes[i].edges[j].begin != -1)
            {
                if(nodes[i].edges[j].end == -1) nodes[i].edges[j].end = current_pos-1;
                printf("\tedge:%d dst_node#%d [%p] %s\n",
                    j, pointer_map[nodes[i].edges[j].node], nodes[i].edges[j].node,
                    nodes[i].edges[j].getEdge(m_str, current_pos-1).c_str());
            }
        }
    }
    return 0;
}

bool SuffixTree::isSuffix(const string &s)
{
    if(s.empty()) 
        return false;
    
    string::const_iterator it = s.begin();
    SuffixTreeNode *node = &nodes[0];

    while(true)
    {
        if(node->edges[*it].begin == -1)
            return false;
        
        SuffixTreeEdge &cur_edge = node->edges[*it];
        int i = cur_edge.begin;
        for(; i <= cur_edge.end && it != s.end(); i++, it++)
        {
            if(*it != str[i])
                return false;
        }
        
        if(i > cur_edge.end) {
            if(it == s.end()) {
                if(cur_edge.node->edges[alphabet_size].begin != -1)
                    return true;
                else
                    return false;
            } else {
                node = cur_edge.node;
                continue;
            }
        } else /* if(i <= cur_edge.end) */ {
            assert(it == s.end());
            if(str[i] == alphabet_size) return true;
            else return false;
        }
        
        assert("should not go here");
        return false;
    }

    return false;
}

