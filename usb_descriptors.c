#include "tusb.h"
#include "usb_descriptors.h"

#define USB_VID 0xCAFE
#define USB_PID 0x2023

tusb_desc_device_t const desc_device = {
    .bLength = sizeof(tusb_desc_device_t),
    .bDescriptorType = TUSB_DESC_DEVICE,
    .bcdUSB = 0x200,
    .bDeviceSubClass = 0x00,
    .bDeviceProtocol = 0x00,
    .bMaxPacketSize0 = CFG_TUD_ENDPOINT0_SIZE,

    .idVendor  = USB_VID,
    .idProduct = USB_PID,
    .bcdDevice = 0x0100,

    .iManufacturer = 0x01,
    .iProduct = 0x02,
    .iSerialNumber = 0x03,

    .bNumConfigurations = 0x01
};

uint8_t const desc_hid_report[] = {
    GAMECON_REPORT_DESC_GAMEPAD(HID_REPORT_ID(REPORT_ID_GAMEPAD)),
};

uint8_t const *tud_hid_descriptor_report_cb(uint8_t instance) {
    return desc_hid_report;
};

enum {
    ITF_GAMEPAD,
    ITF_NUM_TOTAL
};

#define CONFIG_TOTAL_LEN (TUD_CONFIG_DESC_LEN + TUD_HID_DESC_LEN)

uint8_t const desc_configuration[]= {
    TUD_CONFIG_DESCRIPTOR(1,ITF_NUM_TOTAL, 0, CONFIG_TOTAL_LEN, TUSB_DESC_CONFIG_ATT_REMOTE_WAKEUP, 100),
    TUD_HID_DESCRIPTOR(ITF_GAMEPAD, 0, HID_ITF_PROTOCOL_NONE, sizeof(desc_hid_report), 0x81, CFG_TUD_HID_BUFSIZE, 1)
};

uint8_t const *tud_descriptor_configuration_cb(uint8_t index) {
    (void) index;
    return desc_configuration;
};

char const *string_desc_arr[] = {
    (const char[]){0x09, 0x04},
    "UNISAL",
    "UNISAL-ATARI",
    "123456",
};

static uint16_t desc_str[32];

uint16_t const *tud_descriptor_string_cb(uint8_t index, uint16_t langid){
    (void) langid;
    uint8_t char_cont;
    if(index==0) {
        memcpy(&desc_str[1], string_desc_arr[0], 2);
        char_cont = 1;
    } else {
        if(!(index < sizeof(string_desc_arr) / sizeof(string_desc_arr[0])))
            return NULL;
        
        const char *str = string_desc_arr[index];

        char_cont = strlen(str);
        if(char_cont > 31) 
            char_cont = 31;
        for(uint8_t i = 0; i < char_cont; i++) {
            desc_str[1 + i] = str[i];
        }
    }

    desc_str[0] = (TUSB_DESC_STRING << 8) | (2 * char_cont + 2);
    
    return desc_str;
}