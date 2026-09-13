#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "save.h"   /* 内含 piece.h(State)与 game.h(Record) */

#define SAVE_LINE_MAX  256   /* 一行的最大长度 */
#define SAVE_TOKEN_MAX 128   /* 单个着法串(detail_move / move)的最大长度 */

/* ==================== 进程内棋谱库 ==================== */

#define LIB_INIT_CAP 4       /* 棋谱库初始容量 */

static Record** lib_records = NULL;   /* 保存棋谱指针的数组 */
static int lib_count = 0;             /* 已保存的棋谱份数 */
static int lib_cap = 0;               /* 数组容量 */

int record_count(void)
{
    return lib_count;
}

Record* get_record(int index)
{
    if (index < 0 || index >= lib_count)
        return NULL;
    return lib_records[index];        /* 直接返回库里的指针,不复制 */
}

int store_record(Record* M)
{
    if (M == NULL)
        return 0;

    if (lib_count == lib_cap)
    {
        int newcap = (lib_cap == 0) ? LIB_INIT_CAP : lib_cap * 2;
        Record** tmp = (Record**)realloc(lib_records, sizeof(Record*) * newcap);
        if (tmp == NULL)
            return 0;
        lib_records = tmp;
        lib_cap = newcap;
    }

    lib_records[lib_count++] = M;
    return 1;
}

void free_all_records(void)
{
    for (int i = 0; i < lib_count; i++)
        free_record(lib_records[i]);

    free(lib_records);
    lib_records = NULL;
    lib_count = 0;
    lib_cap = 0;
}

/* ==================== 文件保存 / 读取 ==================== */

/*
 * 同一局面下有几条着法时,让用户选一条保留。
 * first 是主线着法(候选 1),其余候选通过 next_varr 串在后面。
 * 返回 1..count;输入结束(EOF)或输入非法时返回 1,即默认保留主线。
 */
static int ask_branch(MPtr first, int count)
{
    printf("find %d branch,choose one to save:\n", count);

    int idx = 1;
    for (MPtr p = first; p != NULL; p = p->next_varr)
    {
        printf("  %d) %s  %s\n", idx,
               (p->detail_move != NULL) ? p->detail_move : "-",
               (p->move != NULL)        ? p->move        : "-");
        idx++;
    }

    printf("Enter(1-%d, invalid input will be regarded as first branch): ", count);

    int choice = 1;
    if (scanf(" %d", &choice) != 1)
        choice = 1;                  /* EOF:不卡死,保留主线 */
    if (choice < 1 || choice > count)
        choice = 1;

    return choice;
}

int save_record(Record* M, const char* path)
{
    if (M == NULL || path == NULL)
        return 0;

    FILE* fp = fopen(path, "w");
    if (fp == NULL)
        return 0;

    fprintf(fp, "# chess record v1\n");

    MPtr cur = M->head;
    while (cur != NULL)
    {
        MPtr first = cur->next;      /* 当前局面下的主线着法 */
        if (first == NULL)
            break;                   /* 没有后续着法,棋谱结束 */

        int count = 0;
        for (MPtr p = first; p != NULL; p = p->next_varr)
            count++;                 /* 主线 + 各变例 */

        MPtr pick = first;
        if (count > 1)
        {
            int choice = ask_branch(first, count);
            for (int i = 1; i < choice && pick != NULL; i++)
                pick = pick->next_varr;
        }

        fprintf(fp, "%s %s\n",
                (pick->detail_move != NULL) ? pick->detail_move : "-",
                (pick->move != NULL)        ? pick->move        : "-");

        cur = pick;                  /* 沿选中的分支继续往后走 */
    }

    if (fclose(fp) != 0)
        return 0;
    return 1;
}

Record* load_record(const char* path)
{
    if (path == NULL)
        return NULL;

    FILE* fp = fopen(path, "r");
    if (fp == NULL)
        return NULL;

    Record* M = init();
    if (M == NULL)
    {
        fclose(fp);
        return NULL;
    }

    char line[SAVE_LINE_MAX];

    while (fgets(line, sizeof line, fp) != NULL)
    {
        /* 跳过行首空白,便于识别空行和注释行 */
        char* p = line;
        while (*p == ' ' || *p == '\t' || *p == '\r' || *p == '\n')
            p++;

        if (*p == '\0' || *p == '#')
            continue;

        char detail[SAVE_TOKEN_MAX];
        char stan[SAVE_TOKEN_MAX];

        /* 需要“着法 SAN”两个字段;格式不对的行直接跳过 */
        if (sscanf(p, "%127s %127s", detail, stan) != 2)
            continue;

        char* d = (char*)malloc(strlen(detail) + 1);
        char* s = (char*)malloc(strlen(stan) + 1);
        if (d == NULL || s == NULL)
        {
            free(d);
            free(s);
            free_record(M);   /* 已经读入的部分一并释放,避免泄漏 */
            fclose(fp);
            return NULL;
        }
        strcpy(d, detail);
        strcpy(s, stan);

        add(M, d, s);         /* 所有权交给棋谱,由 free_record 统一释放 */
    }

    fclose(fp);
    return M;
}
