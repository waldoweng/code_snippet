#ifndef _IP_UTILS_H__
#define _IP_UTILS_H__

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>

#include <string>

static string &getLocalIp()
{
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(fd == -1) return "";

    struct ifreq if;
    ifr.ifr_addr.sa_family = AF_INET;

    strncpy(ifr.ifr_name, "eth0", IFNAMESIZ);

    int ret = ioctl(fd, SIOCGIFADDR, &ifr);
    if(ret == -1)
    {
        close(fd);
        return "";
    }
    
    close(fd);
    return inet_ntoa(((sockaddr_in *)&ifr.ifr_addr)->sin_addr);
};

#endif //_IP_UTILS_H__

