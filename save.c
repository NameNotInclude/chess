#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "save.h"   /* 内含 piece.h(State)与 game.h(Record) */

#define SAVE_LINE_MAX  256   /* 一行的最大长度 */
#define SAVE_TOKEN_MAX 128   /* 单个着法串(detail_move / move)的最大长度 */

int save_record(Record* M, const char* path)
{
    if (M == NULL || path == NULL)
        return 0;

    FILE* fp = fopen(path, "w");
    if (fp == NULL)
        return 0;

    fprintf(fp, "# chess record v1\n");

    /* 从 head->next 开始:head 只是哨兵,两个字符串都是 NULL */
    for (MPtr p = M->head->next; p != NULL; p = p->next)
    {
        const char* detail = (p->detail_move != NULL) ? p->detail_move : "-";
        const char* stan   = (p->move != NULL)        ? p->move        : "-";
        fprintf(fp, "%s %s\n", detail, stan);
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
