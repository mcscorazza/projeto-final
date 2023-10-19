#include <stdio.h>

#include "bsp/board.h"
#include "tusb.h"
#include "usb_descriptors.h"

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

#define TOTAL_INPUTS 5

void usb_hid_task();

void init_botoes();
uint16_t ler_botoes();
void ler_direcional();

struct report {
    uint16_t botoes;
    int8_t x;
    int8_t y;
} report;

enum entradas {
    GPIO_0 = 0,
    GPIO_1,
    GPIO_2,
    GPIO_3,
    GPIO_4,
};

int main() {
    board_init();
    tusb_init();
    init_botoes();

    while(true) {
        tud_task();
        usb_hid_task();
    }

    return 0;
}

uint16_t ler_botoes() {
    uint16_t resultado = 0;
    for(int i = 4; i < TOTAL_INPUTS; i++){
        if(gpio_get(i) == 0) {
            uint16_t aux = 1 << (i-4);
            resultado = resultado | aux;
        }
    }
    return resultado;
};

void ler_direcional() {
    if (gpio_get(GPIO_0) == 0) {
        report.y = -127;
    } else if (gpio_get(GPIO_1) == 0 ) {
        report.y = 127;
    } else {
        report.y = 0;
    }

    if (gpio_get(GPIO_2) == 0) {
        report.x = -127;
    } else if (gpio_get(GPIO_3) == 0 ) {
        report.x = 127;
    } else {
        report.x = 0;
    }
};

void init_botoes() {
    for(int i = 0; i < TOTAL_INPUTS; i++) {
        gpio_init(i);
        gpio_set_dir(i, GPIO_IN);
        gpio_set_pulls(i, true, false);
    }
};

void tud_suspend_cb(bool remote_wakeup_en) {
    (void) remote_wakeup_en;
};

uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t *buffer, uint16_t reqlen) {
    (void) instance;
    (void) report_id;
    (void) report_type;
    (void) buffer;
    (void) reqlen;

    return 0;
};

void tud_hid_set_report_cb(uint8_t intance, uint8_t report_id, hid_report_type_t report_type, uint8_t const *buffer, uint16_t bufsize) {
    tud_hid_report(0, buffer, bufsize);
};

void usb_hid_task() {
    const uint32_t intervalo_ms = 1;
    static uint32_t inicio_ms = 0;
    if (board_millis() - inicio_ms < intervalo_ms) {
        return;
    }

    report.botoes = ler_botoes();
    ler_direcional();

    if(tud_suspended()) {
        tud_remote_wakeup();
    }

    if(tud_hid_ready()){
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, true);
        tud_hid_n_report(REPORT_ID_GAMEPAD, REPORT_ID_GAMEPAD, &report, sizeof(report));
    }
};