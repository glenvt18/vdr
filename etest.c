#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#include "epg.h"


#define TM_CLKID  CLOCK_THREAD_CPUTIME_ID

#define T_LEN 50
#define S_LEN 100
#define D_LEN 400
#define A_LEN 20

#define N_EVENTS 30 * 1000
//#define N_EVENTS 1

static char *longString(int len, char ch)
{
    char *s = new char [len + 1];
    for (int i = 0; i < len; i++)
        s[i] = ch;
    s[len] = '\0';

    return s;
}


int main() {
    cEvent ev(123456);
    ev.SetTableID(0x1f);
    ev.SetVersion(0x2f);
    ev.SetTitle(longString(T_LEN, 'T'));
    ev.SetShortText(longString(S_LEN, 'S'));
    ev.SetDescription(longString(D_LEN, 'D'));
    ev.SetStartTime(time(NULL));
    ev.SetDuration(3600);
    ev.SetVps(time(NULL));
    ev.SetAux(longString(A_LEN, 'A'));
    ev.SetContents((uchar *)("\x14\xA0"));
    ev.SetParentalRating(15);

    //ev.SetDescription(strdup("Desc\nription"));
    //ev.SetAux(strdup("A\nux\n"));

    cComponents *cc = new(cComponents);
    cc->SetComponent(cc->NumComponents(), 0x05, 0x0b, "eng", "video AVC");
    cc->SetComponent(cc->NumComponents(), 0x02, 0x03, "eng", "audio MPEG1");
    cc->SetComponent(cc->NumComponents(), 0x04, 0x01, "eng", "audio AC3");
    cc->SetComponent(cc->NumComponents(), 0x07, 0x01, "eng", "audio DTS");
    ev.SetComponents(cc);

    struct timespec t1, t2;

    clock_gettime(TM_CLKID, &t1);
    for (int i = 0; i < N_EVENTS; i++) {
        ev.Dump_orig(stdout, "215-");
    }
    clock_gettime(TM_CLKID, &t2);
    int64_t diff = (t2.tv_sec * 1000 * 1000 + t2.tv_nsec / 1000) -
                 (t1.tv_sec * 1000 * 1000 + t1.tv_nsec / 1000);

    fprintf(stderr, "Original: %ld mS\n", diff / 1000);

    clock_gettime(TM_CLKID, &t1);
    for (int i = 0; i < N_EVENTS; i++) {
        ev.Dump(stdout, "215-");
    }
    clock_gettime(TM_CLKID, &t2);
    diff = (t2.tv_sec * 1000 * 1000 + t2.tv_nsec / 1000) -
                 (t1.tv_sec * 1000 * 1000 + t1.tv_nsec / 1000);

    fprintf(stderr, "Optimized: %ld mS\n", diff / 1000);

    return 0;
}
