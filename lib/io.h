#ifndef __IO_H__
#define __IO_H__

#include <stdint.h>

#define bytes_out_by_port(port, value) ({       \
        asm volatile("out dx, al"               \
                     :                          \
                     : "a" (value), "d" (port)  \
                     : );                       \
})

#endif 