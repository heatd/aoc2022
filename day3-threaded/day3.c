/* SPDX-License-Identifier: GPL-2.0 */
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <err.h>
#include <semaphore.h>
#include <stdint.h>

typedef uint64_t u64;

u64
cntfmap(str)
register const char *str;
{
    u64 fmap;
    register len, i;
    len = strlen(str);
    if (str[len - 1] == '\n')
        len--;
    fmap = 0;
    for (i = 0; i < len; ++i)
        fmap |= (1UL << (str[i] >= 'a' ? str[i] - 'a' : (str[i] - 'A' + 26)));
    return fmap;
}

cntbdg(str, str2, str3)
register const char *str, *str2, *str3;
{
    register i;
    u64 fmap, fmap2, fmap3, total;

    fmap = cntfmap(str);
    fmap2 = cntfmap(str2);
    fmap3 = cntfmap(str3);

    total = fmap & fmap2 & fmap3;

    return __builtin_ffsll(total);
}

struct work
{
    char buf[128];
    char buf2[128];
    char buf3[128];
    struct work *nextwq;
};

struct wq
{
    pthread_t thread;
    sem_t sem;
    struct work *work;
    int score;
    int stop;
    pthread_spinlock_t worklock;
};

struct wq wq[8];

void *thr_main(void *arg)
{
    unsigned long id = (unsigned long) arg;

    for (;;)
    {
        sem_wait(&wq[id].sem);

        if (wq[id].stop && !wq[id].work)
            break;
        pthread_spin_lock(&wq[id].worklock);
        struct work *w = wq[id].work;
        wq[id].work = w->nextwq;
        pthread_spin_unlock(&wq[id].worklock);
        wq[id].score += cntbdg(w->buf, w->buf2, w->buf3);

        free(w);
    }

    return NULL;
}

main(argc, argv, envp)
char **argv, **envp;
{
    register score;

    score = 0;

    for (int i = 0; i < 8; i++)
    {
        sem_init(&wq[i].sem, 0, 0);
        pthread_spin_init(&wq[i].worklock, 0);
        int st = pthread_create(&wq[i].thread, NULL, thr_main, (void *) (long) i);
        if (st < 0)
            err(1, "pthread_create");
    }

    int id = 0;

    for (;;)
    {
        struct work *wo = malloc(sizeof(*wo));
        if (!wo)
            return 1;

        if (!fgets(wo->buf, 128, stdin))
            break;
        if (!fgets(wo->buf2, 128, stdin))
            break;
        if (!fgets(wo->buf3, 128, stdin))
            break;
        wo->nextwq = NULL;

        struct wq *a = &wq[id % 8];
        pthread_spin_lock(&a->worklock);
        wo->nextwq = a->work, a->work = wo;
        pthread_spin_unlock(&a->worklock);
        sem_post(&a->sem);
    }

    for (int i = 0; i < 8; i++)
    {
        wq[i].stop = 1;
        sem_post(&wq[i].sem);
        pthread_join(wq[i].thread, NULL);
        score += wq[i].score;
    }

    printf("%d\n", score);
}
