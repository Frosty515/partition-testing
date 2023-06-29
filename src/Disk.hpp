#ifndef _DISK_HPP
#define _DISK_HPP

#include <stdio.h>
#include <stdint.h>

#define SECTOR_SIZE 512

class Disk {
public:
    Disk();
    Disk(const char* path);
    ~Disk();

    bool Read(uint8_t* buffer, uint64_t lba, uint64_t count = 1) const;
    bool Write(const uint8_t* buffer, uint64_t lba, uint64_t count = 1) const;

    void SetFile(const char* path);

    const char* GetFile() const;

    size_t GetSectorCount() const;

private:
    FILE* m_file;
    char const* m_path;
    bool m_ready;
};

#endif /* _DISK_HPP */