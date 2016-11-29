#ifndef _SMART_FLOCK_H__
#define _SMART_FLOCK_H__

#include "sys/file.h"


class SmartFlock
{
public:
    SmartFlock(int fd): m_fd(fd), m_locked(false)
    {
    }
    ~SmartFlock()
    {
        if(m_locked)
        {
            flock(m_fd, LOCK_UN);
            m_locked = false;
        }
    }

public:
    inline int lockEx()
    {
        if(m_locked) return 0;

        int ret = flock(m_fd, LOCK_EX);
        if(ret != 0) return ret;

        m_locked = true;
        return 0;
    }
    inline int lockSh()
    {
        if(m_locked) return 0;

        int ret = flock(m_fd, LOCK_SH);
        if(ret != 0) return ret;

        m_locked = true;
        return 0;

    }
    inline int unLock()
    {
        if(!m_locked) return 0;

        flock(m_fd, LOCK_UN);
        m_locked = false;
        return 0;
    }
    inline bool isLocked()
    {
        return m_locked;
    }

private:
    int m_fd;
    bool m_locked;
};


#endif //_SMART_FLOCK_H__

