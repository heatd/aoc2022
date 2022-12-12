/* SPDX-License-Identifier: GPL-2.0 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <err.h>
#include <ctype.h>
#include <stddef.h>

struct list_head
{
    struct list_head *prev, *next;
};

#define LIST_HEAD_INIT(lh) {&(lh), &(lh)}

static void init_list_head(struct list_head *lh)
{
    lh->next = lh->prev = lh;
}

static inline void __list_add(struct list_head *_new, struct list_head *prev,
                              struct list_head *next)
{
    next->prev = _new;
    _new->next = next;
    _new->prev = prev;
    prev->next = _new;
}

static inline void list_add(struct list_head *_new, struct list_head *head)
{
    __list_add(_new, head, head->next);
}

static inline void list_add_tail(struct list_head *_new, struct list_head *head)
{
    __list_add(_new, head->prev, head);
}

static inline void list_remove_bulk(struct list_head *prev, struct list_head *next)
{
    prev->next = next;
    next->prev = prev;
}

static inline void list_remove(struct list_head *node)
{
    list_remove_bulk(node->prev, node->next);
    node->prev = node->next = NULL;
}

#define container_of(ptr, type, member) ((type *) ((char *) ptr - offsetof(type, member)))
#define list_for_every(lh) for (l = (lh)->next; l != (lh); l = l->next)

struct file
{
    char *name;
    struct file *parent;
    struct list_head children, node, node2;
    int size;
    u_char dir;
};

static struct list_head dirl = LIST_HEAD_INIT(dirl);

struct file *
openf(dir, name)
struct file *dir;
const char *name;
{
    struct list_head *l;

    if (!dir->dir)
        errx(1, "%s not dir\n", dir->name);

    if (!strcmp(name, ".."))
        return dir->parent;

    list_for_every(&dir->children)
    {
        struct file *f;

        f = container_of(l, struct file, node);
        
        if (!strcmp(f->name, name))
            return f;
    }

    errx(1, "%s not found\n", name);
}

void
creatf(dir)
struct file *dir;
{
    char buf[128];
    for (;;)
    {
        char c, *elem;
        u_char isdir;
        int size;
        struct file *f;

        isdir = 0;
        size = 0;
        
        c = fgetc(stdin);

        if (c == EOF)
            break;

        /* While first char != $, this is ls output so create files */
        ungetc(c, stdin);
        if (c == '$')
            break;
        
        if (!fgets(buf, sizeof buf, stdin))
            err(1, "fgets");
        
        if (buf[strlen(buf) - 1] == '\n')
            buf[strlen(buf) - 1] = '\0';
        
        elem = strtok(buf, " ");

        if (*elem == 'd')
            isdir = 1;
        else
            sscanf(elem, "%d", &size);

        f = malloc(sizeof *f);
        f->name = strdup(strtok(NULL, " "));
        f->parent = dir;
        init_list_head(&f->children);
        f->dir = isdir;

        if (isdir)
            list_add_tail(&f->node2, &dirl);
        f->size = size;
        list_add_tail(&f->node, &dir->children);
        /* propagate size */
        if (size)
        {
            struct file *d;

            for (d = dir; d; d = d->parent)
                d->size += size;
        }
    }
}

main(argc, argv)
char **argv;
{
    char buf[128];
    struct list_head *l;
    struct file root;
    struct file *cwd;
    enum
    {
        CMD_CD = 0,
        CMD_LS
    } cmd;

    int score, unused;

    score = unused = 0;

    root.name = "/";

    root.parent = NULL;
    root.dir = 1;
    root.size = 0;
    init_list_head(&root.children);

    cwd = NULL;

    for (;;)
    {
        char *b;

        if (!fgets(buf, sizeof buf, stdin))
            break;
        
        if (buf[strlen(buf) - 1] == '\n')
            buf[strlen(buf) - 1] = '\0';

        if (*buf != '$')
            errx(1, "program bug: didn't consume all the output");

        b = buf + 1;

        while(isspace(*b)) b++;

        if (!strncmp(b, "cd", strlen("cd")))
        {
            cmd = CMD_CD;
            b += 3;
        }
        else if (!strncmp(b, "ls", strlen("ls")))
        {
            cmd = CMD_LS;
            b += 3;
        }
        else
            errx(1, "unknown cmd %s\n", b);
        
        if (cmd == CMD_CD)
        {
            if (!strcmp(b, "/"))
            {
                cwd = &root;
            }
            else
            {
                cwd = openf(cwd, b);
            }
        }
        else if (cmd == CMD_LS)
        {
            creatf(cwd);
        }
    }

#ifdef PART1
    list_for_every(&dirl)
    {
        struct file *f;

        f = container_of(l, struct file, node2);

        if (f->size <= 100000)
            score += f->size;
        printf("%s size %u\n", f->name, f->size);
    }
#else
    score = 70000000;
    unused = 70000000 - root.size;

    list_for_every(&dirl)
    {
        struct file *f;

        f = container_of(l, struct file, node2);

        if (f->size + unused >= 30000000)
            score = f->size < score ? f->size : score;
    }
#endif

    printf("%d\n", score);
}
