/*************************************************************************
	> File Name: maze.c
	> Author: 
	> Mail: 
	> Created Time: 2018年12月25日 星期二 20时06分48秒
 ************************************************************************/

/*
* 非递归程序求解
* 迷宫存储在文件中，通过输入文件名，创建相应的迷宫。
* 最终的解要求在屏幕上显示并存入文件中。
* 扩展要求：
*  * 随机产生迷宫
*  * 多条通路， 找最优解
*  * 界面效果
*  */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<sys/epoll.h>
#include<unistd.h>
#include<pthread.h>
#include<mysql/mysql.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<regex.h>


#define N 1000
#define M 30
int visit[N][N];

typedef struct 
{   
    int x;
    int y;
    int pre;
}path;

path PATH[N];
int dx[4] = {0, 1, 0, -1};
int dy[4] = {1, 0, -1, 0};


int main_menu();
int user_login_menu();

int main_menu(void)
{
    int choice;
    printf("------------------------------------------------------\n");
    printf("-------------------欢迎来到迷宫游戏-------------------\n");
    printf("-------------------   1.用户登录   -------------------\n");
    printf("-------------------   2.用户注册   -------------------\n");
    printf("-------------------   0.退出游戏   -------------------\n");
    printf("-------------------欢迎来到迷宫游戏-------------------\n");
    printf("------------------------------------------------------\n");
    printf("\n");
    printf("\t\t你输入序号：");
    scanf("%d",&choice);
    return choice;
}

int user_login_menu(void)
{
    MYSQL_RES *res;
    MYSQL_ROW row;
    MYSQL mysql;
    char sql[200];//mysql执行语句
    char user_name[M];
    char user_passwd[M];
    memset(sql,0,sizeof(sql));
    memset(user_name,0,sizeof(user_name));
    memset(user_passwd,0,sizeof(user_passwd));
    if(NULL == mysql_init(&mysql))
    {
        printf("init:   %s\n",mysql_error(&mysql));
    }
    if(!mysql_real_connect(&mysql,"localhost","debian-sys-maint","5FBt2AwBpXoUKYdV","maze_game",0,NULL,0))
    {
        printf("connect: %s\n",mysql_error(&mysql));
    }
    printf("------------------------------------------------------\n");
    printf("--温馨提示：账号必须以字母数字开头，长度为5-15个字符--\n");
    printf("--                                                  --\n");
    printf("--                                                  --\n");
    printf("-- 请输入你的账号：");
    scanf("%s",user_name);
    getchar();
    printf("-- 请输入你的密码：");
    scanf("%s",user_passwd);
    getchar();
    printf("--                                                  --\n");
    printf("------------------------------------------------------\n");
    strcpy(sql,"SELECT uname,upasswd FROM user_login;");
    if(mysql_real_query(&mysql, sql, strlen(sql)))
    {
        printf("query:%s\n",mysql_error(&mysql));
    }
    res = mysql_store_result(&mysql);
    while((row = mysql_fetch_row(res)))
    {
        if(strcmp(row[0],user_name) == 0)
        {
            if(strcmp(row[1],user_passwd) == 0)
            {
                printf("-- 登陆成功\n");
                mysql_free_result(res);
                return 1;//成功
            }
            mysql_free_result(res);
            return 2;//密码错误
        }
    }
    mysql_free_result(res);
    return 0;//账号输入不正确
}


int user_log_menu(void)
{
    regex_t regex;
    int regexInit;
    int reti;
    MYSQL_RES *res;
    MYSQL_ROW row;
    MYSQL mysql;
    char sql[200];//mysql执行语句
    char user_name[M];
    char user_passwd[M];
    memset(sql,0,sizeof(sql));
    memset(user_name,0,sizeof(user_name));
    memset(user_passwd,0,sizeof(user_passwd));
    if(NULL == mysql_init(&mysql))
    {
        printf("init:   %s\n",mysql_error(&mysql));
    }
    if(!mysql_real_connect(&mysql,"localhost","debian-sys-maint","5FBt2AwBpXoUKYdV","maze_game",0,NULL,0))
    {
        printf("connect: %s\n",mysql_error(&mysql));
    }
    printf("------------------------------------------------------\n");
    printf("--温馨提示：账号必须以字母数字开头，长度为5-15个字符--\n");
    printf("--                                                  --\n");
    printf("--                                                  --\n");
    printf("-- 请输入注册的账号：");
    scanf("%s",user_name);
    getchar();
    regexInit = regcomp(&regex, "^[a-zA-Z0-9]{5,15}$",REG_EXTENDED);
    if(regexInit)
    {
        my_err("regcomp", __LINE__);
    }
    else
    {
        reti = regexec(&regex,user_name,0,NULL,0);
        if(REG_NOERROR != reti)
        {
            printf("-- 输入名不按规范来\n");
            regfree(&regex);
            return 3;//
        }
        else
        {
            printf("-- 请输入你设置的密码：");
            scanf("%s",user_passwd);
            getchar();
            printf("--                                          --\n");
            printf("----------------------------------------------\n");
        }
    }
    regfree(&regex);
    regexInit = regcomp(&regex, "[a-zA-Z0-9]$",REG_EXTENDED);
    if(regexInit)
    {
        my_err("regcomp", __LINE__);
    }
    else
    {
        reti = regexec(&regex, user_passwd, 0, NULL, 0);
        if(REG_NOERROR != reti)
        {
            printf("-- 密码强度 简单\n");
        }
        else
        {
            printf("-- 密码强度 复杂\n");
        }
    }
    regfree(&regex);
    strcpy(sql,"SELECT uname FROM user_login;");
    if(mysql_real_query(&mysql, sql, strlen(sql)))
    {
        printf("query:%s\n",mysql_error(&mysql));
    }
    res = mysql_store_result(&mysql);
    while((row = mysql_fetch_row(res)))
    {
        if(strcmp(row[0],user_name) == 0)
        {
            mysql_free_result(res);
            return 2;
        }
    }
    mysql_free_result(res);
    memset(sql,0,sizeof(sql));
    sprintf(sql, "INSERT INTO user_login(uname,upasswd)VALUES('%s','%s');",user_name,user_passwd);
    if(mysql_real_query(&mysql,sql,strlen(sql)))
    {
        printf("query:%s\n",mysql_error(&mysql));
    }
    return 0;
}


int user_game__menu(void)
{
    int choice;
    printf("---------------------------------------------------\n");
    printf("|            ★  欢迎来到迷宫游戏  ★               |\n");
    printf("|               1. 查看游戏说明                   |\n");
    printf("|               2. 简单等级                       |\n");
    printf("|               3. 中等等级                       |\n");
    printf("|               4. 困难等级                       |\n");
    printf("---------------------------------------------------\n");
    printf("★ 请输入你的选择：");
    scanf("%d",&choice);
}


int main(void)
{
    int choice1,choice2,choice3;
    memset(visit,0,sizeof(visit));
    memset(PATH,0,sizeof(PATH));
    choice1 = main_menu();
    while(choice1 != 0)
    {
        switch(choice1)
        {
            case 1://用户登录
                printf("\t\t正在跳转...\n");
                sleep(0.5);
                system("reset");
                choice2 = user_login_menu();
                switch(choice2)
                {
                    case 1:
                        printf("\t\t登陆成功!\n");
                        printf("\t\t请稍后...\n");
                        sleep(1);
                        system("reset");
                        choice3 = user_game__menu();
                        break;
                    case 2:
                        printf("\t\t用户密码输入错误\n");
                        break;
                    case 0:
                        printf("\t\t没有该用户！\n");
                        break;
                }
                break;
            case 2://用户注册
                printf("\t\t正在跳转...\n");
                sleep(1);
                system("reset");
                choice2 = user_log_menu();
                switch(choice2)
                {
                    case 1:
                        printf("\t\t注册成功!\n");
                        break;
                    case 2:
                        printf("\t\t该用户已经注册过了!\n");
                        break;
                    case 3:
                        printf("\t\t注册失败！\n");
                        break;
                }
                break;
            case 0://退出
                break;
        }
        sleep(0.5);
        system("reset");
        choice1 = main_menu();
    }
    printf("\t\t你已经退出游戏~欢迎下次再来玩！\n");
}

