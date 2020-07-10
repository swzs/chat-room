/*************************************************************************
	> File Name: udp_epoll.h
	> Author: suyelu 
	> Mail: suyelu@126.com
	> Created Time: Thu 09 Jul 2020 04:40:49 PM CST
 ************************************************************************/

#ifndef _UDP_EPOLL_H
#define _UDP_EPOLL_H
int udp_accept(int fd, struct User *user);
void add_to_sub_reactor(struct User *user);
void del_event(int epollfd, int fd);

#endif
