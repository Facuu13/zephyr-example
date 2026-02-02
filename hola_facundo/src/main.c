
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

int main(void)
{
    printk("Hola Facundo! Zephyr corriendo en ESP32-C3 😄\n");

    while (1) {
        printk("tick: %lld ms\n", k_uptime_get());
        k_sleep(K_SECONDS(1));
    }

    return 0;
}

