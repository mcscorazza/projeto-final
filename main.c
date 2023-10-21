#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "bsp/board.h"
#include "tusb.h"
#include "pico/stdlib.h"

#define TOTAL_INPUTS 5
#define PICO_DEFAULT_LED_PIN 25

void hid_task(void);
void init_buttons();
uint16_t read_fire_button();
void read_arrows();


// Estruturação do REPORT
struct report
{
    uint16_t buttons;
    int8_t x;
    int8_t y;
} report;

// Enumeração dos GPIOs utilizados no controle
enum control_ios{
    GPIO_0 = 0,
    GPIO_1 = 1,
    GPIO_2 = 2,
    GPIO_3 = 3,
    GPIO_4 = 4
};

// Programa Principal
int main(void)
{
    board_init();
    init_buttons();
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    gpio_put(PICO_DEFAULT_LED_PIN, 1);
    tusb_init();

    while (1)
    {
        hid_task();
        tud_task();
    }

    return 0;
}

void con_panic(uint16_t errcode) 
{

    report.buttons = errcode;
    while (1)
    {
        tud_task();
        if (tud_suspended())
        {
            tud_remote_wakeup();
        }

        if (tud_hid_ready())
        {
            tud_hid_n_report(0x00, 0x01, &report, 18);
        }
    }
}

// Leitura dos direcionais do controle
void read_arrows (){
    if ( gpio_get(GPIO_0) == 0) 
        report.y = -127;
    else if ( gpio_get(GPIO_1) == 0) 
        report.y = 127;
    else 
        report.y = 0;

    if ( gpio_get(GPIO_2) == 0) 
        report.x = -127;
    else if ( gpio_get(GPIO_3) == 0) 
        report.x = 127;
    else 
        report.x = 0;
}


void hid_task(void)
{
    const uint32_t interval_ms = 1;
    static uint32_t start_ms = 0;

    if (board_millis() - start_ms < interval_ms)
        return;
    start_ms += interval_ms;
    
    report.buttons = read_fire_button();
    read_arrows();
    
    if (tud_suspended())
    {
        tud_remote_wakeup();
    }

    if (tud_hid_ready()){
        gpio_put(PICO_DEFAULT_LED_PIN, 1);
        tud_hid_n_report(0x00, 0x01, &report, sizeof(report));
    }
}

// Inicialização de todos os GPIOs
void init_buttons() {
    for( int i = 0; i < TOTAL_INPUTS; i++) {
        gpio_init(i);
        gpio_set_dir(i, GPIO_IN);
        gpio_set_pulls (i,true,false); 
    }
}

// Leitura do Fire Button
uint16_t read_fire_button() {
    uint16_t ret = 0;
    if(gpio_get(GPIO_4) == 0 ) {
        uint16_t aux = 1 << (0);
        ret = ret | aux;
    }
    return ret;
}

//--------------------------------------------------------------------+
// Device callbacks
//--------------------------------------------------------------------+
void tud_mount_cb(void)
{
}

void tud_umount_cb(void)
{
}

void tud_suspend_cb(bool remote_wakeup_en)
{
    (void)remote_wakeup_en;
}

void tud_resume_cb(void)
{
}

//--------------------------------------------------------------------+
// USB HID
//--------------------------------------------------------------------+
uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t *buffer, uint16_t reqlen)
{
    (void)report_id;
    (void)report_type;
    (void)buffer;
    (void)reqlen;

    return 0;
}

void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const *buffer, uint16_t bufsize)
{
    tud_hid_report(0, buffer, bufsize);
}