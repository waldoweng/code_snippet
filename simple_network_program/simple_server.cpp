#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

int main(int argc, char *argv[])
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0)
    {
        printf("socket fail:%d %m", errno, errno);
        return 0;
    }
    
    struct sockaddr_in svraddr;
    bzero(&svraddr, sizeof(svraddr));
    svraddr.sin_family = AF_INET;
    svraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    svraddr.sin_port = (unsigned short)htons(5630);

    int ret = bind(sockfd, (sockaddr  *)&svraddr, sizeof(svraddr));
    if(ret < 0)
    {
        printf("bind fail:%d %m", errno, errno);
        return 0;
    }

    ret = listen(sockfd, 14);
    if(ret < 0)
    {
        printf("listen fail:%d %m", errno, errno);
        return 0;
    }
    
    while(true)
    {
        struct sockaddr_in cliaddr;
        socklen_t _len = sizeof(cliaddr);
        int confd = accept(sockfd, (sockaddr *)&cliaddr, &_len);
        if(confd < 0)
        {
            printf("accept fail:%d %m", errno, errno);
            return 0;
        }
        
        char buf[1024] = {0};
        printf("connection from %s:%d\n",
            inet_ntop(AF_INET, &cliaddr.sin_addr, buf, sizeof(buf)),
            ntohs(cliaddr.sin_port));

        time_t timestamp = time(NULL);
        int len = snprintf(buf, sizeof(buf), "%.24s\r\n", ctime(&timestamp));
        write(confd, buf, len);

        close(confd);
    }
    return 0;
}
