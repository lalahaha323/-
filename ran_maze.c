/*************************************************************************
	> File Name: test.c
	> Author: 
	> Mail: 
	> Created Time: 2018年12月27日 星期四 12时31分57秒
 ************************************************************************/
#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<string.h>


#define N 2//关卡的数目
#define M N*50 //地图大小 M*M


int level = 0;//游戏等级
int map[M + 1][M + 1] = {0};
int count;//地图大小
int my_x, my_y;//我的位置
char str[2 * (M + 1) * (M + 1)] = { '\0' };
void Initialize(int count);//初始化地图
void print();//打印函数
void find();//找到自己
void menu();//菜单选择难度和大小
int IsHaveNeighbor();//判断当前方块处是否有邻居
void Creat_01();//第一种创建方法


int main(void)
{
    int s[100][100] = {0};
    srand((unsigned)time(NULL));
    menu();
    Initialize(count);
    print();
    return 0;
}



void find()
{
    int i, j;
    for(i = 0; i < count; i++)
    {
        for(j = 0; j < count; j++)
        {
            if(map[i][j] == 3)
            {
                my_x = i;
                my_y = j;
            }
        }
    }
}


void print()
{
    int i = 0, j = 0;
    str[0] = '\0';
    //system("reset");
    for(i = 0; i < count ; i++)
    {
        for(j = 0; j < count ; j++)
        {
            if(map[i][j] == 0)
                printf("■");
            else if(map[i][j] == 5)
            {
                if( i == 1 && j == 1 )
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


void Initialize(int num)
{
    int i,j;
    if(num % 2 == 0)
        count++;
    for(i = 0; i < count; i++)//将地图全部初始化等于0
    {
        for(j = 0; j < count; j++)
            map[i][j] = 0;
    }
    for(i = 0;i < M;i++)
    {
        for(j = 0; j < M; j++)
        {
            if(i >= count || j >= count)
                map[i][j] = -1;
            else if(i != 0 && j != 0 && i != count - 1 && j != count - 1)
            {
                if(i % 2 != 0)
                {
                    if(j % 2 == 1)//i是奇数并且j是奇数
                    {
                        map[i][j] = 1;
                    }
                }
            }
        }
    }
    if(level == 0)
    {
        Creat_01(1, 1);   
    }
    else if(level == 1)
    {
        map[1][1] = 5;
        map[1][2] = 6;
        map[2][1] = 6;
        //Creat_02(1, 1);
    }
}

void Creat_01(int X_index, int Y_index)
{
    int rand_position;//随机一个方向 
    int x, y, flag = 0;
    x = X_index;
    y = Y_index;

    //如果四个方向都没有了，返回上一步，否则继续
    while(1)
    {
        flag = 0;
        flag = IsHaveNeighbor(X_index, Y_index);
        if(flag == 0)
            return;
        else 
        {
            map[X_index][Y_index] = 5;//即红色格子
            x = X_index;
            y = Y_index;
            while(1)
            {
                rand_position = rand() % 4;//随机得到一个方向
                if(rand_position == 0 && X_index >=3 && map[X_index - 2][Y_index] == 1)//上
                {
                    X_index = X_index - 2;
                }
                else if(rand_position == 1 && X_index < count - 3 && map[X_index + 2][Y_index] == 1)//下
                {
                    X_index = X_index + 2;
                }
                else if(rand_position == 2 && Y_index >= 3 && map[X_index][Y_index - 2] == 1)//左
                {
                    Y_index = Y_index - 2;
                }
                else if(rand_position == 3 && Y_index < count - 3 && map[X_index][Y_index + 2] == 1)//右：
                {
                    Y_index = Y_index + 2;
                }
                map[(x + X_index) / 2][(y + Y_index) / 2] = 5;
                map[X_index][Y_index] = 5;
                Creat_01(X_index, Y_index);
                break;
            }
        }

    }
}

int IsHaveNeighbor(int X_index, int Y_index)
{
    int i, j, flag = 0;
    if(level == 0)
    {
        if((X_index >= 3 && map[X_index - 2][Y_index] == 1) || (X_index < count - 3 && map[X_index + 2][Y_index] == 1) || (Y_index >= 3 && map[X_index][Y_index - 2] == 1) || (Y_index < count - 3 && map[X_index][Y_index + 2] == 1))
            return 1;
        return 0;
    }
}

void menu()
{
    char select[10];
    printf("分别用wasd控制人物移动，按ESC退出游戏\n");
    printf("请输入地图大小：");
    scanf("%d",&count);
    printf("请输入游戏难度（1,2）：");
    while(1)
    {
        scanf("%s",select);
        if(strlen(select) > 1)
        {
            printf("错误输入，请重新输入\n");
        }
        else
        {
            if('1' == select[0])
            {
                level = 0;
                break;
            }
            else if('2' == select[0])
            {
                level = 1;
                break;
            }
            else
            {
                printf("错误输入，请重新输入\n");
            }
        }
    }
}


