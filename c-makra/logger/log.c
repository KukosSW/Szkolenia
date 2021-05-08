#include <log.h>
#include <stdio.h>
#include <stdarg.h>
#include <sys/stat.h> 	/* stat */
#include <time.h> 		/* strftime */
#include <string.h> 	/* strlen, SSE  */
#include <fcntl.h>

#define ___unused___ __attribute__(( unused ))

static ___unused___ FILE *logfd = NULL;
static ___unused___ FILE *logfile = NULL;
static ___unused___ const char * const logdir = "./logs";

FILE *log_get_fd(void)
{
	return logfd;
}

FILE *log_get_file(void)
{
	return logfile;
}

#define LOGBUF_SIZE	128

#ifdef DEBUG_MODE
void __log__(const char *msg, ...)
{
	va_list args;

	if (logfd != NULL)
	{
    	va_start(args, msg);
		vfprintf(logfd, msg, args);
		va_end(args);
	}

	if (logfile != NULL)
	{
		va_start (args, msg);
		vfprintf(logfile, msg, args);
		va_end(args);
	}
}
#else
void __log__(const char *msg, ...) { (void)msg; }
#endif

#ifdef TRACE_MODE
void __trace_call__(const char *msg, ...)
{
    va_list args;

	if (logfd != NULL)
	{
    	va_start(args, msg);
		vfprintf(logfd, msg, args);
		va_end(args);
	}

	if (logfile != NULL)
	{
		va_start (args, msg);
		vfprintf(logfile, msg, args);
		va_end(args);
	}
}
#else
void __trace_call__(const char *msg, ...) { (void)msg; }
#endif

#ifndef SILENT_ERROR
void __error__(const char *msg, ...)
{
	va_list args;

	if (logfd == NULL)
	{
    	va_start(args, msg);
		vfprintf(stderr, msg, args);
		va_end(args);
	}
	else
	{
		va_start(args, msg);
		vfprintf(logfd, msg, args);
		va_end(args);
	}

	if (logfile != NULL)
	{
		va_start (args, msg);
		vfprintf(logfile, msg, args);
		va_end(args);
	}
}
#else
void __error__(const char *msg, ...) { (void)msg; }
#endif

void __fatal__(const char *msg, ...)
{
	va_list args;

	if (logfd == NULL)
	{
    	va_start(args, msg);
		vfprintf(stderr, msg, args);
		va_end(args);
	}
	else
	{
		va_start(args, msg);
		vfprintf(logfd, msg, args);
		va_end(args);
	}

	if (logfile != NULL)
	{
		va_start (args, msg);
		vfprintf(logfile, msg, args);
		va_end(args);
	}
}

#ifdef DEBUG_MODE
int log_init(const FILE *fd, log_file_mode_t file_mode)
{
	struct stat st;
	struct tm t;
	time_t now;

	char buf[LOGBUF_SIZE];
	size_t len;

	logfd = (FILE *)fd; /* NULL is ok, log only to file */
	if (fd != NULL && (fd != stderr || fd != stdout))
		logfd = stderr;

	if (file_mode == LOG_FILE_MODE_ENABLED)
	{
		/* create directory if doesn't exist */
		if (stat(logdir, &st) == -1)
			if (mkdir(logdir, 0775) == -1)
			{
				perror("mkdir error\n");
				return 1;
			}

		len = strlen(logdir);
		if (memcpy((void *)buf, (void *)logdir, len) == NULL)
		{
			perror("memcpy error\n");
			return 1;
		}

		/* get local time and create string - file name */
		now = time(NULL);
		t = *localtime(&now);
		if (strftime(buf + len, LOGBUF_SIZE, "/%Y%m%d%H%M%S.log", &t) == 0)
		{
			perror("strftime error\n");
			return 1;
		}

		/* check existing of file */
		if (stat(buf, &st) == -1)
		{
			if (creat(buf, 0644) == -1)
			{
				perror("creat error\n");
				return 1;
			}
		}
		else
		{
		 	perror("Logs file exists\n");
			return 1;
		}

		/* open */
		logfile = fopen(buf, "w");
		if (logfile == NULL)
		{
			perror("fopen error\n");
			return 1;
		}
	}
	else
		logfile = NULL;

	return 0;
}
#else
int log_init(const FILE *fd, log_file_mode_t file_mode) { (void)fd; (void)file_mode; return 0; }
#endif

#ifdef DEBUG_MODE
void log_deinit(void)
{
	if (logfile != NULL)
	{
		(void)fclose(logfile);
		logfile = NULL;
	}

	logfd = NULL;
}
#else
void log_deinit(void) { }
#endif

#include <execinfo.h>

#define MAX_TRACE   100

#define NLEN    30
#define ALEN    20
#define OLEN    10

static inline void print_func_trace(const char *str);
static inline void print_func_trace(const char *str)
{
    const char * const spaces = "                                                                         ";
    char *name;
    char *off;
    char *addr;

    char *end_off;
    char *end_addr;

    const char * const empty = "";
    const char * const empty_name = "()";

    FILE *fd;
    FILE *file;

    fd = log_get_fd();
    file = log_get_file();

    name = strchr(str, '(');
    if (name == NULL || *(name + 1) == ')')
        name = (char *)empty_name;
    else
        ++name;

    off = strchr(str, '+');
    if (off == NULL)
        off = (char *)empty;
    else
        ++off;

    addr = strchr(str, '[');
    if (addr == NULL)
        addr = (char *)empty;
    else
        ++addr;

    end_addr = strchr(str, ']');
    if (end_addr != NULL)
        *end_addr = '\0';

    end_off = strchr(str, ')');
    if (end_off != NULL)
        *end_off = '\0';

    if (off != empty)
        *(off - 1) = '\0';

    fprintf(stderr, "NAME = %s%.*s", name, strlen(name) > NLEN ? 0 : (int)(NLEN - strlen(name)), spaces);
    fprintf(stderr, "ADDR = %s%.*s", addr, strlen(addr) > ALEN ? 0 : (int)(ALEN - strlen(addr)), spaces);
    fprintf(stderr, "OFF = %s%.*s\n", off, strlen(off) > OLEN ? 0 : (int)(OLEN - strlen(off)), spaces);

    if (fd != NULL && fd != stderr)
    {
        fprintf(fd, "NAME = %s%.*s", name, strlen(name) > NLEN ? 0 : (int)(NLEN - strlen(name)), spaces);
        fprintf(fd, "ADDR = %s%.*s", addr, strlen(addr) > ALEN ? 0 : (int)(ALEN - strlen(addr)), spaces);
        fprintf(fd, "OFF = %s%.*s\n", off, strlen(off) > OLEN ? 0 : (int)(OLEN - strlen(off)), spaces);
    }

    if (file != NULL)
    {
        fprintf(file, "NAME = %s%.*s", name, strlen(name) > NLEN ? 0 : (int)(NLEN - strlen(name)), spaces);
        fprintf(file, "ADDR = %s%.*s", addr, strlen(addr) > ALEN ? 0 : (int)(ALEN - strlen(addr)), spaces);
        fprintf(file, "OFF = %s%.*s\n", off, strlen(off) > OLEN ? 0 : (int)(OLEN - strlen(off)), spaces);
    }
}

void stack_trace(void)
{
    void *t[MAX_TRACE];
    int size;
    char **strings;
    int i;

    size = backtrace(t, MAX_TRACE);
    strings = backtrace_symbols(t, size);
    if (strings == NULL)
        return;

    for (i = 1; i < size; ++i)
        print_func_trace(strings[i]);

    free(strings);
}
