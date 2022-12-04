/* SPDX-License-Identifier: GPL-2.0 */
#include <stdio.h>
#include <string.h>

#ifdef PART1
main(argc, argv, envp)
char **argv, **envp;
{
    register score;

    score = 0;

    for (;;)
    {
        unsigned x1, y1, x2, y2;
        if (scanf(" %d-%d,%d-%d", &x1, &y1, &x2, &y2) != 4)
            break; /* EOF */
        score += (x1 <= x2 && y1 >= y2) || (x2 <= x1 && y2 >= y1); 
    }

    printf("%d\n", score);
}

#else

main(argc, argv, envp)
char **argv, **envp;
{
    register score;

    score = 0;

    for (;;)
    {
        unsigned x1, y1, x2, y2;
        if (scanf(" %d-%d,%d-%d", &x1, &y1, &x2, &y2) != 4)
            break; /* EOF */
        score += (x1 <= y2 && x2 <= y1); 
    }

    printf("%d\n", score);
}

#endif
