#ifndef _GPT_HPP
#define _GPT_HPP

#include <stdint.h>

struct GPT_PartitionTableHeader {
    char Signature[8]; // Should be "EFI PART"
    uint32_t Revision;
    uint32_t HeaderSize;
    uint32_t HeaderChecksum; // CRC32
    uint32_t Reserved0;
    uint64_t HeaderLBA; // The LBA containing this header
    uint64_t AlternateHeaderLBA; // The LBA of the alternate GPT header
    uint64_t FirstUsable; // The first usable block that can be contained in a GPT entry
    uint64_t LastUsable; // The last usable block that can be contained in a GPT entry
    uint8_t GUID[16];
    uint64_t GPEntryArrayLBA; // Starting LBA of the GUID Partition Entry array
    uint32_t TotalPartitionEntries;
    uint32_t EntrySize; // Size in bytes of each entry in the Partition Entry Array - must be a value of 128 * 2^n where n >= 0
    uint32_t GPEntryArrayChecksum; // CRC32
} __attribute__((packed));

struct GPT_PartitionTableEntry {
    uint8_t PartitionTypeGUID[16]; // zero for unused entry
    uint8_t UniquePartitionGUID[16];
    uint64_t StartingLBA;
    uint64_t EndingLBA;
    uint64_t Attributes;
    uint16_t PartitionName[36]; // 2-byte characters. size should be PartitionTableEntrySize - 0x38
} __attribute__((packed));

#endif /* _GPT_HPP */