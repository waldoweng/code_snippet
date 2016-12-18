#include <cstddef>
#include <cassert>
#include "suffix_tree.h"
#include <stdio.h>

const unsigned int alphabet_size = 256;
const unsigned int charater_size = sizeof(char);

struct SuffixTreeEdge
{
    SuffixTreeEdge(): 
        begin(-1), end(-1), node(nullptr) 
    {
    }

    size_t begin;
    size_t end;
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
    SuffixTreeActivePoint() : node(nullptr), edge_index(alphabet_size), depth(0)
    {
    }

    SuffixTreeNode *node;
    unsigned int edge_index;
    unsigned int depth;
};

void SuffixTree::handleActivePoint(unsigned int current_pos, unsigned int current_charater, SuffixTreeActivePoint &active_point, 
    int &remain_count, unsigned int &free_node_index, SuffixTreeNode *&suffix_link, bool &need_suffix_link)
{
    printf("pos:%u char:%u remain_count:%d free_node_index:%u active_point.edge_index:%u"
            " depth:%u node:%p\n", current_pos, current_charater, remain_count, free_node_index,
            active_point.edge_index, active_point.depth, &active_point.node);
    if(remain_count == 0)
    {
        // if break in the begining, need a new brach
        if(active_point.node->edges[current_charater].begin == -1)
        {
            // create a new edge
            active_point.node->edges[current_charater].begin = current_pos;
            active_point.node->edges[current_charater].node = &nodes[free_node_index++];
            active_point.node->edges[current_charater].node->suffix_link = &nodes[0];
        } 
        else 
        {
            remain_count ++;
            active_point.edge_index = current_charater;
            need_suffix_link = false;
        }
    }
    else if(remain_count > 0)
    {
        SuffixTreeEdge &current_edge = active_point.node->edges[active_point.edge_index];
        assert(current_edge.begin != -1);
        
        //check the edge length, if not enough, go to the next edge
        while(current_edge.end != -1 && current_edge.end-current_edge.begin < remain_count)
        {
            active_point.node = current_edge.node;
            active_point.edge_index = m_str.c_str()[current_edge.end+1];
            remain_count -= (current_edge.end - current_edge.begin + 1);
            current_edge = active_point.node->edges[active_point.edge_index];
        }
        
        // make some assertion
        unsigned int tmp_end = 0;
        if(current_edge.end == -1)
            tmp_end = current_pos;
        else 
            tmp_end = current_edge.end;
        
        printf("tmp_end:%u end:%lu current:%u begin:%lu remain_count:%u\n", 
            tmp_end, current_edge.end, current_pos, current_edge.begin, remain_count);
        assert(tmp_end - current_edge.begin + 1 >= remain_count);
        assert(m_str.size()-1 > current_edge.begin+remain_count);

        // process
        if(current_charater == m_str.c_str()[current_edge.begin+remain_count])
        {
            remain_count ++;
        } else {
            // break in the middle
            unsigned int break_point = current_edge.begin + remain_count;
            unsigned int break_edge_index = m_str.c_str()[break_point];
            // create a new edge
            nodes[free_node_index].edges[break_edge_index].begin = break_point;
            nodes[free_node_index].edges[break_edge_index].end = current_edge.end;
            nodes[free_node_index].edges[break_edge_index].node = current_edge.node;
            nodes[free_node_index].edges[current_charater].begin = current_pos;
            nodes[free_node_index].edges[current_charater].node = &nodes[free_node_index+1];
            nodes[free_node_index].suffix_link = suffix_link;
            suffix_link = &nodes[free_node_index];
            current_edge.node = &nodes[free_node_index++];
            current_edge.end = break_point - 1;
            free_node_index++;
        }
    }
}

int SuffixTree::build()
{
    if(nodes != nullptr) delete[] nodes;

    // initialize node array, in maximum 2*sizeof(str)-1
    unsigned int node_count = m_str.size()*2 - 1;
    nodes = new SuffixTreeNode[node_count];
    unsigned int free_node_index = 1;
    
    SuffixTreeNode &root_node = nodes[0];
    root_node.suffix_link = &nodes[0];
    
    // initialize active_point & remain_count for building suffix tree
    int remain_count = 0;
    SuffixTreeActivePoint active_point;
    active_point.node = &root_node;
    
    // current_pos 
    unsigned int current_pos = 0;

    for(string::iterator it = m_str.begin(); it != m_str.end(); ++it, current_pos++)
    {
        // set suffix link 
        SuffixTreeNode *suffix_link = &root_node;
        unsigned int current_charater = *it;
        bool need_suffix_link = true;
        while(true)
        {
            handleActivePoint(current_pos, current_charater, active_point, remain_count, 
                free_node_index, suffix_link, need_suffix_link);
            if(active_point.node->suffix_link == nullptr || remain_count <= 0 || !need_suffix_link)
                break;
            else
            {
                active_point.edge_index = m_str.c_str()[current_pos-remain_count+1];
                active_point.depth --;
                active_point.node = active_point.node->suffix_link;
                remain_count --;
            }
        }
    }

    bool need_suffix_link = true;
    SuffixTreeNode *suffix_link = &root_node;
    while(true)
    {
        handleActivePoint(current_pos, 256, active_point, remain_count, 
            free_node_index, suffix_link, need_suffix_link);

        printf("node:%p suffix_link:%p remain_count:%d\n", 
            active_point.node, active_point.node->suffix_link, remain_count);
        if(active_point.node->suffix_link == nullptr || remain_count <= 0 || !need_suffix_link)  
            break;
        else
        {
            if(current_pos-remain_count+1 < m_str.size())
                active_point.edge_index = m_str.c_str()[current_pos-remain_count+1];
            else
                active_point.edge_index = 256;

            active_point.depth --;
            active_point.node = active_point.node->suffix_link;
            remain_count --;
        }
    }

    for(int i = 0; i < free_node_index; i++)
    {
        printf("node #%d %p suffix_link:%p sizeof(SuffixTreeNode):%ld\n", 
            i, &nodes[i], nodes[i].suffix_link, sizeof(SuffixTreeNode));
        for(int j = 0; j < alphabet_size+1; j++)
        {
            if(nodes[i].edges[j].begin != -1)
            {
                if(nodes[i].edges[j].end == -1) nodes[i].edges[j].end = current_pos;
                printf("\t edge#%d begin:%ld end:%ld node:%p\n",
                    j, nodes[i].edges[j].begin, nodes[i].edges[j].end, nodes[i].edges[j].node);
            }
        }
    }
    return 0;
}


