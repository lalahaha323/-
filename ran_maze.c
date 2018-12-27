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

#define SIZE 2*50 //地图大小 SIZE*SIZE

int visit[SIZE][SIZE] = {0};
int dx[4] = {1, 0, -1, 0};
int dy[4] = {0, 1, 0, -1};
struct stu
{
    int X;
    int Y;
    int pre;
}Path[10000];

int map[SIZE + 1][SIZE + 1] = {0};
int count;//地图大小

void Initialize(int count);//初始化地图
void print();//打印函数
void menu();//菜单选择难度和大小
int IsHaveNeighbor();//判断当前方块处是否有邻居
void Creat_01();//第一种创建方法
void find_path();
void out_put();
int judge();
void print_map();

int main(void)
{
    srand((unsigned)time(NULL));
    menu();
    Initialize(count);
    print();
    find_path();
    print_map();
    return 0;
}

void find_path(void)
{
    //从1,1开始，终点是count-2,count-2,边框大小是1-count-1
    int i, flag = 0;
    int next_x, next_y;
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

void out_put(int a)
{
    if(Path[a].pre != -1)
    {
        out_put(Path[a].pre);
        map[Path[a].X][Path[a].Y] = 2;
    }
}

int judge(int next_x, int next_y)
{
    if(map[next_x][next_y] == 5 && visit[next_x][next_y] == 0 && next_x >= 1 && next_x <= count - 2 && next_y >= 1 && next_y <= count - 2)
        return 1;
    else
        return 0;
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
                else if( i == count - 2 && j == count - 2 )
                    printf("★");
                else
                    printf(" ");
            }
            else if(map[i][j] == 2)
            {
                if( i == count - 2 && j == count - 2 )
                    printf("★");
                else
                    printf("◇");
            }
        }
        printf("\n");
    }
}
void print()
{
    int i = 0, j = 0;
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
    /*for(i = 0; i < count; i++)//将地图全部初始化等于0
    {
        for(j = 0; j < count; j++)
            map[i][j] = 0;
    }*/
    memset(map,0,sizeof(map));
    for(i = 0;i < SIZE;i++)
    {
        for(j = 0; j < SIZE; j++)
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
    Creat_01(1, 1);   
}

void Creat_01(int X_index, int Y_index)
{
    int rand_position;//随机一个方向 
    int x1, y1, flag = 0;
    x1 = X_index;
    y1 = Y_index;

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
            x1 = X_index;
            y1 = Y_index;
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
                map[(x1 + X_index) / 2][(y1 + Y_index) / 2] = 5;
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
    if((X_index >= 3 && map[X_index - 2][Y_index] == 1) || (X_index < count - 3 && map[X_index + 2][Y_index] == 1) || (Y_index >= 3 && map[X_index][Y_index - 2] == 1) || (Y_index < count - 3 && map[X_index][Y_index + 2] == 1))
        return 1;
    return 0;
}

void menu()
{
    printf("请输入地图大小：");
    scanf("%d",&count);
}


