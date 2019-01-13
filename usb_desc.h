#ifndef USB_DESC_H
#define USB_DESC_H

#include <avr/pgmspace.h>
#include <LUFA/Drivers/USB/USB.h>

typedef struct _usb_desc_t
{
    USB_Descriptor_Configuration_Header_t header;
    USB_Descriptor_Interface_t            kbd_boot_intf;
    USB_HID_Descriptor_HID_t              kbd_boot_hid;
    USB_Descriptor_Endpoint_t             kbd_boot_ep;
    USB_Descriptor_Interface_t            kbd_nkro_intf;
    USB_HID_Descriptor_HID_t              kbd_nkro_hid;
    USB_Descriptor_Endpoint_t             kbd_nkro_ep;
} usb_desc_t;

typedef enum _intf_desc_t
{
    INTF_BootKeyboard   = 0,
    INTF_NKROKeyboard   = 1,
} intf_desc_t;

typedef enum _string_desc_t
{
    STRING_Lang     = 0,
    STRING_Vendor   = 1,
    STRING_Product  = 2,
} string_desc_t;

#define BOOT_MAX_KEYS           6
#define NKRO_MAX_KEYS           62

typedef struct _boot_protocol_t
{
    uint8_t mods;
    uint8_t rsvd;
    uint8_t keys[BOOT_MAX_KEYS];
} ATTR_PACKED boot_protocol_t;

typedef struct _nkro_protocol_t
{
    uint8_t mods;
    uint8_t rsvd;
    uint8_t keys[NKRO_MAX_KEYS];
} ATTR_PACKED nkro_protocol_t;

#define BOOT_KEYBOARD_EPADDR    (ENDPOINT_DIR_IN | 1)
#define BOOT_KEYBOARD_EPSIZE    (sizeof(boot_protocol_t))

#define NKRO_KEYBOARD_EPADDR    (ENDPOINT_DIR_IN | 2)
#define NKRO_KEYBOARD_EPSIZE    (sizeof(nkro_protocol_t))

#endif

