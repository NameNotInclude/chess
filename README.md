# chess

用 C 写的命令行国际象棋程序:支持完整走法判定(含将军/将杀/逼和)、易位、吃过路兵、升变,
能记录棋谱(链表保存 + 每局一句说明)、把棋谱导出/读入文本文件,并支持复盘分析(可挂变例)。

## 编译与运行

```sh
gcc main.c board.c piece.c game.c mode.c save.c -o chess
./chess
```

- 只依赖标准 C 库(gcc / C99 即可),没有第三方依赖。
- 编译建议加 `-Wall -Wextra`;棋谱文件默认放在 `record/` 目录下(该目录需存在)。

## 目录与模块

| 文件 | 作用 |
| --- | --- |
| `main.c` | 菜单与整体流程(p/a/e/o/l/x),输入的整行读取处理 |
| `piece.c/.h` | 走法判定 `valid_move`、行棋 `move`、SAN 生成 `transform`、状态更新 `State_Update`、`State_Print` |
| `board.c/.h` | 棋盘、初始局面 `init_board`、打印 `board_print`、复制 `cp_board` |
| `game.c/.h` | `check` / `no_legal_move`,棋谱链表 `Record`/`Move`(`init`/`add`/`free_record`/`print_record`) |
| `mode.c/.h` | 对局模式 `play`、复盘模式 `analysis` |
| `save.c/.h` | 进程内棋谱库链表 + 棋谱文件保存/读取(`store_record`/`record_list`/`get_record`/`save_record`/`load_record`) |
| `test/` | 测试棋谱(每行一个坐标着法) |
| `record/` | 棋谱存档文件目录(已在 `.gitignore` 中忽略) |

## 棋盘与着法格式

- 棋盘 `char map[8][8]`:`map[0]` 是第 8 横线(黑方底线),`map[7]` 是第 1 横线(白方底线),列 0~7 对应 a~h。
- 大写字母 = 白方棋子,小写字母 = 黑方棋子,`'-'` = 空位。
- 程序输入的着法是"起点 + 终点",如 `e2e4`;易位写 `O-O` / `O-O-O`(小写 `o-o` 也可以)。
- 升变在对局中是交互输入:兵走到最后一格后会出现 `Choose to promote:`,输入 `q`/`r`/`b`/`n`。
- 复盘/输出用的是标准代数记法(SAN),由 `transform()` 生成,例如 `e4`、`Nf3`、`exd6`、`O-O`、`f8=Q#`。

## 主菜单

```
Options:
	p:play a game
	a:analyse a game
	e:board editor
	o:output a game into fime
	l:load game from file
	x:exit
```

| 命令 | 说明 |
| --- | --- |
| `p` | 下一局;结束后可保存并给这局棋谱写一句说明(直接回车记为 `No information`) |
| `a` | 从棋谱库中选一份进入复盘模式 |
| `e` | 棋盘编辑器(**尚未实现**) |
| `o` | 把棋谱库中某一份导出到 `record/<文件名>` |
| `l` | 从 `record/<文件名>` 读入棋谱,并给它命名后存入棋谱库 |
| `x` | 退出(退出时统一释放棋谱库) |

所有需要输入的地方都是**整行读取**:只按回车当作"没输入"(菜单里忽略、是否保存视为否、说明用默认值、文件名提示重新输入),不会把回车当成指令,也不会吞掉后面的字符。

## 对局状态 State

`State` 记录易位与吃过路兵相关的状态:

- `WHITE_KING_MOVE` / `BLACK_KING_MOVE`、`WHITE_A_ROOK_MOVE` / `WHITE_H_ROOK_MOVE` / `BLACK_A_ROOK_MOVE` / `BLACK_H_ROOK_MOVE`
- `WHITE_EN` / `BLACK_EN`(最近一次二格进兵所在列,`-2` 表示没有)

`move()` 只负责走子和返回一个"着法类型码",真正的状态更新由 `State_Update(comm, player, &pState)` 完成;所有相关函数都用 `State*` 传递,保证调用方的状态能被改到。

## 棋谱数据结构与保存

棋谱是一串着法节点:

```c
typedef struct node
{
    char* detail_move;   // 程序输入格式,如 e2e4 / O-O
    char* move;          // 标准记法,如 e4 / Nf3
    struct node* next;   // 下一招(主变)
    struct node* prev;
    struct node* next_varr;   // 同一局面下的其它走法(变例)
    struct node* prev_varr;
} Move;

typedef struct { MPtr head; MPtr last; } Record;
```

**进程内棋谱库**(`save.c`)用带哨兵头节点的单链表保存多局棋谱,每个节点带一句说明:

```c
typedef struct n
{
    Record* record;   // 棋谱
    char*   infor;    // 这局棋谱的说明
    struct n* next;
} Save;
```

`store_record(M, infor)` 把棋谱挂到链表尾(说明会被复制一份),`record_list()` / `get_record(index)` 取出原指针直接用,`free_all_records()` 统一释放棋谱 + 说明 + 节点。

**棋谱文件**是纯文本,一行一步,`#` 开头为注释;第一列是程序输入着法,第二列是 SAN:

```
# chess record v1
e2e4 e4
e7e5 e5
g1f3 Nf3
O-O O-O
```

- `save_record(M, path)` 保存:`Record` 里同一局面有多条着法(`next_varr`)时会列出分支让你选保留哪一条,输入结束或非法输入则默认保留主线。
- `load_record(path)` 读回一份**新的** `Record`(字符串是重新分配的内存),用完要 `free_record`。

## 复盘模式 analysis

- 逐个局面显示当前所有变例:`Variation 1 : Move e2e4 , standard move : e4`。
- 命令:
  - `n`:输入变例编号,把该步走到棋盘上(升变子从 SAN 里的 `=X` 解析);
  - `p`:从当前局面继续对弈,下完的这一段会接成当前着法的变例;
  - `x`:退出复盘。

## 测试棋谱

`test/` 下有若干棋谱文件(如 `castle.txt`、`checkmate.txt`、`enpass.txt`),每行一个坐标着法,可以直接粘进对局输入里测试易位、将杀、吃过路兵等。

## 分支

- `master`:主分支(远端 `origin/master`);
- `dev/record`:棋谱记录相关开发;
- `feat/mode`:模式选择/对局模式相关开发。

## 已知问题 / 待办

- 菜单里的 `e`(棋盘编辑器)还没实现。
- `analysis()` 里 `while (p!='\0' && *p!='=')` 应为 `*p!='\0'`(编译有 `-Wpointer-compare` 警告,目前靠 `*p!='='` 也能退出循环)。
- `free_record()` 目前只释放主变(`next`);变例(`next_varr`)启用后会漏内存,需要一并遍历释放。
- 对局中的升变是交互输入,复盘回放升变依赖 SAN 里的 `=Q` 之类写法。
