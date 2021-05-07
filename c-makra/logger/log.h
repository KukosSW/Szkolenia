#ifndef LOG_H
#define LOG_H

/*
	Modes to debug:
	DEBUG_MODE -> if defined, log works

	TRACE_MODE -> if defined trace_call works

    SILENT_ERROR -> if defined error logged errors

    Usage:
    1. First init logs: log_init
        IF you want to store logs in file pass LOG_TO_FILE to function
    2. Normal usage: TRACE, LOG, ERROR in your functions
    3. Clean log system: log_deinit

*/

#include <stdio.h> /* FILE* */
#include <string.h>
#include <stdlib.h>

#define PATH_TO_FILE (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#define LOG_LEVEL_ALWAYS  0
#define LOG_LEVEL_CRIT    1
#define LOG_LEVEL_WARNING 2
#define LOG_LEVEL_INFO    3
#define LOG_LEVEL_NOTICE  4
#define LOG_LEVEL_DEBUG   5

#ifndef DEBUG_MODE
    #define DEBUG_LVL 0
#else
    #define DEBUG_LVL DEBUG_MODE
#endif

#define FATAL_EXIT_CODE 1

/* PRIVATE MACROS TO GET [H | T ] from __VA_ARGS__ */
#define LOG_ARGS_HEAD(...) _LOG_ARGS_HEAD(__VA_ARGS__, "")
#define _LOG_ARGS_HEAD(H, ...) H

#define LOG_ARGS_TAIL(...) _LOG_ARGS_TAIL(__VA_ARGS__, "")
#define _LOG_ARGS_TAIL(H, ...) __VA_ARGS__

/* PRIVATE LOG MACROS */
#define LOG_LEVEL(LVL, ...) _LOG_LEVEL(LVL, LOG_ARGS_HEAD(__VA_ARGS__), LOG_ARGS_TAIL(__VA_ARGS__))
#define _LOG_LEVEL(LVL, msg, ...) __LOG("[LOG %d]\t%s:%s.%d\t" msg "%s", (int)LVL, PATH_TO_FILE, __func__,  __LINE__, __VA_ARGS__)
#define __LOG(msg, ...) __log__(msg, __VA_ARGS__)

/* PRIVATE TRACE MACROS */
#define _TRACE() __trace_call__("[TRACE]\t%s:%s\n", PATH_TO_FILE, __func__)

/* PRIVATE ERROR MACROS */
#define ERROR_LOG(...) _ERROR_LOG(LOG_ARGS_HEAD(__VA_ARGS__), LOG_ARGS_TAIL(__VA_ARGS__))
#define _ERROR_LOG(msg, ...) __ERROR_LOG("[ERROR]\t%s:%s.%d\t" msg "%s%s", PATH_TO_FILE, __func__, __LINE__, __VA_ARGS__)
#define __ERROR_LOG(msg, ...) __error__(msg, __VA_ARGS__)

#define _ERROR(msg, ...) __ERROR(msg, __VA_ARGS__, "")
#define __ERROR(msg, errno, ...) \
    do { \
        ERROR_LOG(msg, __VA_ARGS__); \
        return errno; \
    } while (0)

/* PRIVATE FATAL MACROS */
#define FATAL_LOG(...) _FATAL_LOG(LOG_ARGS_HEAD(__VA_ARGS__), LOG_ARGS_TAIL(__VA_ARGS__))
#define _FATAL_LOG(msg, ...) __FATAL_LOG("[FATAL]\t%s:%s.%d\t" msg "%s%s", PATH_TO_FILE, __func__, __LINE__, __VA_ARGS__)
#define __FATAL_LOG(msg, ...) __fatal__(msg, __VA_ARGS__)

#define _FATAL(...) __FATAL(__VA_ARGS__, "")
#define __FATAL(msg, ...) \
    do { \
        FATAL_LOG(msg, __VA_ARGS__); \
        stack_trace(); \
        exit(FATAL_EXIT_CODE); \
    } while (0)

static inline void __attribute__(( format(printf,1, 2) )) __trash__(const char *msg, ...)
{
    (void)msg;
}

void stack_trace(void);

/* **************************************************** */
/* USE MACROS BELOW TO LOG YOUR CODE */
#define TRACE() _TRACE()
#define LOG(...) LOG_LEVEL(LOG_LEVEL_ALWAYS, __VA_ARGS__)
#define ERROR(msg, ...) _ERROR(msg, __VA_ARGS__)
#define FATAL(...) _FATAL(__VA_ARGS__)

#if DEBUG_LVL >= LOG_LEVEL_CRIT
    #define LOG_CRIT(...) LOG_LEVEL(LOG_LEVEL_CRIT, __VA_ARGS__)
#else
    #define LOG_CRIT(...) __trash__(__VA_ARGS__)
#endif

#if DEBUG_LVL >= LOG_LEVEL_WARNING
    #define LOG_WARN(...) LOG_LEVEL(LOG_LEVEL_WARNING, __VA_ARGS__)
#else
    #define LOG_WARN(...) __trash__(__VA_ARGS__)
#endif

#if DEBUG_LVL >= LOG_LEVEL_INFO
    #define LOG_INFO(...) LOG_LEVEL(LOG_LEVEL_INFO, __VA_ARGS__)
#else
    #define LOG_INFO(...) __trash__(__VA_ARGS__)
#endif

#if DEBUG_LVL >= LOG_LEVEL_NOTICE
    #define LOG_NTCE(...) LOG_LEVEL(LOG_LEVEL_NOTICE, __VA_ARGS__)
#else
    #define LOG_NTCE(...) __trash__(__VA_ARGS__)
#endif

#if DEBUG_LVL >= LOG_LEVEL_DEBUG
    #define LOG_DEBG(...) LOG_LEVEL(LOG_LEVEL_DEBUG, __VA_ARGS__)
#else
    #define LOG_DEBG(...) __trash__(__VA_ARGS__)
#endif

/* **************************************************** */

typedef enum log_file_mode
{
    LOG_FILE_MODE_ENABLED,
    LOG_FILE_MODE_DISABLED
} log_file_mode_t;

void __attribute__(( format(printf,1, 2) )) __log__(const char *msg, ...);
void __attribute__(( format(printf,1, 2) ))  __trace_call__(const char *msg, ...);
void __attribute__(( format(printf,1, 2) ))  __error__(const char *msg, ...);
void __attribute__(( format(printf,1, 2) ))  __fatal__(const char *msg, ...);\
int log_init(const FILE *fd, log_file_mode_t file_mode);
void log_deinit(void);
FILE *log_get_fd(void);
FILE *log_get_file(void);

#endif
