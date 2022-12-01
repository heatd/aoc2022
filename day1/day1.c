/* SPDX-License-Identifier: GPL-2.0 */
#include <limits.h>
#include <stdio.h>

getnum()
{
    char c;
    int n;

    n = 0;

    while ((c = getchar()) != EOF)
    {
        if (isspace(c))
            break;
        if (!isdigit(c))
        {
            printf("bad input on %x\n", c);
            exit(1);
        }

        n *= 10;
        n += c - '0';
    }

    return n;
}

void addscore(bscores, n, score)
int *bscores;
{
    int i;
    int smaller;
    int j;

    j = -1;
    smaller = INT_MAX;

    for (i = 0; i < n; ++i)
    {
        int s;
        s = bscores[i];

        if (smaller > s)
            smaller = s, j = i;
    }


    if (j != -1 && smaller < score)
    {
        bscores[j] = score;
    }
}

main(argc, argv, envp)
char **argv, **envp;
{
    int bscores[3];
    int curscore;
    int i;

    for (i = 0; i < 3; ++i)
        bscores[i] = 0;
    curscore = 0;

    for (;;)
    {
        int score;

        score = getnum();

        curscore += score;

        if (score == 0)
        {
            char c;

            if (curscore == 0)
            {
                /* EOF - break and print result */
                goto out;
            }

            addscore(bscores, 3, curscore);
            curscore = 0;
        }
    }

out:
    curscore = 0;
    for (i = 0; i < 3; ++i)
        curscore += bscores[i];
    printf("%d\n", curscore);
}
