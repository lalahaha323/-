/*************************************************************************
	> File Name: maze2.c
	> Author: 
	> Mail: 
	> Created Time: 2018年12月27日 星期四 21时01分05秒
 ************************************************************************/

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
#include<time.h>

#define N 10000
#define M 30
#define SIZE 100
#define WAll 0
#define ROAD 1

struct stu
{
    int X;
    int Y;
    int pre;
}Path[N];

int count;
int visit[SIZE][SIZE] = {0};
int map[SIZE + 1][SIZE + 1] = {0};
int dx[4] = {0, 1, 0, -1};
int dy[4] = {1, 0, -1, 0};
int ptr[21][21];
int visit_ptr[21][21];


int main_menu();
int user_login_menu();
int user_game_menu();
int user_log_menu();
void my_err();
void formation_menu();
void difficult_maze();
void menu();
void Initialize();
void print();
void find_path();
void print_map();
void Creat_01();
int IsHaveNeighbor();
int judge();
void out_put();
void easy_maze();
void open_file();
void find_easy_path();
int judge_easy();
void out_easy();
void out_easy_map();
void write_file();

void my_err(const char *err_string, int line)
{
    fprintf(stderr, "line: %d ", line);
    perror(err_string);
    exit(1);
}

void open_file(void)
{
    int i, j;
    int len,ret;
    int fd;
    char str[21];
    memset(ptr, 0, sizeof(ptr));
    memset(str, 0, sizeof(str));
    if((fd = open("maze.txt", O_RDONLY)) == -1)
    {
        my_err("open", __LINE__);
    }
    else
    {
        memset(ptr, 0, sizeof(ptr));
        for(i = 0; i < 21; i++)
        {
            if((len = read(fd, str, 21)) < 0)
                my_err("read", __LINE__);
            for(j = 0; j < 21; j++)
            {
                ptr[i][j] = str[j] - '0';
                if(str[j] == '0')
                    printf("■");
                else if(str[j] == '1')
                {
                    if(i == 1 && j == 1)
                        printf("⊙");
                    else if(i == 19 && j == 19)
                        printf("★");
                    else 
                        printf(" ");
                }
            }
            printf("\n");
        }
        //sleep(3);
    }
    close(fd);
}

void easy_maze(void)
{
    open_file();
    find_easy_path();
    printf("\n");
    out_easy_map();
    write_file();
    sleep(5);
}

void write_file()
{
    int q = 0,i = 0, j = 0;
    int fd;
    char str2[22];
    memset(str2, 0, sizeof(str2));
    if((fd = open("maze_2.txt", O_WRONLY|O_CREAT,S_IRUSR|S_IWUSR)) == -1)
    {
        my_err("open", __LINE__);
    }
    else
    {
        for(i = 0;i < 21; i++)
        {
            q = 0;
            memset(str2, 0, sizeof(str2));
            for(j = 0;j < 21; j++)
            {
                str2[q] = ptr[i][j] + '0';
                q++;
            }
            write(fd, str2, sizeof(char )* 21);
        }
    }
    close(fd);
}


void out_easy_map(void)
{
    int i = 0, j = 0;
    for(i = 0; i < 21; i++)
    {
        for(j = 0; j < 21; j++)
        {
            if(ptr[i][j] == 0)
                printf("■");
            else if(ptr[i][j] == 2)
            {
                if(i == 1 && j == 1)
                    printf("⊙");
                else if(i == 19 && j == 19)
                    printf("★");
                else
                    printf("◇");
            }
            else if(ptr[i][j] == 1)
            {
                if(i == 1 && j == 1)
                    printf("⊙");
                else
                    printf(" ");
            }
        }
        printf("\n");
    }
}

void find_easy_path(void)
{
    memset(visit_ptr, 0, sizeof(visit_ptr));
    memset(Path, 0 ,sizeof(Path));
    int i, flag = 0;
    int next_x,next_y;
    int front = 0;
    int rear = 1;
    int decide = 0;
    Path[0].X = 1;
    Path[0].Y = 1;
    Path[0].pre = -1;
    while(front < rear)
    {
        for(i = 0; i < 4; i++)
        {
            next_x = Path[front].X + dx[i];
            next_y = Path[front].Y + dy[i];
            if(decide = (judge_easy(next_x, next_y)))
            {
                visit_ptr[next_x][next_y] = 1;
                Path[rear].X = next_x;
                Path[rear].Y = next_y;
                Path[rear].pre = front;
                rear++;
            }
            if(next_x == 19 && next_y == 19)
            {
                out_easy(rear - 1);
                flag = 1;
                break;
            }
        }
        if(flag == 1)
            break;
        front++;
    }
}

void out_easy(int a)
{
    if(Path[a].pre != -1)
    {
        out_easy(Path[a].pre);
        ptr[Path[a].X][Path[a].Y] = 2;
    }
}

int judge_easy(int next_x, int next_y)
{
    if(ptr[next_x][next_y] == 1 && visit_ptr[next_x][next_y] == 0 && next_x >= 1 && next_x <= 20 && next_y >= 1 && next_y <= 20)
        return 1;
    else
        return 0;
}


int main_menu(void)
{
    int choice;
    printf("\t\t------------------------------------------\n");
    printf("\t\t|          ★ 欢迎来到迷宫游戏 ★          |\n");
    printf("\t\t|              1. 用户登录               |\n");
    printf("\t\t|              2. 用户注册               |\n");
    printf("\t\t|              0. 退出游戏               |\n");
    printf("\t\t|          ★ 欢迎来到迷宫游戏 ★          |\n");
    printf("\t\t------------------------------------------\n");
    printf("\n");
    printf("\t\t请输入序号：");
    scanf("%d",&choice);
    getchar();
    return choice;
}

int user_login_menu(void)
{
    MYSQL_RES *res;
    MYSQL_ROW row;
    MYSQL mysql;
    char sql[200];
    char user_name[M];
    char user_passwd[M];
    memset(sql, 0, sizeof(sql));
    memset(user_name, 0, sizeof(user_name));
    memset(user_passwd, 0, sizeof(user_passwd));
    if(NULL == mysql_init(&mysql))
    {
        printf("init: %s\n", mysql_error(&mysql));
    }
    if(!mysql_real_connect(&mysql, "localhost", "debian-sys-maint", "5FBt2AwBpXoUKYdV", "maze_game", 0, NULL, 0))
    {
        printf("connect: %s\n", mysql_error(&mysql));
    }
    printf("\n");
    printf("\t\t ☆ 请输入你的账号：");
    scanf("%s", user_name);
    getchar();
    printf("\t\t ☆ 请输入你的密码：");
    scanf("%s", user_passwd);
    getchar();
    strcpy(sql, "SELECT uname, upasswd FROM user_login");
    if(mysql_real_query(&mysql, sql, strlen(sql)))
    {
        printf("query: %s\n",mysql_error(&mysql));
    }
    res = mysql_store_result(&mysql);
    while((row = mysql_fetch_row(res)))
    {
        if(strcmp(row[0], user_name) == 0)
        {
            if(strcmp(row[1], user_passwd) == 0)
            {
                printf("\t\t ☆ 登陆成功\n");
                printf("\t\t 正在跳转...\n");
                sleep(1);
                mysql_free_result(res);
                return 1;
            }
            printf("\t\t ☆ 密码不正确\n");
            sleep(1);
            mysql_free_result(res);
            return 2;
        }
    }
    mysql_free_result(res);
    printf("\t\t ☆ 没有该用户\n");
    sleep(1);
    return 0;
}

int user_log_menu(void)
{
    regex_t regex;
    int regexInit;
    int reti;
    MYSQL_RES *res;
    MYSQL_ROW row;
    MYSQL mysql;
    char sql[200];
    char user_name[M];
    char user_passwd[M];
    memset(sql, 0, sizeof(sql));
    memset(user_name, 0, sizeof(user_name));
    memset(user_passwd, 0, sizeof(user_passwd));
    if(NULL == mysql_init(&mysql))
    {
        printf("init:   %s\n",mysql_error(&mysql));
    }
    if(!mysql_real_connect(&mysql,"localhost","debian-sys-maint","5FBt2AwBpXoUKYdV","maze_game",0,NULL,0))
    {
        printf("connect: %s\n",mysql_error(&mysql));
    }
    printf("\t\t ☆ 温馨提示：账号必须是以字母数字的组合，长度为5-15个字符\n");
    printf("\t\t ☆ 请输入账号：");
    scanf("%s", user_name);
    getchar();
    regexInit = regcomp(&regex, "^[a-zA-Z0-9]{5,15}$", REG_EXTENDED);
    if(regexInit)
    {
        my_err("regcomp", __LINE__);
    }
    else
    {
        reti = regexec(&regex, user_name, 0, NULL, 0);
        if(REG_NOERROR != reti)
        {
            printf("\t\t ☆ 账户名称不按要求来!\n");
            regfree(&regex);
            printf("\t\t ☆ 注册失败\n");
            sleep(1);
            return 2;
        }
        else
        {
            printf("\t\t ☆ 请输入你设置的密码：");
            scanf("%s", user_passwd);
            getchar();
        }
    }
    regfree(&regex);
    regexInit = regcomp(&regex, "^[0-9]$", REG_EXTENDED);
    if(regexInit)
    {
        my_err("regcomp", __LINE__);
    }
    else
    {
        reti = regexec(&regex, user_passwd, 0, NULL, 0);
        if(REG_NOERROR != reti)
        {
            printf("\t\t ☆ 密码强度 ■■\n");
            sleep(0.5);
        }
        else
        {
            printf("\t\t ☆ 密码强度 ■\n");
            sleep(0.5);
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
            printf("\t\t ☆ 该用户已经注册过\n");
            sleep(1);
            return 1;
        }
    }
    mysql_free_result(res);
    memset(sql,0,sizeof(sql));
    sprintf(sql, "INSERT INTO user_login(uname,upasswd)VALUES('%s','%s');",user_name,user_passwd);
    if(mysql_real_query(&mysql,sql,strlen(sql)))
    {
        printf("query:%s\n",mysql_error(&mysql));
    }
    printf("\t\t ☆ 注册成功\n");
    sleep(1);
    return 0;
}

int user_game_menu(void)
{
    int choice;
    printf("\t\t--------------------------------------------\n");
    printf("\t\t|           ☆ 欢迎来到迷宫游戏 ☆           |\n");
    printf("\t\t|             1. 查看游戏说明              |\n");
    printf("\t\t|             2. 简单等级                  |\n");
    printf("\t\t|             3. 随机等级                  |\n");
    printf("\t\t|             0. 退出此页面                |\n");
    printf("\t\t|           ☆ 欢迎来到迷宫游戏 ☆           |\n");
    printf("\t\t--------------------------------------------\n");
    printf("\n");
    printf("\t\t ☆ 请输入你的选择：");
    scanf("%d",&choice);
    getchar();
    return choice;
}

void formation_menu(void)
{
    printf("\t\t------------------------------------------------\n");
    printf("\t\t|             ☆ 欢迎你查看游戏说明 ☆            \n");
    printf("\t\t|  1. 简单等级：你可以享受本游戏已经存在的迷宫  \n");
    printf("\t\t|  2. 随机等级：需要你输入你想要的迷宫行数      \n");
    printf("\t\t|             ☆ 欢迎你查看游戏说明 ☆            \n");
    printf("\t\t------------------------------------------------\n");
    sleep(2);
}

void difficult_maze(void)
{
    memset(map, 0, sizeof(map));
    memset(visit, 0, sizeof(visit));
    memset(Path, 0, sizeof(Path));
    srand((unsigned)time(NULL));
    menu();
    Initialize(count);
    print();
    printf("\n");
    printf("\n");
    find_path();
    print_map();
}

int judge(int next_x, int next_y)
{
    if(map[next_x][next_y] == 5 && visit[next_x][next_y] == 0 && next_x >= 1 && next_x <= count - 2 && next_y >= 1 && next_y <= count - 2)
        return 1;
    else
        return 0;
}

void out_put(int a)
{
    if(Path[a].pre != -1)
    {
        out_put(Path[a].pre);
        map[Path[a].X][Path[a].Y] = 2;
    }
}

void print_map(void)
{
    int i = 0, j = 0;
    for(i = 0; i < count; i++)
    {
        for(j = 0; j < count; j++)
        {
            if(map[i][j] == 0)
                printf("■");
            else if(map[i][j] == 5)
            {
                if(i == 1 && j == 1)
                    printf("⊙");
                else if(i == count - 2 && j == count - 2)
                    printf("★");
                else
                    printf(" ");
            }
            else if(map[i][j] == 2)
            {
                if(i == count - 2 && j == count - 2)
                    printf("★");
                else 
                    printf("◇");
            }
        }
        printf("\n");
    }
}


void find_path(void)
{
    int i, flag = 0;
    int next_x,next_y;
    int front = 0;
    int rear = 1;
    int decide = 0;
    Path[0].X = 1;
    Path[0].Y = 1;
    Path[0].pre = -1;
    while(front < rear)
    {
        for(i = 0; i < 4; i++)
        {
            next_x = Path[front].X + dx[i];
            next_y = Path[front].Y + dy[i];
            if(decide = (judge(next_x, next_y)))
            {
                visit[next_x][next_y] = 1;
                Path[rear].X = next_x;
                Path[rear].Y = next_y;
                Path[rear].pre = front;
                rear++;
            }
            if(next_x == count - 2 && next_y == count - 2)
            {
                out_put(rear - 1);
                flag = 1;
                break;
            }
        }
        if(flag == 1)
            break;
        front++;
    }
}


void Initialize(int num)
{
    int i,j;
    if(num % 2 == 0)
        count++;
    memset(map, 0, sizeof(map));
    for(i = 0; i < SIZE; i++)
    {
        for(j = 0; j < SIZE; j++)
        {
            if(j >= count || j >= count)
                map[i][j] = -1;
            else if(i != 0 && j != 0 && i != count - 1 && j != count - 1)
            {
                if(i % 2 != 0)
                {
                    if(j % 2 == 1)
                    {
                        map[i][j] = 1;
                    }
                }
            }
        }
    }
    Creat_01(1, 1);
}

void Creat_01(int X_index, int Y_indey)
{
    int rand_position;
    int x1, y1, flag = 0;
    x1 = X_index;
    y1 = Y_indey;
    while(1)
    {
        flag = 0;
        flag = IsHaveNeighbor(X_index, Y_indey);
        if(flag == 0)
            return;
        else
        {
            map[X_index][Y_indey] = 5;
            x1 = X_index;
            y1 = Y_indey;
            while(1)
            {
                rand_position = rand() % 4;
                if(rand_position == 0 && X_index >= 3 && map[X_index - 2][Y_indey] == 1)
                {
                    X_index = X_index - 2;
                }
                else if(rand_position == 1 && X_index < count - 3 && map[X_index + 2][Y_indey] == 1)
                {
                    X_index = X_index + 2;
                }
                else if(rand_position == 2 && Y_indey >= 3 && map[X_index][Y_indey - 2] == 1)
                {
                    Y_indey = Y_indey - 2;
                }
                else if(rand_position == 3 && Y_indey < count - 3 && map[X_index][Y_indey + 2] == 1)
                {
                    Y_indey = Y_indey + 2;
                }
                map[(x1 + X_index) / 2][(y1 + Y_indey) / 2] = 5;
                map[X_index][Y_indey] = 5;
                Creat_01(X_index, Y_indey);
                break;
            }
        }
    }
}

int IsHaveNeighbor(int X_index, int Y_indey)
{
    int i, j, flag = 0;
    if((X_index >= 3 && map[X_index - 2][Y_indey] == 1)|| (X_index < count - 3 && map[X_index + 2][Y_indey] == 1) || (Y_indey >= 3 && map[X_index][Y_indey + 2] == 1))
        return 1;
    return 0;
}

void print(void)
{
    int i = 0, j = 0;
    for(i = 0; i < count; i++)
    {
        for(j = 0; j < count; j++)
        {
            if(map[i][j] == 0)
                printf("■");
            else if(map[i][j] == 5)
            {
                if(i == 1 && j == 1)
                    printf("⊙");
                else if(i == count - 2 && j == count - 2)
                    printf("★");
                else 
                    printf(" ");
            }
        }
        printf("\n");
    }
}


void menu(void)
{
    printf("\t\t ☆ 请输入地图的大小：");
    scanf("%d", &count);
}

int main(void)
{
    memset(visit, 0, sizeof(visit));
    memset(map, 0, sizeof(map));
    memset(Path, 0, sizeof(Path));
    int choice1, choice2, choice3;
    choice1 = main_menu();
    while(choice1 != 0)
    {
        switch(choice1)
        {
            case 1:
                printf("\t\t正在跳转...\n");
                sleep(1);
                system("reset");
                choice2 = user_login_menu();
                switch(choice2)
                {
                    case 1:
                        system("reset");
                        choice3 = user_game_menu();
                        sleep(0.5);
                        system("reset");
                        while(choice3 != 0)
                        {
                            switch(choice3)
                            {
                                case 1:
                                    formation_menu();
                                    break;
                                case 2:
                                    easy_maze();
                                    break;
                                case 3:
                                    difficult_maze();
                                    sleep(10);
                                    break;
                                case 0:
                                    break;
                            }
                            sleep(0.5);
                            system("reset");
                            choice3 = user_game_menu();
                        }
                        break;
                    case 2:
                        break;
                    case 3:
                        break;
                }
                break;
            case 2:
                printf("\t\t 正在跳转...\n");
                sleep(1);
                system("reset");
                choice2 = user_log_menu();
                switch(choice2)
                {
                    case 1:
                        break;
                    case 2:
                        break;
                    case 3:
                        break;
                }
                break;
            case 0:
                break;
        }
        sleep(0.5);
        system("reset");
        choice1 = main_menu();
    }
    printf("\t\t你已经退出游戏～欢迎下次再来！\n");
}
