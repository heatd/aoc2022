/* SPDX-License-Identifier: GPL-2.0 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <err.h>

#ifdef PART1
#define TOFIND 4
#else
#define TOFIND 14
#endif

main(argc, argv, envp)
char **argv, **envp;
{
    char buf[4096 + 2];
    int fmap[26];
    int i, start, len;

    start = 0;

    for (i = 0; i < 26; i++)
        fmap[i] = 0;
    
    if (!fgets(buf, sizeof(buf), stdin))
        err(1, "fgets");

    if (buf[4096] == '\n')
        buf[4096] = 0;

    len = strlen(buf);

    for (i = 0; i < len; i++)
    {
        int n;

        n = buf[i] - 'a';

        if (fmap[n])
        {
            for (; start < i; start++)
            {
                if (buf[start] == buf[i])
                {
                    start++;
                    break;
                }

                fmap[buf[start] - 'a']--;
            }
        }
        else
            fmap[n]++;
        
        if (i - start == TOFIND - 1)
            break;
    }

    printf("%i\n", start + TOFIND);
}
