// sw/driver/hdu_driver.cpp
// Simple file-backed MMIO mock

#include "hdu_driver.h"
#include <fstream>
#include <filesystem>
#include <stdexcept>
#include <iostream>
#include <cstring>

HduDriver::HduDriver(const std::string &mmio_dir)
  : mmio_dir_(mmio_dir)
{
    ensure_dirs();
}

HduDriver::~HduDriver() {}

void HduDriver::ensure_dirs() {
    std::filesystem::create_directories(mmio_dir_);
}

// write 32-bit LE
void HduDriver::write32(const std::string &name, uint32_t value) {
    std::string path = mmio_dir_ + "/" + name;
    std::ofstream f(path, std::ios::binary | std::ios::trunc);
    if (!f) throw std::runtime_error("MMIO write32 failed: " + path);
    f.write(reinterpret_cast<const char*>(&value), sizeof(value));
    f.close();
}

// read 32-bit LE
uint32_t HduDriver::read32(const std::string &name) {
    std::string path = mmio_dir_ + "/" + name;
    std::ifstream f(path, std::ios::binary);
    if (!f) return 0;
    uint32_t v = 0;
    f.read(reinterpret_cast<char*>(&v), sizeof(v));
    f.close();
    return v;
}

// write 64-bit LE
void HduDriver::write64(const std::string &name, uint64_t value) {
    std::string path = mmio_dir_ + "/" + name;
    std::ofstream f(path, std::ios::binary | std::ios::trunc);
    if (!f) throw std::runtime_error("MMIO write64 failed: " + path);
    f.write(reinterpret_cast<const char*>(&value), sizeof(value));
    f.close();
}

// read 64-bit LE
uint64_t HduDriver::read64(const std::string &name) {
    std::string path = mmio_dir_ + "/" + name;
    std::ifstream f(path, std::ios::binary);
    if (!f) return 0;
    uint64_t v = 0;
    f.read(reinterpret_cast<char*>(&v), sizeof(v));
    f.close();
    return v;
}

// Public API implementations

void HduDriver::write_cam_entry(uint32_t addr, uint16_t func_id, uint64_t token) {
    // For the simple file-based MMIO, write registers:
    // CFG_ADDR, CFG_FUNCID, CFG_TOKEN_LO, CFG_TOKEN_HI, CFG_WR_EN
    write32("CFG_ADDR", addr);
    write32("CFG_FUNCID", static_cast<uint32_t>(func_id));
    write32("CFG_TOKEN_LO", static_cast<uint32_t>(token & 0xffffffffULL));
    write32("CFG_TOKEN_HI", static_cast<uint32_t>(token >> 32));
    write32("CFG_WR_EN", 1u);
    // Caller can clear the pulse by writing 0 or we leave it for harness to detect
}

uint32_t HduDriver::read_alloc_count() {
    return read32("ALLOC_CNT");
}

uint32_t HduDriver::read_fail_count() {
    return read32("FAIL_CNT");
}

uint32_t HduDriver::read_overflow_count() {
    return read32("OVERFLOW_CNT");
}

std::optional<uint64_t> HduDriver::read_fallback_head() {
    uint64_t v = read64("FALLBACK_HEAD");
    if (v == 0) return std::nullopt;
    // Optionally clear it (driver acknowledges)
    write64("FALLBACK_HEAD", 0);
    return v;
}

bool HduDriver::poll_irq() {
    return read32("IRQ_STATUS") != 0;
}

void HduDriver::clear_irq() {
    write32("IRQ_STATUS", 0);
}
