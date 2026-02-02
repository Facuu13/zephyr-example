````md
# Zephyr + ESP32-C3 — Crear tu propio proyecto desde cero (paso a paso)

> Objetivo: crear un proyecto **nuevo** (no un sample), compilarlo para **ESP32-C3** y flashearlo.
> Placa: `esp32c3_devkitm` (si usás otra, cambiá el `-b`).

---

## 0) Requisitos previos 
- Workspace Zephyr en: `~/zephyr/zephyrproject`
- Venv en: `~/zephyr/.venv`
- Zephyr SDK instalado
- `west` funcionando
- `esptool` instalado (por `west packages pip --install`)
- Probaste `hello_world` y anduvo ✅

---

## 1) Entrar al workspace y activar entorno

```bash
cd ~/zephyr/zephyrproject
source ../.venv/bin/activate
````

> Tip: cada vez que abras una terminal nueva, hacé esto.

---

## 2) Crear una carpeta para tus apps (afuera de `zephyr/`)

Es buena práctica no mezclar tus proyectos con el repo de Zephyr.

```bash
mkdir -p ~/zephyr/apps
cd ~/zephyr/apps
```

---

## 3) Crear tu nuevo proyecto: `hola_facundo`

```bash
mkdir -p hola_facundo/src
cd hola_facundo
```

Estructura mínima que vamos a armar:

```text
hola_facundo/
├── CMakeLists.txt
├── prj.conf
└── src/
    └── main.c
```

---

## 4) Crear los archivos mínimos

### 4.1) `CMakeLists.txt`

Crealo así:

```bash
cat > CMakeLists.txt << 'EOF'
cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(hola_facundo)

target_sources(app PRIVATE src/main.c)
EOF
```

---

### 4.2) `prj.conf`

Este archivo habilita configuraciones del kernel/log. Para arrancar, lo dejamos simple:

```bash
cat > prj.conf << 'EOF'
# Habilita consola/serial para ver prints
CONFIG_SERIAL=y
CONFIG_CONSOLE=y
CONFIG_UART_CONSOLE=y

# Logging (opcional, útil para proyectos reales)
CONFIG_LOG=y
CONFIG_LOG_DEFAULT_LEVEL=3
EOF
```

> Nota: `printk()` suele funcionar igual, pero LOG te conviene para proyectos reales.

---

### 4.3) `src/main.c`

Tu “hola” personalizado:

```bash
cat > src/main.c << 'EOF'
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
EOF
```

---

## 5) Compilar tu proyecto para ESP32-C3

Volvé al workspace (o pasá el path absoluto). Recomendado: compilar desde el workspace.

```bash
cd ~/zephyr/zephyrproject
```

Compilá indicando el path de tu app:

```bash
west build -b esp32c3_devkitm -p ~/zephyr/apps/hola_facundo
```

* `-b esp32c3_devkitm` = tu placa
* `-p` = build limpio

Si tu placa fuera otra (ej. `esp32c3_devkitc`):

```bash
west build -b esp32c3_devkitc -p ~/zephyr/apps/hola_facundo
```

---

## 6) Flashear a la placa

Conectá la ESP32-C3 por USB y ejecutá:

```bash
west flash
```

> Si te pide el puerto o hay dudas, mirá qué aparece:
> `ls /dev/ttyUSB* /dev/ttyACM* 2>/dev/null`

---

## 7) Ver la salida por serial (monitor)

```bash
west espressif monitor
```

Deberías ver algo así:

```text
*** Booting Zephyr OS ***
Hola Facundo! Zephyr corriendo en ESP32-C3 😄
tick: 1000 ms
tick: 2000 ms
...
```

Salir del monitor:

* `Ctrl + ]` (o a veces `Ctrl + C` depende del monitor)

---

## 8) Ciclo típico de desarrollo (rápido)

1. Editás `src/main.c`
2. Rebuild incremental:

```bash
west build
```

3. Flash:

```bash
west flash
```

4. Monitor:

```bash
west espressif monitor
```

> Si cambiás cosas “grandes” (Kconfig, device tree, board, etc.), usá:
> `west build -p always`

---

## 9) Buenas prácticas (desde ya)

### 9.1) No copies tu app dentro de `zephyr/`

Mantené tus apps en `~/zephyr/apps/` (o un repo Git propio).

### 9.2) Build directory separado (opcional)

Podés compilar en un directorio específico:

```bash
west build -b esp32c3_devkitm -p -d build_hola ~/zephyr/apps/hola_facundo
```

Y luego:

```bash
west flash -d build_hola
west espressif monitor -d build_hola
```

---

## 10) Próximo paso recomendado (para entender Zephyr de verdad)

Después del “hola”, lo ideal es:

1. **Blinky (LED)** usando `gpio_dt_spec` + DeviceTree
2. **Botón con interrupción** (GPIO callback)
3. **Thread + queue / message passing**
4. **Kconfig propio** para configurar comportamiento

Decime cuál querés hacer primero 😄

```
::contentReference[oaicite:0]{index=0}
```
