/* SPDX-License-Identifier: GPL-2.0 */
#include <stdio.h>
#include <string.h>

#ifdef PART1
countrs(str)
register const char *str;
{
    register len, i, score;
    auto fmap[26 * 2], fmap2[26 * 2];
    const char *rs1, *rs2;

    len = strlen(str);

    if (str[len - 1] == '\n')
        len--;

    rs1 = str;
    rs2 = str + len / 2;

    for (i = 0; i < 26 * 2; ++i)
        fmap[i] = fmap2[i] = 0;

    for (i = 0; i < len / 2; ++i)
    {
        auto index;

        index = rs1[i] >= 'a' ? rs1[i] - 'a' : (rs1[i] - 'A' + 26);
        fmap[index]++;
    }

    for (i = 0; i < len / 2; ++i)
    {
        auto index;

        index = rs2[i] >= 'a' ? rs2[i] - 'a' : (rs2[i] - 'A' + 26);
        fmap2[index]++;
    }

    score = 0;

    for (i = 0; i < 26 * 2; ++i)
    {
        if (fmap[i] && fmap2[i])
            score += i + 1;
    }

    return score;
}

main(argc, argv, envp)
char **argv, **envp;
{
    char buf[128];
    register score;

    score = 0;

    for (;;)
    {
        if (!fgets(buf, 128, stdin))
            break;
        score += countrs(buf);    
    }

    printf("%d\n", score);
}

#else

void
cntfmap(str, fmap)
register const char *str;
int *fmap;
{
    register len, i;
    len = strlen(str);
    if (str[len - 1] == '\n')
        len--;
    for (i = 0; i < 26 * 2; ++i)
        fmap[i] = 0;
    for (i = 0; i < len; ++i)
        fmap[str[i] >= 'a' ? str[i] - 'a' : (str[i] - 'A' + 26)]++;
}

cntbdg(str, str2, str3)
register const char *str, *str2, *str3;
{
    register i;
    auto fmap[26 * 2], fmap2[26 * 2], fmap3[26 * 3];

    cntfmap(str, fmap);
    cntfmap(str2, fmap2);
    cntfmap(str3, fmap3);

    for (i = 0; i < 26 * 2; ++i)
    {
        if (fmap[i] && fmap2[i] && fmap3[i])
            return i + 1;
    }

    fprintf(stderr, "what?? bad input\n");
    exit(1);
}

main(argc, argv, envp)
char **argv, **envp;
{
    char buf[128], buf2[128], buf3[128];
    register score;

    score = 0;

    for (;;)
    {
        if (!fgets(buf, 128, stdin))
            break;
        if (!fgets(buf2, 128, stdin))
            break;
        if (!fgets(buf3, 128, stdin))
            break;
        score += cntbdg(buf, buf2, buf3);    
    }

    printf("%d\n", score);
}

#endif
