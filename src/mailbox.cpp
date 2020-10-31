#include "mailbox.h"
#include "hw_config.h"
#include "timer.h"
#include "utils.h"

#define MAILBOX_STATUS_FULL  (0x80000000)
#define MAILBOX_STATUS_EMPTY (0x40000000)

static void inline mailbox_wait_while_flag(unsigned int flag);
static void inline mailbox_wait_while_full();
static void inline mailbox_wait_while_empty();
static void inline mailbox_write_data(unsigned int data);
static unsigned int inline mailbox_read_data();

bool mailbox_call(mailbox_channel_e channel, unsigned int* mailbox_messages)
{
    bool response_successful = false;

    unsigned int mailbox_data = ((unsigned int)((unsigned int)mailbox_messages) & ~0xF);
    mailbox_data |= (channel & 0xF);

    mailbox_write_data(mailbox_data);

    bool waiting_for_response = true;
    while (waiting_for_response) 
    {
        bool data_is_ours = mailbox_read_data() == mailbox_data;

        if (data_is_ours)
        {
            waiting_for_response = false;
            response_successful = mailbox_messages[1] == MAILBOX_TYPE_RESPONSE;
        }
    }
    
    return response_successful;
}

static void inline mailbox_wait_while_full()
{
    mailbox_wait_while_flag(MAILBOX_STATUS_FULL);
}

static void inline mailbox_wait_while_empty()
{
    mailbox_wait_while_flag(MAILBOX_STATUS_EMPTY);
}

static void inline mailbox_wait_while_flag(unsigned int flag)
{
    while (read32(VIDEOCORE_MAILBOX_STATUS) & flag)
    {
        timer_wait_single_cycle();
    }    
}

static void inline mailbox_write_data(unsigned int data)
{
    mailbox_wait_while_full();
    write32(VIDEOCORE_MAILBOX_WRITE, data);
}

static unsigned int inline mailbox_read_data()
{
    mailbox_wait_while_empty();
    return read32(VIDEOCORE_MAILBOX_READ);
}