/* SPDX-License-Identifier: GPL-2.0 */
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <err.h>
#include <semaphore.h>

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
    auto fmap[26 * 2], fmap2[26 * 2], fmap3[26 * 2];

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
        if(!a->work)
            a->work = wo;
        else
        {
            struct work *b = a->work;
            while (b->nextwq) b = b->nextwq;
            b->nextwq = wo;
        }
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
