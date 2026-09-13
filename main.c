#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "board.h"
#include "piece.h"
#include "game.h"
#include "mode.h"
#include "save.h"

/*
 * 读取一整行输入,并去掉行尾的换行符。
 * 只按回车时 buf 为空串;输入结束(EOF)返回 0。
 * 行太长时把本行剩余字符丢掉,避免影响下一次输入。
 */
static int read_line(char* buf,int size)
{
    if (fgets(buf,size,stdin)==NULL)
    {
        buf[0]='\0';
        return 0;
    }

    if (strchr(buf,'\n')==NULL)
    {
        int ch;
        while ((ch=getchar())!='\n' && ch!=EOF);
    }
    else
        buf[strcspn(buf,"\n")]='\0';

    return 1;
}

/*
 * 读取一个 1..max 之间的编号(整行读取,只按回车/非法输入会重新询问)。
 * 输入结束(EOF)返回 0。
 */
static int choose_index(int max)
{
    char line[128];
    int i;

    while (1)
    {
        printf("Choose one:\n");

        if (!read_line(line,sizeof line))
            return 0;

        if (sscanf(line,"%d",&i)==1 && i>=1 && i<=max)
            return i;

        printf("Invalid\n");
    }
}

int main()
{
    char board[8][8];
    State pState={0,0,0,0,0,0,-2,-2};
    State start={0,0,0,0,0,0,-2,-2};
    char infor[128];
    char line[128];

    int a=0;
    while (1)
    {
        printf("Options:\n\tp:play a game\n\ta:analyse a game\n\te:board editor\n\to:output a game into fime\n\tl:load game from file\n\tx:exit\n");

        if (a)
        {
            printf("Nothing to analyse\n");
            a=0;
        }

        if (!read_line(line,sizeof line))
            break;       
        
        char comm=line[0];

        if (comm=='p')
        {
            int curr=1;
            cp_board(init_board,board);
            pState=start;

            Record* m=play(board,pState,curr);

            printf("Do you want to save the game? y for yes\n");
            read_line(line,sizeof line);
            char cmd=line[0];

            if (cmd=='y')
            {
                printf("Enter info, 128 characters at most:\n");

                if (!read_line(infor,sizeof infor) || infor[0]=='\0')
                    strcpy(infor,"No information");

                if (!store_record(m,infor))
                {
                    printf("Fail to store the game\n");
                    free_record(m);
                }
            }
            else
                free_record(m); 
        }

        if (comm=='a')
        {
            if (lib_count==0)
            {
                a=1;
                system("clear");
                continue;
            }

            SPtr check=lib_record->next;

            for (int i=0;i<lib_count;i++)
            {
                printf("Record %d: %s\n",i+1,check->infor?check->infor:"(no info)");
                check=check->next;
            }

            int i=choose_index(lib_count);
            if (i==0)
                continue;

            check=lib_record;
            for (int j=0;j<i;j++,check=check->next);

            analysis(check->record);
        }

        if (comm=='e')
        {

        }

        if (comm=='o')
        {
            if (lib_count==0)
            {
                a=1;
                continue;
            }

            SPtr check=lib_record->next;

            for (int i=0;i<lib_count;i++)
            {
                printf("Record %d: %s\n",i+1,check->infor?check->infor:"(no info)");
                check=check->next;
            }

            int i=choose_index(lib_count);
            if (i==0)
                continue;

            check=lib_record;
            for (int j=0;j<i;j++,check=check->next);

            printf("Enter file name, 128 characters at most:\n");
            if (!read_line(line,sizeof line) || line[0]=='\0')
            {
                printf("No file name\n");
                continue;
            }

            char out[129+7];
            strcpy(out,"record/");
            strcat(out,line);

            save_record(check->record,out);
        }

        if (comm=='l')
        {
            printf("Enter file name, 128 characters at most:\n");

            /* 整行读取文件名:只按回车视为没输入,也不会被残留的换行/格式串坑到 */
            if (!read_line(line,sizeof line) || line[0]=='\0')
            {
                printf("No file name\n");
                continue;
            }

            char out[129+7];
            strcpy(out,"record/");
            strcat(out,line);

            Record* m=load_record(out);
            if (m==NULL)
            {
                printf("Fail to load the game\n");
                continue;
            }

            printf("Name this game:\n");

            if (!read_line(infor,sizeof infor) || infor[0]=='\0')
                    strcpy(infor,"No information");

            if (!store_record(m,infor))
            {
                printf("Fail to store the game\n");
                free_record(m);
            }
        }
        if (comm=='x')
            break;

        system("clear");
    }

    free_all_records();
    return 0;
}
