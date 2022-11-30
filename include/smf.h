/*
 * Copyright 2021 The Chromium OS Authors
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/* State Machine Framework */

#ifndef INCLUDE_SMF_H_
#define INCLUDE_SMF_H_

#ifdef SMF_CONFIG
#define SMF_STRINGIFY(x) #x
#define SMF_STR(x) SMF_STRINGIFY(x)
#include SMF_STR(SMF_CONFIG)
#endif

#define __ARG_PLACEHOLDER_1 0,
#define __take_second_arg(__ignored, val, ...) val

/*
 * Getting something that works in C and CPP for an arg that may or may
 * not be defined is tricky.  Here, if we have "#define CONFIG_BOOGER 1"
 * we match on the placeholder define, insert the "0," for arg1 and generate
 * the triplet (0, 1, 0).  Then the last step cherry picks the 2nd arg (a one).
 * When CONFIG_BOOGER is not defined, we generate a (... 1, 0) pair, and when
 * the last step cherry picks the 2nd arg, we get a zero.
 */
#define __is_defined(x) ___is_defined(x)
#define ___is_defined(val) ____is_defined(__ARG_PLACEHOLDER_##val)
#define ____is_defined(arg1_or_junk) __take_second_arg(arg1_or_junk 1, 0)

#define IS_BUILTIN(option) __is_defined(option)
#define IS_ENABLED(option) IS_BUILTIN(option)

#ifdef CONFIG_SMF_ANCESTOR_SUPPORT
/**
 * @brief Macro to create a hierarchical state.
 *
 * @param _entry  State entry function
 * @param _run    State run function
 * @param _exit   State exit function
 * @param _parent State parent object or NULL
 */
#define SMF_CREATE_STATE(_entry, _run, _exit, _parent)                         \
    {                                                                          \
        .entry = _entry, .run = _run, .exit = _exit, .parent = _parent         \
    }

#else

/**
 * @brief Macro to create a flat state.
 *
 * @param _entry  State entry function
 * @param _run  State run function
 * @param _exit  State exit function
 */
#define SMF_CREATE_STATE(_entry, _run, _exit)                                  \
    {                                                                          \
        .entry = _entry, .run = _run, .exit = _exit                            \
    }

#endif /* CONFIG_SMF_ANCESTOR_SUPPORT */

/**
 * @brief Macro to cast user defined object to state machine
 *        context.
 *
 * @param o A pointer to the user defined object
 */
#define SMF_CTX(o) ((struct smf_ctx *)o)

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**
 * @brief Function pointer that implements a portion of a state
 *
 * @param obj pointer user defined object
 */
typedef void (*state_execution)(void *obj);

/** General state that can be used in multiple state machines. */
struct smf_state {
    /** Optional method that will be run when this state is entered */
    const state_execution entry;
    /**
     * Optional method that will be run repeatedly during state machine
     * loop.
     */
    const state_execution run;
    /** Optional method that will be run when this state exists */
    const state_execution exit;
    /**
     * Optional parent state that contains common entry/run/exit
     *	implementation among various child states.
     *	entry: Parent function executes BEFORE child function.
     *	run:   Parent function executes AFTER child function.
     *	exit:  Parent function executes AFTER child function.
     *
     *	Note: When transitioning between two child states with a shared parent,
     *	that parent's exit and entry functions do not execute.
     */
    const struct smf_state *parent;
};

/** Defines the current context of the state machine. */
struct smf_ctx {
    /** Current state the state machine is executing. */
    const struct smf_state *current;
    /** Previous state the state machine executed */
    const struct smf_state *previous;
    /**
     * This value is set by the set_terminate function and
     * should terminate the state machine when its set to a
     * value other than zero when it's returned by the
     * run_state function.
     */
    int32_t terminate_val;
    /**
     * The state machine casts this to a "struct internal_ctx" and it's
     * used to track state machine context
     */
    uint32_t internal;
};

/**
 * @brief Initializes the state machine and sets its initial state.
 *
 * @param ctx        State machine context
 * @param init_state Initial state the state machine starts in.
 */
void smf_set_initial(struct smf_ctx *ctx, const struct smf_state *init_state);

/**
 * @brief Changes a state machines state. This handles exiting the previous
 *        state and entering the target state. A common parent state will not
 *        exited nor be re-entered.
 *
 * @param ctx       State machine context
 * @param new_state State to transition to (NULL is valid and exits all states)
 */
void smf_set_state(struct smf_ctx *ctx, const struct smf_state *new_state);

/**
 * @brief Terminate a state machine
 *
 * @param ctx  State machine context
 * @param val  Non-Zero termination value that's returned by the smf_run_state
 *             function.
 */
void smf_set_terminate(struct smf_ctx *ctx, int32_t val);

/**
 * @brief Runs one iteration of a state machine (including any parent states)
 *
 * @param ctx  State machine context
 * @return	   A non-zero value should terminate the state machine. This
 *			   non-zero value could represent a terminal state being
 *reached or the detection of an error that should result in the termination of
 *the state machine.
 */
int32_t smf_run_state(struct smf_ctx *ctx);

#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_SMF_H_ */