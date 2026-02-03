#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>

/* Tomamos el LED desde el alias 'led0' que definimos en el overlay */
#define LED0_NODE DT_ALIAS(led0)

#if !DT_NODE_HAS_STATUS(LED0_NODE, okay)
#error "No se encontró el alias led0 en el DeviceTree. Revisa el overlay."
#endif

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

int main(void)
{
    if (!gpio_is_ready_dt(&led)) {
        printk("GPIO no listo para LED\n");
        return 0;
    }

    int ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE);
    if (ret < 0) {
        printk("Error configurando LED: %d\n", ret);
        return 0;
    }

    printk("Blinky OK: parpadeando LED desde DeviceTree\n");

    while (1) {
        gpio_pin_toggle_dt(&led);
        k_sleep(K_MSEC(500));
    }

    return 0;
}
