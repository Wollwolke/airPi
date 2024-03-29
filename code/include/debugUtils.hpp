#pragma once

#if DEBUG
#define D_printf(...) Serial.printf(__VA_ARGS__);
#define D_print(...) Serial.print(__VA_ARGS__)
#define D_write(...) Serial.write(__VA_ARGS__)
#define D_println(...) Serial.println(__VA_ARGS__)
#else
#define D_printf(...)
#define D_print(...)
#define D_write(...)
#define D_println(...)
#endif
