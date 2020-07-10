/*************************************************************************
	> File Name: client_recv.c
	> Author:sw 
	> Mail:601164744@qq.com 
	> Created Time: Sat 11 Jul 2020 12:20:04 AM CST
 ************************************************************************/

#include "head.h"
extern int sockfd;

void *do_recv(void *arg)
{
    struct ChatMsg msg;
    while (1) {
        //struct ChatMsg msg;
        bzero(&msg, sizeof(msg));
        int ret = recv(sockfd, (void *)&msg, sizeof(msg), 0);
        if (msg.type & CHAT_WALL) {
            printf(""BLUE"%s"NONE" : %s\n", msg.name, msg.msg);
        }else if (msg.type & CHAT_MSG){
            printf(""RED"%s"NONE" : %s\n", msg.name, msg.msg);
        }else if (msg.type & CHAT_SYS){
            printf(YELLOW"Server Inof"NONE" : %s\n", msg.msg);
        } else if (msg.type & CHAT_FIN){
            printf(L_RED"Server Inof "NONE"Server Down!\n");
            exit(1);
        } else if(msg.type & CHAT_FUNC){
            printf(GREEN"%s \n"NONE, msg.name);
        }
    }
}



