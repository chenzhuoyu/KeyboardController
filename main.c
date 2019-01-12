#include <string.h>
#include <stdbool.h>

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/power.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#include <LUFA/Drivers/USB/USB.h>
#include <LUFA/Platform/Platform.h>

#include "usb_desc.h"

#define SW_DP1              (1 << PB0)
#define SW_DP2              (1 << PC2)
#define SW_DP3              (1 << PC7)
#define SW_DP4              (1 << PB4)

#define SW_BTN              (1 << PB5)
#define LED_RED             (1 << PB6)
#define LED_BLUE            (1 << PB7)

#define SHR_DS              (1 << PC4)
#define SHR_CK              (1 << PC5)
#define SHR_ST              (1 << PC6)

#define DP1()               (PINB & SW_DP1)
#define DP2()               (PINC & SW_DP2)
#define DP3()               (PINC & SW_DP3)
#define DP4()               (PINB & SW_DP4)
#define BTN()               (PINB & SW_BTN)

#define SET_DS()            PORTC |=  SHR_DS
#define CLR_DS()            PORTC &= ~SHR_DS

#define SET_CK()            PORTC |=  SHR_CK
#define CLR_CK()            PORTC &= ~SHR_CK

#define SET_ST()            PORTC |=  SHR_ST
#define CLR_ST()            PORTC &= ~SHR_ST

#define RED_ON()            PORTB &= ~LED_RED
#define RED_OFF()           PORTB |=  LED_RED

#define BLUE_ON()           PORTB &= ~LED_BLUE
#define BLUE_OFF()          PORTB |=  LED_BLUE

#define KEY(k)              _KEY(k)
#define _KEY(k)             HID_KEYBOARD_SC_ ## k

#define KEY_1               KEY(1_AND_EXCLAMATION)
#define KEY_2               KEY(2_AND_AT)
#define KEY_3               KEY(3_AND_HASHMARK)
#define KEY_4               KEY(4_AND_DOLLAR)
#define KEY_5               KEY(5_AND_PERCENTAGE)
#define KEY_6               KEY(6_AND_CARET)
#define KEY_7               KEY(7_AND_AMPERSAND)
#define KEY_8               KEY(8_AND_ASTERISK)
#define KEY_9               KEY(9_AND_OPENING_PARENTHESIS)
#define KEY_0               KEY(0_AND_CLOSING_PARENTHESIS)

#define KEY_UP              KEY(UP_ARROW)
#define KEY_DOWN            KEY(DOWN_ARROW)
#define KEY_LEFT            KEY(LEFT_ARROW)
#define KEY_RIGHT           KEY(RIGHT_ARROW)

#define KEY_FN              0xff
#define KEY_BAR             KEY(BACKSLASH_AND_PIPE)
#define KEY_BKSP            KEY(BACKSPACE)
#define KEY_PSSR            KEY(PRINT_SCREEN)
#define KEY_PABR            KEY(PAUSE)

#define KEY_ESC             KEY(ESCAPE)
#define KEY_DEL             KEY(DELETE)
#define KEY_INS             KEY(INSERT)
#define KEY_PGUP            KEY(PAGE_UP)
#define KEY_PGDN            KEY(PAGE_DOWN)

#define KEY_DOT             KEY(DOT_AND_GREATER_THAN_SIGN)
#define KEY_COMMA           KEY(COMMA_AND_LESS_THAN_SIGN)
#define KEY_SLASH           KEY(SLASH_AND_QUESTION_MARK)
#define KEY_SMCL            KEY(SEMICOLON_AND_COLON)
#define KEY_APOS            KEY(APOSTROPHE_AND_QUOTE)
#define KEY_NUHM            KEY(NON_US_HASHMARK_AND_TILDE)

#define KEY_EQUAL           KEY(EQUAL_AND_PLUS)
#define KEY_MINUS           KEY(MINUS_AND_UNDERSCORE)
#define KEY_LBRK            KEY(OPENING_BRACKET_AND_OPENING_BRACE)
#define KEY_RBRK            KEY(CLOSING_BRACKET_AND_CLOSING_BRACE)

#define KEY_NUM             KEY(NUM_LOCK)
#define KEY_CAPS            KEY(CAPS_LOCK)
#define KEY_SCLK            KEY(SCROLL_LOCK)

#define KEY_NP1             KEY(KEYPAD_1_AND_END)
#define KEY_NP2             KEY(KEYPAD_2_AND_DOWN_ARROW)
#define KEY_NP3             KEY(KEYPAD_3_AND_PAGE_DOWN)
#define KEY_NP4             KEY(KEYPAD_4_AND_LEFT_ARROW)
#define KEY_NP5             KEY(KEYPAD_5)
#define KEY_NP6             KEY(KEYPAD_6_AND_RIGHT_ARROW)
#define KEY_NP7             KEY(KEYPAD_7_AND_HOME)
#define KEY_NP8             KEY(KEYPAD_8_AND_UP_ARROW)
#define KEY_NP9             KEY(KEYPAD_9_AND_PAGE_UP)
#define KEY_NP0             KEY(KEYPAD_0_AND_INSERT)
#define KEY_NP_DOT          KEY(KEYPAD_DOT_AND_DELETE)
#define KEY_NP_ADD          KEY(KEYPAD_PLUS)
#define KEY_NP_SUB          KEY(KEYPAD_MINUS)
#define KEY_NP_MUL          KEY(KEYPAD_ASTERISK)
#define KEY_NP_DIV          KEY(KEYPAD_SLASH)
#define KEY_NP_RET          KEY(KEYPAD_ENTER)

#define KEY_LCMD            KEY(LEFT_GUI)
#define KEY_RCMD            KEY(RIGHT_GUI)
#define KEY_LOPT            KEY(LEFT_ALT)
#define KEY_ROPT            KEY(RIGHT_ALT)
#define KEY_LCTRL           KEY(LEFT_CONTROL)
#define KEY_RCTRL           KEY(RIGHT_CONTROL)
#define KEY_LSHIFT          KEY(LEFT_SHIFT)
#define KEY_RSHIFT          KEY(RIGHT_SHIFT)

#define KEY_DEAD_TIME       10
#define HID_INTF_INDEX      INTERFACE_ID_Keyboard
#define HID_BUFFER_SIZE     sizeof(USB_KeyboardReport_Data_t)

/* Keyboard matrix. */
static const uint8_t PROGMEM MATRIX[15][8] = {
    { KEY(N)    , KEY(M)    , KEY_7     , KEY_6     , KEY(J)    , KEY(H)    , KEY(Y)    , KEY(U)     },
    { KEY_DOWN  , KEY_NUM   , 0         , KEY_DEL   , KEY_NP1   , KEY(SPACE), KEY_NP4   , KEY_NP7    },
    { KEY_RIGHT , KEY_NP_DIV, 0         , KEY_INS   , KEY_NP2   , KEY_NP0   , KEY_NP5   , KEY_NP8    },
    { KEY(F12)  , KEY(ENTER), KEY(F10)  , KEY(F9)   , KEY_BAR   , KEY(F11)  , KEY_BKSP  , 0          },
    { 0         , KEY_COMMA , KEY_8     , KEY_EQUAL , KEY(K)    , KEY(F6)   , KEY_RBRK  , KEY(I)     },
    { KEY_FN    , KEY_DOT   , KEY_9     , KEY(F8)   , KEY(L)    , 0         , KEY(F7)   , KEY(O)     },
    { 0         , KEY(C)    , KEY_3     , KEY(F2)   , KEY(D)    , KEY(F4)   , KEY(F3)   , KEY(E)     },
    { KEY_SLASH , 0         , KEY_0     , KEY_MINUS , KEY_SMCL  , KEY_APOS  , KEY_LBRK  , KEY(P)     },
    { KEY_RCMD  , 0         , KEY_PSSR  , 0         , KEY_ROPT  , KEY_LCMD  , 0         , KEY_SCLK   },
    { 0         , KEY(X)    , KEY_2     , KEY(F1)   , KEY(S)    , 0         , KEY_CAPS  , KEY(W)     },
    { KEY(B)    , KEY(V)    , KEY_4     , KEY_5     , KEY(F)    , KEY(G)    , KEY(T)    , KEY(R)     },
    { KEY_NP_SUB, KEY_NP_MUL, KEY_PGDN  , KEY_PGUP  , KEY_NP3   , KEY_NP_DOT, KEY_NP6   , KEY_NP9    },
    { KEY_LEFT  , KEY_RSHIFT, KEY(END)  , KEY(HOME) , KEY_NP_RET, KEY_UP    , KEY_LSHIFT, KEY_NP_ADD },
    { 0         , KEY_RCTRL , KEY(F5)   , KEY_LCTRL , 0         , 0         , KEY_LOPT  , KEY_PABR   },
    { 0         , KEY(Z)    , KEY_1     , KEY_NUHM  , KEY(A)    , KEY_ESC   , KEY(TAB)  , KEY(Q)     },
};

/** Debouncer and report buffer. **/
static uint8_t _key_states[15] = {};
static uint8_t _key_timers[15][8] = {};
static uint8_t _hid_buffer[HID_BUFFER_SIZE] = {};

/** USB HID device object. **/
static USB_ClassInfo_HID_Device_t _hid_device =
{
    .Config = {
        .InterfaceNumber    = HID_INTF_INDEX,
        .ReportINEndpoint   = {
            .Address    = KEYBOARD_EPADDR,
            .Size       = KEYBOARD_EPSIZE,
            .Banks      = 1,
        },
        .PrevReportINBuffer     = _hid_buffer,
        .PrevReportINBufferSize = HID_BUFFER_SIZE,
    }
};

static void hw_init(void)
{
    /* DIP switches */
    DDRB &= ~SW_DP1;
    DDRC &= ~SW_DP2;
    DDRC &= ~SW_DP3;
    DDRB &= ~SW_DP4;

    /* LEDs and button switch */
    DDRB &= ~SW_BTN;
    DDRB |=  LED_RED;
    DDRB |=  LED_BLUE;

    /* turn off lights when start */
    PORTB |= LED_RED;
    PORTB |= LED_BLUE;

    /* shift register control */
    DDRC |= SHR_DS;
    DDRC |= SHR_CK;
    DDRC |= SHR_ST;

    /* setup DS and ST */
    SET_DS();
    CLR_ST();

    /* set all 16 bits */
    for (int i = 0; i < 16; i++)
    {
        SET_CK();
        CLR_CK();
    }

    /* load into output */
    SET_ST();
    CLR_ST();

    /* all inputs for PORTD */
    DDRD = 0;
    PORTD = 0xff;
}


static void kbd_scan(void)
{
    /* first low bit */
    CLR_DS();
    SET_CK();
    CLR_CK();
    SET_ST();
    CLR_ST();

    /* remainings are all high bits */
    SET_DS();
    SET_CK();
    CLR_CK();
    SET_ST();
    CLR_ST();

    /* read each row */
    for (uint8_t i = 0; i < 15; i++)
    {
        /* read row buffer */
        uint8_t row = PIND;

        /* scan each colum */
        for (uint8_t j = 0; j < 8; j++)
        {
            /* key debouncing */
            if (_key_timers[i][j])
            {
                row >>= 1;
                _key_timers[i][j]--;
                continue;
            }

            /* only trigger on state changes */
            if (!(row & 1) != !(_key_states[i] & (1 << j)))
            {
                row >>= 1;
                continue;
            }

            /* update key status */
            if (!(row & 1))
                _key_states[i] |= (1 << j);
            else
                _key_states[i] &= ~(1 << j);

            /* key debouncer */
            row >>= 1;
            _key_timers[i][j] = KEY_DEAD_TIME;
        }

        /* move to next row */
        SET_CK();
        CLR_CK();
        SET_ST();
        CLR_ST();
    }
}

static void kbd_update(USB_KeyboardReport_Data_t *report)
{
    uint8_t n = 0;
    uint8_t key;

    /* read each row */
    for (uint8_t i = 0; i < 15; i++)
    {
        /* read each colum */
        for (uint8_t j = 0; j < 8; j++)
        {
            /* check for key state */
            if (_key_states[i] & (1 << j))
            {
                /* check for modifiers */
                switch ((key = pgm_read_byte(&(MATRIX[i][j]))))
                {
                    /* modifiers -- left hand side */
                    case KEY_LCMD   : report->Modifier |= HID_KEYBOARD_MODIFIER_LEFTGUI; break;
                    case KEY_LOPT   : report->Modifier |= HID_KEYBOARD_MODIFIER_LEFTALT; break;
                    case KEY_LCTRL  : report->Modifier |= HID_KEYBOARD_MODIFIER_LEFTCTRL; break;
                    case KEY_LSHIFT : report->Modifier |= HID_KEYBOARD_MODIFIER_LEFTSHIFT; break;

                    /* modifiers -- right hand side */
                    case KEY_RCMD   : report->Modifier |= HID_KEYBOARD_MODIFIER_RIGHTGUI; break;
                    case KEY_ROPT   : report->Modifier |= HID_KEYBOARD_MODIFIER_RIGHTALT; break;
                    case KEY_RCTRL  : report->Modifier |= HID_KEYBOARD_MODIFIER_RIGHTCTRL; break;
                    case KEY_RSHIFT : report->Modifier |= HID_KEYBOARD_MODIFIER_RIGHTSHIFT; break;

                    /* normal keys */
                    default:
                    {
                        /* 6KRO mode */
                        if (n < 6)
                            report->KeyCode[n] = key;;

                        /* update key counter */
                        n++;
                        break;
                    }
                }
            }
        }
    }
}

static void kbd_event_out(const uint8_t *data)
{
    /* Num-Lock */
	if (*data & HID_KEYBOARD_LED_NUMLOCK)
        BLUE_ON();
    else
        BLUE_OFF();

    /* Caps-Lock */
    if (*data & HID_KEYBOARD_LED_CAPSLOCK)
        RED_ON();
    else
        RED_OFF();
}

int main(void)
{
    /* initialize GPIO and USB */
    hw_init();
	USB_Init();
	GlobalInterruptEnable();

    /* main event loop */
	for (;;)
	{
        kbd_scan();
		HID_Device_USBTask(&_hid_device);
		USB_USBTask();
	}
}

/***** Event Handlers *****/

/** Event handler for the USB device Start Of Frame event. */
void EVENT_USB_Device_StartOfFrame(void)
{
    /* nothing interesting, forward to LUFA */
	HID_Device_MillisecondElapsed(&_hid_device);
}

/** Event handler for the library USB Control Request reception event. */
void EVENT_USB_Device_ControlRequest(void)
{
    /* nothing interesting, forward to LUFA */
	HID_Device_ProcessControlRequest(&_hid_device);
}

/** Event handler for the library USB Configuration Changed event. */
void EVENT_USB_Device_ConfigurationChanged(void)
{
	HID_Device_ConfigureEndpoints(&_hid_device);
	USB_Device_EnableSOFEvents();
}

/** HID class driver callback function for the creation of HID reports to the host.
 *
 *  \param[in]     self  Pointer to the HID class interface configuration structure being referenced
 *  \param[in,out] id    Report ID requested by the host if non-zero, otherwise callback should set to the generated report ID
 *  \param[in]     type  Type of the report to create, either HID_REPORT_ITEM_In or HID_REPORT_ITEM_Feature
 *  \param[out]    data  Pointer to a buffer where the created report should be stored
 *  \param[out]    size  Number of bytes written in the report (or zero if no report is to be sent)
 *
 *  \return Boolean \c true to force the sending of the report, \c false to let the library determine if it needs to be sent
 */
bool CALLBACK_HID_Device_CreateHIDReport(USB_ClassInfo_HID_Device_t *self, uint8_t *id, uint8_t type, void *data, uint16_t *size)
{
    /* update keyboard state */
    memset(data, 0, sizeof(USB_KeyboardReport_Data_t));
    kbd_update((USB_KeyboardReport_Data_t *)data);

    /* fixed report size */
	*size = sizeof(USB_KeyboardReport_Data_t);
	return false;
}

/** HID class driver callback function for the processing of HID reports from the host.
 *
 *  \param[in] self  Pointer to the HID class interface configuration structure being referenced
 *  \param[in] id    Report ID of the received report from the host
 *  \param[in] type  The type of report that the host has sent, either HID_REPORT_ITEM_Out or HID_REPORT_ITEM_Feature
 *  \param[in] data  Pointer to a buffer where the received report has been stored
 *  \param[in] size  Size in bytes of the received HID report
 */
void CALLBACK_HID_Device_ProcessHIDReport(USB_ClassInfo_HID_Device_t *self, uint8_t id, uint8_t type, const void *data, uint16_t size)
{
    /* only care about this event */
    if (type == HID_REPORT_ITEM_Out)
        kbd_event_out((const uint8_t *)data);
}
