#include "mailbox.h"
#include "hw_config.h"
#include "timer.h"
#include "uart.h"

#define MAILBOX_STATUS_FULL  (0x80000000)
#define MAILBOX_STATUS_EMPTY (0x40000000)

static mailbox_t* g_videocore_mailbox = (mailbox_t*)VIDEOCORE_MAILBOX;

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
    while (g_videocore_mailbox->status & flag)
    {
        timer_wait_single_cycle();
    }    
}

static void inline mailbox_write_data(unsigned int data)
{
    mailbox_wait_while_full();
    g_videocore_mailbox->write = data;
}

static unsigned int inline mailbox_read_data()
{
    mailbox_wait_while_empty();
    return g_videocore_mailbox->read;
}