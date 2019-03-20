#include "firmware_version.h"


static const version_t __version = {VERSION_MAJOR, VERSION_MINOR, VERSION_BUILD};

const version_t *version_instance()
{
    return &__version;
}

u8_t version_get_major()
{
    return __version.major;
}

u8_t version_get_minor()
{
    return __version.minor;
}
u16_t version_get_build()
{
    return __version.build;
}
