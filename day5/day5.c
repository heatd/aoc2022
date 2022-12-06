/* SPDX-License-Identifier: GPL-2.0 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <err.h>

struct stack
{
    char *buf;
    char *end;
    char *pos;
};

void
spush(s, elem)
struct stack *s; char elem;
{
    if (s->end == s->pos)
    {
        char *newsz, *posoff;

        posoff = s->pos - s->buf;

        newsz = (s->end - s->buf) * 2;

        if (newsz == 0)
            newsz = 4;

        s->buf = realloc(s->buf, (unsigned long) newsz * sizeof(char));
        if (!s->buf)
            err(1, "spush");
        
        s->end = s->buf + (unsigned long) newsz;
        s->pos = s->buf + (unsigned) posoff;
    }

    *s->pos++ = elem;
}

char
spop(s)
struct stack *s;
{
    return *--s->pos;
}

void
sinit(s)
struct stack *s;
{
    s->buf = s->pos = s->end = 0;
}

void
srvrse(s)
struct stack *s;
{
    char *it, *mid, *endit;
    it = s->buf, endit = s->pos - 1;

    while (it < endit)
    {
        char c;
        c = *it;
        *it++ = *endit;
        *endit-- = c;
    }
}

void
smov(s0, s1, n)
struct stack *s0, *s1;
{
    int i;
    for (i = 0; i < n; i++)
        spush(s1, spop(s0));
}

void
smov2(s0, s1, n)
struct stack *s0, *s1;
{
    char *pos;
    register i;
    
    pos = s0->pos - n;

    for (i = 0; i < n; i++)
        spush(s1, *pos++);
        
    s0->pos = s0->pos - n;
}

char
speek(s)
struct stack *s;
{
    return s->buf == s->pos ? ' ' : *(s->pos - 1); 
}

#ifdef PART1
main(argc, argv, envp)
char **argv, **envp;
{
    struct stack stacks[9];
    int i;
    char buf[128];

    for (i = 0; i < 9; i++)
        sinit(&stacks[i]);

    for (;;)
    {
        if (!fgets(buf, 128, stdin))
            err(1, "fgets");
        
        if (buf[1] == '1')
        {
            /* we're getting into the line numbers, break */
            break;
        }
        
        for (i = 0; i < 9; i++)
        {
            if (buf[(i<<2)+1] != ' ')
                spush(&stacks[i], buf[(i<<2)+1]);
        }
    }

    for (i = 0; i < 9; i++)
        srvrse(&stacks[i]);

    for (;;)
    {
        auto n, s0, s1;
 
        if (scanf(" move %d from %d to %d", &n, &s0, &s1) != 3)
            break;
        smov(&stacks[s0-1], &stacks[s1-1], n);
    }

    for (i = 0; i < 9; i++)
        putchar(speek(&stacks[i]));
    putchar('\n');
}

#else

main(argc, argv, envp)
char **argv, **envp;
{
    struct stack stacks[9];
    int i;
    char buf[128];

    for (i = 0; i < 9; i++)
        sinit(&stacks[i]);

    for (;;)
    {
        if (!fgets(buf, 128, stdin))
            err(1, "fgets");
        
        if (buf[1] == '1')
        {
            /* we're getting into the line numbers, break */
            break;
        }
        
        for (i = 0; i < 9; i++)
        {
            if (buf[(i<<2)+1] != ' ')
                spush(&stacks[i], buf[(i<<2)+1]);
        }
    }

    for (i = 0; i < 9; i++)
        srvrse(&stacks[i]);

    for (;;)
    {
        auto n, s0, s1;
 
        if (scanf(" move %d from %d to %d", &n, &s0, &s1) != 3)
            break;

        smov2(&stacks[s0-1], &stacks[s1-1], n);
    }

    for (i = 0; i < 9; i++)
        putchar(speek(&stacks[i]));
    putchar('\n');
}

#endif
