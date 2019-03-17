/**
 * @file firmware_version.h
 * @brief
 * @author Lucas Peixoto (lucaspeixotoac@gmail.com)
 * @version 0.1
 * @date 2019-03-06
 */


#ifndef _FIRMWARE_VERSION_H_
#define _FIRMWARE_VERSION_H_

#include <zephyr/types.h>

#include "current_version.h"


typedef struct {
    u8_t major;
    u8_t minor;
    u16_t build;
} version_t;

const version_t *version_instance();
u8_t version_get_major();
u8_t version_get_minor();
u16_t version_get_build();

#endif
