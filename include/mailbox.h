#ifndef MAILBOX_H
#define MAILBOX_H

#define MAILBOX_TYPE_REQUEST          (0x00000000)
#define MAILBOX_TYPE_RESPONSE         (0x80000000)
#define MAILBOX_TYPE_PARTIAL_RESPONSE (0x80000001)

typedef enum mailbox_tags
{
    MAILBOX_TAG_GET_SERIAL = 0x00010004,
    MAILBOX_TAG_SETPOWER = 0x00028001,
    MAILBOX_TAG_SETCLKRATE = 0x00038002,
    MAILBOX_TAG_FB_GET_FRAME_BUFFER = 0x00040001,
    MAILBOX_TAG_FB_GET_PITCH = 0x00040008,
    MAILBOX_TAG_FB_SET_PHY_WIDTH_HEIGHT = 0x00048003,
    MAILBOX_TAG_FB_SET_VIR_WIDTH_HEIGHT = 0x00048004,
    MAILBOX_TAG_FB_SET_COLOR_DEPTH = 0x00048005,
    MAILBOX_TAG_FB_SET_PIXEL_ORDER = 0x00048006,
    MAILBOX_TAG_FB_SET_VIR_OFFSET = 0x00048009,

    MAILBOX_TAG_LAST = 0
} mailbox_tags_e;

typedef struct mailbox
{
    unsigned int read;
    unsigned int reserved[0x4];
    unsigned int poll;
    unsigned int sender;
    unsigned int status;
    unsigned int config;
    unsigned int write;
} __attribute__((packed)) mailbox_t;

typedef enum mailbox_channel
{
    MAILBOX_CHANNEL_POWER       = 0,
    MAILBOX_CHANNEL_FRAMEBUFFER = 1,
    MAILBOX_CHANNEL_VUART       = 2,
    MAILBOX_CHANNEL_VCHIQ       = 3,
    MAILBOX_CHANNEL_LEDS        = 4,
    MAILBOX_CHANNEL_BTNS        = 5,
    MAILBOX_CHANNEL_TOUCH       = 6,
    MAILBOX_CHANNEL_COUNT       = 7,
    MAILBOX_CHANNEL_PROPERY     = 8
} mailbox_channel_e;

bool mailbox_call(mailbox_channel_e channel, unsigned int* mailbox_messages);

#endif // MAILBOX_H