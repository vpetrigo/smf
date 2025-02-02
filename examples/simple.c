/**
 * \file
 * \brief Simple state machine demonstration
 * \author Vladimir Petrigo
 */
#include <smf.h>

#include <stdio.h>
#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#define sleep(x) Sleep((x) * 1000)
#else
#include <unistd.h>
#endif

/* Forward declaration of state table */
static extern const struct smf_state demo_states[];

/* List of demo states */
enum demo_state
{
    S0,
    S1,
    S2
};

/* User defined object */
struct s_object {
    /* This must be first */
    struct smf_ctx ctx;

    /* Other state specific data add here */
} s_obj;

/* State S0 */
static void s0_entry(void *o)
{
    /* Do something */
    puts(__func__);
}

static void s0_run(void *o)
{
    puts(__func__);
    smf_set_state(SMF_CTX(&s_obj), &demo_states[S1]);
}

static void s0_exit(void *o)
{
    /* Do something */
    puts(__func__);
}

/* State S1 */
static void s1_run(void *o)
{
    puts(__func__);
    smf_set_state(SMF_CTX(&s_obj), &demo_states[S2]);
}

static void s1_exit(void *o)
{
    /* Do something */
    puts(__func__);
}

/* State S2 */
static void s2_entry(void *o)
{
    /* Do something */
    puts(__func__);
}

static void s2_run(void *o)
{
    static unsigned count = 0;
    struct smf_ctx *ctx = SMF_CTX(o);

    puts(__func__);

    if (count++ == 3) {
        smf_set_terminate(ctx, 1);
        puts("Terminating state machine");
    }
    else {
        smf_set_state(SMF_CTX(&s_obj), &demo_states[S0]);
    }
}

/* Populate state table */
static const struct smf_state demo_states[] = {
    [S0] = SMF_CREATE_STATE(s0_entry, s0_run, s0_exit, NULL, NULL),
    /* State S1 does not have an entry action */
    [S1] = SMF_CREATE_STATE(NULL, s1_run, s1_exit, NULL, NULL),
    /* State S2 does not have an exit action */
    [S2] = SMF_CREATE_STATE(s2_entry, s2_run, NULL, NULL, NULL),
};

int main(void)
{
    /* Set initial state */
    smf_set_initial(SMF_CTX(&s_obj), &demo_states[S0]);

    /* Run the state machine */
    while (1) {
        /* State machine terminates if a non-zero value is returned */
        const int32_t ret = smf_run_state(SMF_CTX(&s_obj));

        if (ret) {
            /* handle return code and terminate state machine */
            break;
        }

        sleep(1);
    }
}