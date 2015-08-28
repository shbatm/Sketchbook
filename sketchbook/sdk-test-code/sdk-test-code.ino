//sdk-test-code.ino

extern "C"
{
    #include "ets_sys.h"
    #include "os_type.h"
    #include "mem.h"
    #include "osapi.h"
    #include "user_interface.h"

    #include "espconn.h"
    #include "gpio.h"
}

void setup() {
    Serial.begin(115200);
    system_uart_swap();
    system_print_meminfo();
}

void loop() {
    os_printf("thing\n");
    delay(1);
}
