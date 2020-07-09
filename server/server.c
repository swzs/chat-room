/*************************************************************************
	> File Name: server.c
	> Author: suyelu 
	> Mail: suyelu@126.com
	> Created Time: Thu 09 Jul 2020 10:51:49 AM CST
 ************************************************************************/

#include "head.h"
char *conf = "./footballd.conf";
struct Map court;
struct Bpoint ball; //球的位置
struct BallStatus ball_status; //球的状态
struct Score score;
int repollfd, bepollfd;

int main(int argc, char **argv) {
    int opt, port = 0, listener, epollfd;
    while ((opt = getopt(argc, argv, "p:")) != -1) {
        switch (opt) {
            case 'p':
                port = atoi(optarg);
                break;
            default:
                fprintf(stderr, "Usage : %s -p port\n", argv[0]);
                exit(1);
        }
    }

    //判断一配置文件合法性

    if (!port) port = atoi(get_conf_value(conf, "PORT"));
    court.width = atoi(get_conf_value(conf, "COLS"));   
    court.height = atoi(get_conf_value(conf, "LINES"));
    
    court.start.x = 3;
    court.start.y = 3;
    
    ball.x = court.width / 2;
    ball.y = court.height / 2;

    bzero(&ball_status, sizeof(ball_status));
    ball_status.who = -1;
    bzero(&score, sizeof(score));

    if ((listener = socket_create_udp(port)) < 0) {
        perror("socket_create_udp()");
        exit(1);
    }

    DBG(GREEN"INFO"NONE" : Server start On port %d.\n", port);

    epollfd = epoll_create(MAX * 2);
    repollfd = epoll_create(MAX);
    bepollfd = epoll_create(MAX);

    if (epollfd < 0 || repollfd < 0 || bepollfd < 0) {
        perror("epoll_create()");
        exit(1);
    }

    struct epoll_event ev, events[MAX * 2];
    ev.events = EPOLLIN;
    ev.data.fd = listener;
    
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, listener, &ev) < 0) {
        perror("epoll_ctl");
        exit(1);
    }
    
    struct sockaddr_in client;
    bzero(&client, sizeof(client));
    socklen_t len = sizeof(client);

    while (1) {
        DBG(YELLOW"Main Reactor"NONE" : Waiting for clienti.\n");
        int nfds = epoll_wait(epollfd, events, MAX * 2, -1); 
        if (nfds < 0) {
            perror("epoll_wait()");
            exit(1);
        }
        for (int i = 0; i < nfds; i++) {
            struct User user;
            char buff[512] = {0};
            if (events[i].data.fd == listener) {
                //先只收数据
                recvfrom(listener, buff, sizeof(buff), 0, (struct sockaddr *)&client, &len);
                DBG(RED"Recv"NONE" :<%s:%d> %s \n", inet_ntoa(client.sin_addr), client.sin_port, buff);
            }
        }

    }


    return 0;
}
