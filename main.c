#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "conc.h"

/**
 *
 * 解析命令行参数 local_download -u https://codeload.github.com/junit-team/junit5/zip/main -f junit5.zip -s sha256
 *
 * 发送请求到服务器，等待服务器放回 success,启动 axel --num-connections 20 http://172.86.70.211/junit5.zip
 *
 *
 *
 *
 */

int main(int argc,char *argv[]) {


    char * url = NULL;

    char * file_name = NULL;

    char * sha = NULL;

    int c;

    while ( (c = getopt(argc,argv,"u:f:s::")) != -1){


        switch (c) {

            case 'u':
                url = optarg;
                break;
            case 'f':
                file_name = optarg;
                break;
            case 's':
                sha = optarg;
                break;

            default:
                printf("[Usage: local_download -u url -f file_name [-s sha256]]");

                exit(1);
                break;
        }
    }

    if(url == NULL || file_name == NULL){
        perror("error command  [Usage: local_download -u url -f file_name [-s sha256]  ]\n");
        exit(1);
    }

    printf("receive parameters \nurl:%s,\nfile_name:%s,\nsha:%s\n",url,file_name,sha);
    printf("starting download...\n");


    download(url,file_name,sha);

    return 0;
}
