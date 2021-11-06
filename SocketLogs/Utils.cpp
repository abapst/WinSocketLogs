#include "Utils.h"


void printClientInfo(sockaddr_in& addrInfo)
{
    printf("Accepted connection from %d.%d.%d.%d on port %d\n",
        addrInfo.sin_addr.S_un.S_un_b.s_b1,
        addrInfo.sin_addr.S_un.S_un_b.s_b2,
        addrInfo.sin_addr.S_un.S_un_b.s_b3,
        addrInfo.sin_addr.S_un.S_un_b.s_b4,
        addrInfo.sin_port
    );
}
