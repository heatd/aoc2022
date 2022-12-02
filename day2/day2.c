/* SPDX-License-Identifier: GPL-2.0 */
#include <limits.h>
#include <stdio.h>

#ifdef PART1
#define ROCK     0
#define PAPER    1
#define SCISSORS 2

wins(other, mine)
int other, mine;
{
    return (other == ROCK && mine == PAPER) ||
           (other == SCISSORS && mine == ROCK) ||
           (other == PAPER && mine == SCISSORS);
}

main(argc, argv, envp)
char **argv, **envp;
{
    char A, X;
    register score;

    score = 0;

    while (scanf(" %c %c", &A, &X) != EOF)
    {
        register int a, b;

        a = A - 'A';
        b = X - 'X';

        score += b + 1 + (wins(a, b) ? 6 : (a == b ? 3 : 0));    
    }

out:
    printf("%d\n", score);
}

#else

#define ROCK     0
#define PAPER    1
#define SCISSORS 2
pick(other, res)
register other, res;
{
    register int n;
    n = other + (res == 2 ? 1 : -1);
    return res == 1 ? other : (n < 0 ? 3 + n : n) % 3;
}

main(argc, argv, envp)
char **argv, **envp;
{
    char A, X;
    register score;

    score = 0;

    while (scanf(" %c %c", &A, &X) != EOF)
    {
        register int a, res;

        a = A - 'A';
        res = X - 'X';

        score += res * 3 + pick(a, res) + 1;
    }

out:
    printf("%d\n", score);
}

#endif
