# FramingLib

Библиотека для работы с фреймами и кусками(чанк) данными в *types.h*.

Данные, которые невозможно передать в одном чанке, разбиваются на несколько чанков.
Каждый чанк содержит свой порядковый номер и общее кол-во чанков.
Объеденив эти чанки в один массив данных получим фрейм данных.

Описание структуры чанка

```c
typedef struct
{
    uint8_t     channel;     // логический номер канала
    uint8_t     reserved[1]; // резерв
    uint16_t    size;        // количестов данных в чанке
    uint16_t    number;      // номер чанка
    uint16_t    total;       // всего чанков
    uint32_t    crc;         // контрольная сумма payload
    uint8_t     payload[];   // данные
} chunk_t;
```

## Зависимости

* [nanocobs](https://github.com/charlesnicholson/nanocobs)
* [Ring-Buffer](https://github.com/Mcublog/Ring-Buffer)
