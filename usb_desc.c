#include "usb_desc.h"

static const USB_Descriptor_Device_t PROGMEM _usb_device =
{
    .Header = {
        .Size = sizeof(USB_Descriptor_Device_t),
        .Type = DTYPE_Device
    },

    .USBSpecification       = VERSION_BCD(1, 1, 0),
    .Class                  = USB_CSCP_NoDeviceClass,
    .SubClass               = USB_CSCP_NoDeviceSubclass,
    .Protocol               = USB_CSCP_NoDeviceProtocol,
    .Endpoint0Size          = FIXED_CONTROL_ENDPOINT_SIZE,
    .VendorID               = 0x01a1,               /* =  417 */
    .ProductID              = 0x07c5,               /* = 1989 */
    .ReleaseNumber          = VERSION_BCD(0, 0, 1),
    .ManufacturerStrIndex   = STRING_Vendor,
    .ProductStrIndex        = STRING_Product,
    .SerialNumStrIndex      = NO_DESCRIPTOR,
    .NumberOfConfigurations = FIXED_NUM_CONFIGURATIONS
};

static const uint8_t PROGMEM _kbd_boot[] =
{
    HID_RI_USAGE_PAGE(8, 0x01),             /* Generic Desktop */
    HID_RI_USAGE(8, 0x06),                  /* Keyboard */
    HID_RI_COLLECTION(8, 0x01),             /* Application */

    HID_RI_LOGICAL_MINIMUM(8, 0x00),
    HID_RI_LOGICAL_MAXIMUM(16, 0x00ff),
    HID_RI_REPORT_SIZE(8, 0x08),
    HID_RI_REPORT_COUNT(8, BOOT_MAX_KEYS),
    HID_RI_INPUT(8, HID_IOF_CONSTANT),

    HID_RI_END_COLLECTION(0)
};

static const uint8_t PROGMEM _kbd_nkro[] =
{
    HID_RI_USAGE_PAGE(8, 0x01),             /* Generic Desktop */
    HID_RI_USAGE(8, 0x06),                  /* Keyboard */
    HID_RI_COLLECTION(8, 0x01),             /* Application */

    HID_RI_USAGE_PAGE(8, 0x07),             /* Modifier Keys */
    HID_RI_USAGE_MINIMUM(8, 0xE0),          /* Keyboard Left Control */
    HID_RI_USAGE_MAXIMUM(8, 0xE7),          /* Keyboard Right GUI */
    HID_RI_LOGICAL_MINIMUM(8, 0x00),
    HID_RI_LOGICAL_MAXIMUM(8, 0x01),
    HID_RI_REPORT_COUNT(8, 0x08),
    HID_RI_REPORT_SIZE(8, 0x01),
    HID_RI_INPUT(8, HID_IOF_DATA | HID_IOF_VARIABLE | HID_IOF_ABSOLUTE),

    HID_RI_REPORT_SIZE(8, 0x08),
    HID_RI_REPORT_COUNT(8, 0x01),
    HID_RI_INPUT(8, HID_IOF_CONSTANT),

    HID_RI_USAGE_PAGE(8, 0x08),             /* LEDs */
    HID_RI_USAGE_MINIMUM(8, 0x01),          /* Num Lock */
    HID_RI_USAGE_MAXIMUM(8, 0x05),          /* Kana */
    HID_RI_REPORT_COUNT(8, 0x05),
    HID_RI_REPORT_SIZE(8, 0x01),
    HID_RI_OUTPUT(8, HID_IOF_DATA | HID_IOF_VARIABLE | HID_IOF_ABSOLUTE | HID_IOF_NON_VOLATILE),

    HID_RI_REPORT_SIZE(8, 0x03),
    HID_RI_REPORT_COUNT(8, 0x01),
    HID_RI_OUTPUT(8, HID_IOF_CONSTANT),

    HID_RI_LOGICAL_MINIMUM(8, 0x00),
    HID_RI_LOGICAL_MAXIMUM(8, 0x65),
    HID_RI_USAGE_PAGE(8, 0x07),             /* Normal Keys */
    HID_RI_USAGE_MINIMUM(8, 0x00),          /* Reserved (no event indicated) */
    HID_RI_USAGE_MAXIMUM(8, 0x65),          /* Keyboard Application */
    HID_RI_REPORT_SIZE(8, 0x08),
    HID_RI_REPORT_COUNT(8, NKRO_MAX_KEYS),
    HID_RI_INPUT(8, HID_IOF_DATA | HID_IOF_ARRAY | HID_IOF_ABSOLUTE),

    HID_RI_END_COLLECTION(0)
};

static const usb_desc_t PROGMEM _usb_desc =
{
    .header = {
        .Header = {
            .Size = sizeof(USB_Descriptor_Configuration_Header_t),
            .Type = DTYPE_Configuration
        },

        .TotalConfigurationSize = sizeof(usb_desc_t),
        .TotalInterfaces        = 2,
        .ConfigurationNumber    = 1,
        .ConfigurationStrIndex  = NO_DESCRIPTOR,
        .ConfigAttributes       = USB_CONFIG_ATTR_RESERVED,
        .MaxPowerConsumption    = USB_CONFIG_POWER_MA(100)
    },

    .kbd_boot_intf = {
        .Header = {
            .Size = sizeof(USB_Descriptor_Interface_t),
            .Type = DTYPE_Interface
        },

        .InterfaceNumber        = INTF_BootKeyboard,
        .AlternateSetting       = 0x00,
        .TotalEndpoints         = 1,
        .Class                  = HID_CSCP_HIDClass,
        .SubClass               = HID_CSCP_BootSubclass,
        .Protocol               = HID_CSCP_KeyboardBootProtocol,
        .InterfaceStrIndex      = NO_DESCRIPTOR
    },

    .kbd_boot_hid = {
        .Header = {
            .Size = sizeof(USB_HID_Descriptor_HID_t),
            .Type = HID_DTYPE_HID
        },

        .HIDSpec                = VERSION_BCD(1, 1, 1),
        .CountryCode            = 0x00,
        .TotalReportDescriptors = 1,
        .HIDReportType          = HID_DTYPE_Report,
        .HIDReportLength        = sizeof(_kbd_boot)
    },

    .kbd_boot_ep = {
        .Header = {
            .Size = sizeof(USB_Descriptor_Endpoint_t),
            .Type = DTYPE_Endpoint
        },

        .EndpointAddress        = BOOT_KEYBOARD_EPADDR,
        .Attributes             = EP_TYPE_INTERRUPT | ENDPOINT_USAGE_DATA,
        .EndpointSize           = BOOT_KEYBOARD_EPSIZE,
        .PollingIntervalMS      = 0x01
    },

    .kbd_nkro_intf = {
        .Header = {
            .Size = sizeof(USB_Descriptor_Interface_t),
            .Type = DTYPE_Interface
        },

        .InterfaceNumber        = INTF_NKROKeyboard,
        .AlternateSetting       = 0x00,
        .TotalEndpoints         = 1,
        .Class                  = HID_CSCP_HIDClass,
        .SubClass               = HID_CSCP_NonBootSubclass,
        .Protocol               = HID_CSCP_NonBootProtocol,
        .InterfaceStrIndex      = NO_DESCRIPTOR
    },

    .kbd_nkro_hid = {
        .Header = {
            .Size = sizeof(USB_HID_Descriptor_HID_t),
            .Type = HID_DTYPE_HID
        },

        .HIDSpec                = VERSION_BCD(1, 1, 1),
        .CountryCode            = 0x00,
        .TotalReportDescriptors = 1,
        .HIDReportType          = HID_DTYPE_Report,
        .HIDReportLength        = sizeof(_kbd_nkro)
    },

    .kbd_nkro_ep = {
        .Header = {
            .Size = sizeof(USB_Descriptor_Endpoint_t),
            .Type = DTYPE_Endpoint
        },

        .EndpointAddress        = NKRO_KEYBOARD_EPADDR,
        .Attributes             = EP_TYPE_INTERRUPT | ENDPOINT_USAGE_DATA,
        .EndpointSize           = NKRO_KEYBOARD_EPSIZE,
        .PollingIntervalMS      = 0x01
    },
};

static const USB_Descriptor_String_t PROGMEM _str_lang    = USB_STRING_DESCRIPTOR_ARRAY(LANGUAGE_ID_ENG);
static const USB_Descriptor_String_t PROGMEM _str_vendor  = USB_STRING_DESCRIPTOR(L"Oxygen");
static const USB_Descriptor_String_t PROGMEM _str_product = USB_STRING_DESCRIPTOR(L"Oxygen's Keyboard");

uint16_t CALLBACK_USB_GetDescriptor(const uint16_t value, const uint16_t index, const void **desc)
{
    switch (value >> 8)
    {
        case DTYPE_Device:
        {
            *desc = &_usb_device;
            return sizeof(USB_Descriptor_Device_t);
        }

        case DTYPE_Configuration:
        {
            *desc = &_usb_desc;
            return sizeof(usb_desc_t);
        }

        case DTYPE_String:
        {
            switch (value & 0xff)
            {
                case STRING_Lang:
                {
                    *desc = &_str_lang;
                    return pgm_read_byte(&(_str_lang.Header.Size));
                }

                case STRING_Vendor:
                {
                    *desc = &_str_vendor;
                    return pgm_read_byte(&(_str_vendor.Header.Size));
                }

                case STRING_Product:
                {
                    *desc = &_str_product;
                    return pgm_read_byte(&(_str_product.Header.Size));
                }

                default:
                {
                    *desc = NULL;
                    return NO_DESCRIPTOR;
                }
            }
        }

        case HID_DTYPE_HID:
        {
            switch (index)
            {
                case INTF_BootKeyboard:
                {
                    *desc = &(_usb_desc.kbd_boot_hid);
                    return sizeof(USB_HID_Descriptor_HID_t);
                }

                case INTF_NKROKeyboard:
                {
                    *desc = &(_usb_desc.kbd_nkro_hid);
                    return sizeof(USB_HID_Descriptor_HID_t);
                }

                default:
                {
                    *desc = NULL;
                    return NO_DESCRIPTOR;
                }
            }
        }

        case HID_DTYPE_Report:
        {
            switch (index)
            {
                case INTF_BootKeyboard:
                {
                    *desc = &_kbd_boot;
                    return sizeof(_kbd_boot);
                }

                case INTF_NKROKeyboard:
                {
                    *desc = &_kbd_nkro;
                    return sizeof(_kbd_nkro);
                }

                default:
                {
                    *desc = NULL;
                    return NO_DESCRIPTOR;
                }
            }
        }

        default:
        {
            *desc = NULL;
            return NO_DESCRIPTOR;
        }
    }
}
