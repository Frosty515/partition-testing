#include "Disk.hpp"

#include <sys/stat.h>

/* Disk class */

/* Public methods */

Disk::Disk() : m_file(nullptr), m_path(nullptr), m_ready(false) {

}

Disk::Disk(const char* path) : m_file(nullptr), m_path(path), m_ready(false) {
    m_file = fopen(path, "rb");
    m_ready = m_file != nullptr;
}

Disk::~Disk() {

}

bool Disk::Read(uint8_t* buffer, uint64_t lba, uint64_t count) const {
    if (!m_ready)
        return false;
    rewind(m_file);
    int rc = fseek(m_file, lba * SECTOR_SIZE, SEEK_SET);
    if (rc != 0)
        return false;
    size_t rc2 = fread(buffer, SECTOR_SIZE, count, m_file);
    return rc2 == count;
}

bool Disk::Write(const uint8_t* buffer, uint64_t lba, uint64_t count) const {
    if (!m_ready)
        return false;
    rewind(m_file);
    int rc = fseek(m_file, lba * SECTOR_SIZE, SEEK_SET);
    if (rc != 0)
        return false;
    size_t rc2 = fwrite(buffer, SECTOR_SIZE, count, m_file);
    return rc2 == count;
}

void Disk::SetFile(const char* path) {
    fclose(m_file);
    m_ready = false;
    m_path = path;
    m_file = fopen(m_path, "rb");
    m_ready = m_file != nullptr;
}

const char* Disk::GetFile() const {
    return m_path;
}

size_t Disk::GetSectorCount() const {
    if (!m_ready)
        return 0;
    struct stat stat_buf;
    int rc = stat(m_path, &stat_buf); 
    return rc == 0 ? (stat_buf.st_size / SECTOR_SIZE) : 0;
}
