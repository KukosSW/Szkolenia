#include <stdio.h>
#include <log.h>

int f(int a);
int f(int a)
{
    TRACE();
    ERROR("Mamy error a = %d\n", 1, a);
}

int main(void)
{
    log_init(stdout, LOG_FILE_MODE_ENABLED);

    LOG("Pierwszy\n");
    LOG_CRIT("Drugi\n");
    LOG_WARN("Trzeci\n");
    LOG_INFO("Czwarty\n");
    LOG_NTCE("Piaty\n");
    LOG_DEBG("Szosty\n");

    TRACE();

    f(10);
    FATAL("Mamy fatal A bylo = %d\n", 10);

    log_deinit();
    return 0;
}