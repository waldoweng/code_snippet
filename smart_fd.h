#ifndef _SMART_FD_H__
#define _SMART_FD_H__

Class SmartFd
{
public:
    SmartFd(int fd) : m_fd(fd)
    {
    }
    ~SmartFd()
    {
        if(m_fd > 0)
        {
            close(m_fd);
            m_fd = -1;
        }
    }
private:
    int m_fd;
};

#endif //_SMART_FD_H__

