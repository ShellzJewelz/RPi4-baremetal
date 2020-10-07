#include "frame_buffer.h"
#include "mem.h"
#include "uart.h"
#include "mailbox.h"

/* PC Screen Font as used by Linux Console */
typedef struct 
{
    unsigned int magic;
    unsigned int version;
    unsigned int headersize;
    unsigned int flags;
    unsigned int numglyph;
    unsigned int bytesperglyph;
    unsigned int height;
    unsigned int width;
    unsigned char glyphs;
} __attribute__((packed)) psf_t;
extern volatile unsigned char _binary_font_psf_start;

/* Scalable Screen Font (https://gitlab.com/bztsrc/scalable-font2) */
typedef struct 
{
    unsigned char  magic[4];
    unsigned int   size;
    unsigned char  type;
    unsigned char  features;
    unsigned char  width;
    unsigned char  height;
    unsigned char  baseline;
    unsigned char  underline;
    unsigned short fragments_offs;
    unsigned int   characters_offs;
    unsigned int   ligature_offs;
    unsigned int   kerning_offs;
    unsigned int   cmap_offs;
} __attribute__((packed)) sfn_t;
extern volatile unsigned char _binary_font_sfn_start;

static unsigned int g_width, g_height, g_pitch;
unsigned char *g_frame_buffer_base;
psf_t *g_font;

static void frame_buffer_display_character(unsigned char* glyph, unsigned int memory_offset);

/**
 * Set screen resolution to 1024x768
 */ 
void frame_buffer_init()
{
    unsigned int __attribute__((aligned(16))) mailbox_messages[36];
    // unsigned int* mailbox_messages = (unsigned int*)mem_alloc();

    mailbox_messages[0] = 35*4;
    mailbox_messages[1] = MAILBOX_TYPE_REQUEST;

    mailbox_messages[2] = MAILBOX_TAG_FB_SET_PHY_WIDTH_HEIGHT;  //set phy wh
    mailbox_messages[3] = 8;
    mailbox_messages[4] = 8;
    mailbox_messages[5] = 1024;         //FrameBufferInfo.width
    mailbox_messages[6] = 768;          //FrameBufferInfo.height

    mailbox_messages[7] = MAILBOX_TAG_FB_SET_VIR_WIDTH_HEIGHT;  //set virt wh
    mailbox_messages[8] = 8;
    mailbox_messages[9] = 8;
    mailbox_messages[10] = 1024;        //FrameBufferInfo.virtual_width
    mailbox_messages[11] = 768;         //FrameBufferInfo.virtual_height

    mailbox_messages[12] = MAILBOX_TAG_FB_SET_VIR_OFFSET; //set virt offset
    mailbox_messages[13] = 8;
    mailbox_messages[14] = 8;
    mailbox_messages[15] = 0;           //FrameBufferInfo.x_offset
    mailbox_messages[16] = 0;           //FrameBufferInfo.y.offset

    mailbox_messages[17] = MAILBOX_TAG_FB_SET_COLOR_DEPTH; //set depth
    mailbox_messages[18] = 4;
    mailbox_messages[19] = 4;
    mailbox_messages[20] = 32;          //FrameBufferInfo.depth

    mailbox_messages[21] = MAILBOX_TAG_FB_SET_PIXEL_ORDER; //set pixel order
    mailbox_messages[22] = 4;
    mailbox_messages[23] = 4;
    mailbox_messages[24] = 1;           //RGB, not BGR preferably

    mailbox_messages[25] = MAILBOX_TAG_FB_GET_FRAME_BUFFER;
    mailbox_messages[26] = 8;
    mailbox_messages[27] = 8;
    mailbox_messages[28] = 4096;        //FrameBufferInfo.pointer
    mailbox_messages[29] = 0;           //FrameBufferInfo.size

    mailbox_messages[30] = MAILBOX_TAG_FB_GET_PITCH; //get pitch
    mailbox_messages[31] = 4;
    mailbox_messages[32] = 4;
    mailbox_messages[33] = 0;           //FrameBufferInfo.pitch

    mailbox_messages[34] = MAILBOX_TAG_LAST;

    if (mailbox_call(MAILBOX_CHANNEL_PROPERY, mailbox_messages) && 
        mailbox_messages[20] == 32 && mailbox_messages[28] != 0) 
    {
        mailbox_messages[28]&=0x3FFFFFFF;
        g_width = mailbox_messages[5];
        g_height = mailbox_messages[6];
        g_pitch = mailbox_messages[33];
        g_frame_buffer_base = (unsigned char*)((unsigned long int)mailbox_messages[28]);
        g_font = (psf_t*)&_binary_font_psf_start;
    }
    else 
    {
        uart_puts("Unable to set screen resolution to 1024x768x32\n");
    }

    //mem_free(mailbox_messages);
}

/**
 * Display a string using fixed size PSF
 */
void frame_buffer_print(unsigned int x, unsigned int y, const char *s)
{
    if (x > g_width - 1 || y > g_height - 1)
    {
        return;
    }

    while (*s) 
    {
        // get the offset of the glyph.
        unsigned char* glyph = (unsigned char*)g_font + g_font->headersize + 
            (*(unsigned char*)s < g_font->numglyph ? *s : 0) * g_font->bytesperglyph;
        
        // calculate the offset on screen
        unsigned int memory_offset = (y * g_pitch) + (x * 4);

        // handle carrige return
        if (*s == '\r') 
        {
            x = 0;
        } 
        else if (*s == '\n') 
        {
            x = 0; 
            y += g_font->height;
        } 
        else 
        {
            frame_buffer_display_character(glyph, memory_offset);

            x += (g_font->width + 1);
        }
        // next character
        s++;
    }
}

static void frame_buffer_display_character(unsigned char* glyph, unsigned int memory_offset)
{
    // variables
    unsigned int line,mask, bytesperline=(g_font->width + 7) / 8;

    for (unsigned int j = 0; j < g_font->height; j++)
    {
        // display one row
        line = memory_offset;
        mask = 1 << (g_font->width - 1);
        for (unsigned int i = 0; i < g_font->width; i++)
        {
            // if bit set, we use white color, otherwise black
            *((unsigned int*)(g_frame_buffer_base + line)) = ((int)*glyph) & mask ? 0xFFFFFF : 0;
            mask >>= 1;
            line += 4;
        }

        // adjust to next line
        glyph += bytesperline;
        memory_offset += g_pitch;
    }
}