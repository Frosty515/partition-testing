#ifndef _MBR_HPP
#define _MBR_HPP

#include <stdint.h>

struct MasterBootRecordEntry {
    uint8_t BootIndicator;
    uint8_t StartingCHS[3];
    uint8_t OSType;
    uint8_t EndingCHS[3];
    uint32_t StartingLBA;
    uint32_t EndingLBA;
} __attribute__((packed));

struct MasterBootRecord {
    uint8_t Bootstrap[440];
    uint32_t UniqueDiskID; // Optional
    uint16_t Reserved; // Optional
    MasterBootRecordEntry Entries[4];
    uint16_t Signature; // should always be 0xAA55 (little endian)
} __attribute__((packed));

#endif /* _MBR_HPP */