#ifndef __MY_SMART_CONTAINER_H__
#define __MY_SMART_CONTAINER_H__

#include "boost/scoped_ptr.hpp"
#include "boost/scoped_array.hpp"
#include <vector>
#include <map>

using std::vector;
using std::map;

template <typename T>
class SmartPtrVec
{
public:
    SmartPtrVec()
    {
    }
    ~SmartPtrVec()
    {
        for(vector<T*>::iterator it = m_private_vec.begin();
            it != m_private_vec.end(); ++it)
            delete *it;
    }
public:
    void add(T *ptr)
    {
        m_private_vec.push(ptr);
        return;
    }
private:
    vector<T*> m_private_vec;
};

template <typename T>
class SmartArrayPtrVec
{
public:
    SmartArrayPtrVec();
    {
    }
    ~SmartArrayPtrVec()
    {
        for(vector<T*>::iterator it = m_private_vec.begin();
            it != m_private_vec.end(); ++it)
            delete[] *it;
    }
public:
    void add(T *ptr)
    {
        m_private_vec.push_back(ptr);
        return;
    }

private:
    vector<T*> m_private_vec;
};

template <typename Key, typename Val>
class SmartPtrMap
{
public:
    SmartPtrMap()
    {
    }
    ~SmartPtrMap()
    {
        for(vector<Key,Val*>::iterator it = m_private_map.begin();
            it != m_private_map.end(); ++it)
            delete *(it->second);
    }
public:
    Val *& operator[] (const Key &k)
    {
        return m_private_map[k];
    }
private:
    map<Key,Val*> m_private_map;
};

template <typename Key, typename Val>
class SmartArrayPtrMap
{
public:
    SmartArrayPtrMap()
    {
    }
    ~SmartArrayPtrMap()
    {
        for(vector<Key,Val*>::iterator it = m_private_map.begin();
            it != m_private_map.end(); ++it)
            delete[] *(it->second);
    }
public:
    Val *& operator[] (const Key &k)
    {
        return m_private_map[k];
    }
private:
    map<Key,Val*> m_private_map;
};

#endif //__MY_SMART_CONTAINER_H__

