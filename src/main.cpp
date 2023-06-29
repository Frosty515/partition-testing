#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include <sys/stat.h>

#include "GPT.hpp"
#include "MBR.hpp"
#include "Disk.hpp"

int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <image>\n", argv[0]);
        return 1;
    }

    Disk* disk = new Disk(argv[1]);

    MasterBootRecord* mbr = (MasterBootRecord*)malloc(SECTOR_SIZE);
    assert(disk->Read((uint8_t*)mbr, 0, 1));

    GPT_PartitionTableHeader* GPT_header = (GPT_PartitionTableHeader*)malloc(SECTOR_SIZE);
    assert(disk->Read((uint8_t*)GPT_header, mbr->Entries[0].StartingLBA, 1));

    assert(strncmp(GPT_header->Signature, "EFI PART", 8) == 0);

    GPT_PartitionTableHeader* GPT_header2 = (GPT_PartitionTableHeader*)malloc(SECTOR_SIZE);
    assert(disk->Read((uint8_t*)GPT_header2, GPT_header->AlternateHeaderLBA, 1));

    assert(strncmp(GPT_header2->Signature, "EFI PART", 8) == 0);
    assert(GPT_header2->AlternateHeaderLBA == mbr->Entries[0].StartingLBA);

    return 0;
}
