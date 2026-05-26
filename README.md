# Monitor del sistema

## Equipo 
1. Ledesma Cuevas Eduardo
2. Mendez Camacho Monserrat

## Descripción

Monitor del sistema para Linux escrito en C

## Caracteristicas

- Uso de memoria RAM
- Uso de Swap
- Uptime del sistema
- Uso de Red
- Uso de Disco
- Porcentaje de uso del CPU
- Listado de procesos en ejecución

## Estructura del proyecto

```
├── include
│   ├── cpu.h
│   ├── disk.h
│   ├── memory.h
│   ├── network.h
│   ├── process.h
│   ├── ui.h
│   └── uptime.h
├── Makefile
├── README.md
└── src
    ├── cpu.c
    ├── disk.c
    ├── main.c
    ├── memory.c
    ├── network.c
    ├── process.c
    ├── ui.c
    └── uptime.c
```

## Uso

### Compilar

```bash
mkdir -p build
```

```bash
make
```

```bash
./build/top
```