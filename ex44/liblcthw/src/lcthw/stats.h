#ifndef lcthw_stats_h
#define lcthw_stats_h

#include <math.h>

typedef struct Stats {
    double sum;
    double sumsq;
    unsigned long n;
    double min;
    double max;
} Stats;

Stats *Stats_recreate(double sum, double sumsq, unsigned long n, double min, double max);

Stats *Stats_create();

static inline double Stats_mean(Stats *st)
{
    return st->sum / st->n;
}

static inline double Stats_stdev(Stats *st)
{
    return sqrt( (st->sumsq - (st->sum * st->sum / st->n)) / (st->n - 1) );
}


void Stats_sample(Stats *st, double s);

void Stats_dump(Stats *st);

#endif
