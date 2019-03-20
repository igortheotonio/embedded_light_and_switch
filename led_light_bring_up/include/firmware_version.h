#ifndef _VERSION_H_
#define _VERSION_H_

#include <zephyr/types.h>

typedef struct {
    u8_t major;
    u8_t minor;
    u16_t build;
} version_t;

const version_t *version_instance();
u8_t version_get_major();
u8_t version_get_minor();
u16_t version_get_build();

#endif  // _VERSION_H_
