// onion_builder.h
#ifndef ONION_BUILDER_H
#define ONION_BUILDER_H

#include <stdint.h>
#include <stddef.h>

// Build full 3-layer onion
uint8_t* build_onion(const char *message, size_t *out_len);

#endif
