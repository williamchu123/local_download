//
// Created by joy on 2020/8/31.
//


#include "conc.h"
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>


/**
 * 简单通信协议v1.0
 *
 *
 * client       ------->       server
 *
 * xxx xxx\n      ----->
 * success/fail url\n   <------
 *
 *
 * axel -n 20 url
 *
 * 过程结束
 *
 */
//建立socket连接

void download(char * url,char * file_name,char * sha){

    if(url == NULL || file_name == NULL){
        perror("参数不正确");
        exit(1);
    }


    int client_fd;


    //1,创建socket
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket() error. Failed to initiate a socket");
        exit(1);
    }


    //2,连接服务器

    struct sockaddr_in server_addr;


    memset(&server_addr,0,sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8001);
//    server_addr.sin_addr.s_addr = inet_addr("172.86.70.211");
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
//    inet_pton(AF_INET, "172.86.70.211", &server_addr.sin_addr.s_addr);


    //connect()
    if(connect(client_fd,(struct sockaddr *)&server_addr,sizeof(server_addr)) == -1){
        perror("connect() error: connect to fail");
        printf("%d\n",errno);
        exit(1);
    }



    //3,发送数据（读和写）
    char * format_str;
    format_str = (char *)malloc(2048);
    sprintf(format_str,"%s %s\n",url,file_name);

    char buf[1024];

    while (1){

        memset(buf,0,sizeof(buf));
        strcpy(buf,format_str);


        if(write(client_fd,buf,strlen(buf)) == -1){
            goto free;
        }


        char * receive_str = (char *)malloc(1024);

        memset(receive_str,0,1024);

        int r_count = read(client_fd,receive_str,1023);

        if(r_count == -1){
            perror("接收数据失败,下载任务取消");
            goto free;
        }


        char * result_url = (char *)malloc(1024);

        char * result_flag = (char *)malloc(24);

        memset(result_url,0,1024);
        memset(result_flag,0,24);

        sscanf(receive_str,"%s %s\n",result_flag,result_url);


        printf("result_flag=%s\nresult_url=%s\n",result_flag,result_url);

        if(strcmp("success",result_flag) == 0){

            char * pstr = malloc(1024);
            memset(pstr,0,1024);

            sprintf(pstr,"axel --num-connections=%d --output=%s %s",THREAD_NUM,file_name,result_url);


            system(pstr);
            exit(0);
        }else{
            perror("下载失败");
            goto free;
        }
    }




    free:
    close(client_fd);
    free(format_str);
    exit(1);








}








// 发送数据包


