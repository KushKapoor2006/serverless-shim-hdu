#pragma once
// sw/driver/hdu_driver.h
// Lightweight file-backed "MMIO" driver for local co-simulation / testing.
// This is a mock driver: writes simple 32-bit registers into files under a directory.
// Useful during development when hardware isn't exposed via PCIe.

#include <cstdint>
#include <optional>
#include <string>

class HduDriver {
public:
    explicit HduDriver(const std::string &mmio_dir = "sw/mmio");
    ~HduDriver();

    // Write a CAM entry (addr: index, func_id: 16-bit, token: 64-bit)
    void write_cam_entry(uint32_t addr, uint16_t func_id, uint64_t token);

    // Read counters/irq (32-bit)
    uint32_t read_alloc_count();
    uint32_t read_fail_count();
    uint32_t read_overflow_count();

    // Fallback FIFO head (64-bit) — returns empty optional if none present
    std::optional<uint64_t> read_fallback_head();

    // Poll IRQ status (non-zero means IRQ pending)
    bool poll_irq();

    // Utility to clear IRQ status (driver acknowledges)
    void clear_irq();

private:
    std::string mmio_dir_;
    void ensure_dirs();
    void write32(const std::string &name, uint32_t value);
    uint32_t read32(const std::string &name);
    void write64(const std::string &name, uint64_t value);
    uint64_t read64(const std::string &name);
};
