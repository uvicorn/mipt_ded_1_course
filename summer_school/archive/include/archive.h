#ifndef ARCHIVE_H
#define ARCHIVE_H

#include "utils.h"
#include <stdint.h>
#include "output.h"

Output encode_buffer(const uint8_t* read_buffer, size_t size);

#endif
