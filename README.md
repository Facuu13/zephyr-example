# Zephyr + ESP32-C3 — Hola Mundo desde cero

---



Este proyecto es un **ejemplo mínimo** para aprender a crear una aplicación **propia** en **Zephyr RTOS**, compilada y flasheada sobre una **ESP32-C3**, sin usar los samples oficiales.

El objetivo es:
- Entender el **flujo real de Zephyr**
- Separar correctamente **workspace Zephyr** y **apps propias**
- Tener una base limpia para futuros proyectos (GPIO, botones, BLE, MQTT, etc.)

---

## 📦 Estructura del entorno

El setup recomendado (y utilizado en este proyecto) es:

```text
~/zephyr/
└── zephyrproject/        ← Workspace Zephyr (kernel, drivers, west)

~/github/zephyr-example/
└── hola_facundo/         ← App Zephyr (este repo)
````

* El **workspace Zephyr** vive en un directorio dedicado
* Las **apps viven donde vos quieras** (ideal para GitHub)
* `west` siempre se ejecuta **desde el workspace**

---

## 🛠 Requisitos

* Linux (Ubuntu recomendado)
* ESP32-C3 (ej: `esp32c3_devkitm`)
* Python ≥ 3.10
* Zephyr SDK instalado
* `west` funcionando
* `esptool` instalado (`west packages pip --install`)

---

## 📁 Estructura del proyecto

```text
hola_facundo/
├── CMakeLists.txt
├── prj.conf
├── src/
│   └── main.c
├── build/               ← generado (ignorado por git)
└── .gitignore
```

---

## 🧱 Archivos principales

### `CMakeLists.txt`

```cmake
cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(hola_facundo)

target_sources(app PRIVATE src/main.c)
```

---

### `prj.conf`

```conf
# Consola y serial
CONFIG_SERIAL=y
CONFIG_CONSOLE=y
CONFIG_UART_CONSOLE=y

# Logging
CONFIG_LOG=y
CONFIG_LOG_DEFAULT_LEVEL=3
```

---

### `src/main.c`

```c
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
```

---

## 🚀 Compilación del proyecto

> ⚠️ IMPORTANTE
> `west` **SIEMPRE** se ejecuta desde el workspace Zephyr.

### 1️⃣ Activar entorno

```bash
cd ~/zephyr/zephyrproject
source ../.venv/bin/activate
```

---

### 2️⃣ Compilar la app (build limpio)

```bash
west build -b esp32c3_devkitm -p always \
  -d ~/github/zephyr-example/hola_facundo/build \
  ~/github/zephyr-example/hola_facundo
```

* `-b esp32c3_devkitm` → board
* `-p always` → build limpio
* `-d` → directorio de build (dentro del repo)
* último argumento → path de la app

---

## 🔥 Flasheo en la ESP32-C3

Con la placa conectada por USB:

```bash
west flash -d ~/github/zephyr-example/hola_facundo/build
```

---

## 🖥 Monitor serial

```bash
west espressif monitor
```

Salida esperada:

```text
*** Booting Zephyr OS ***
Hola Facundo! Zephyr corriendo en ESP32-C3 😄
tick: 1000 ms
tick: 2000 ms
...
```

Salir del monitor:

* `Ctrl + ]` o `Ctrl + C`

---

## 🧹 `.gitignore`

```gitignore
/build/
```

---

## 🧠 Conceptos clave aprendidos

* Zephyr separa **workspace** y **aplicaciones**
* `west` pertenece al workspace, no a la app
* `CMakeLists.txt` debe agregar fuentes explícitamente (`target_sources`)
* `prj.conf` controla el kernel y subsistemas
* ESP32-C3 funciona muy bien con Zephyr (RISC-V)

---

