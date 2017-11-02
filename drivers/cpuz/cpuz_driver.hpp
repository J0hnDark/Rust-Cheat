#pragma once

#include <cstdint>
#include <stdexcept>
#include <vector>
#include "../../windefs.h"

class cpuz_driver
{
  cpuz_driver();
  ~cpuz_driver();

  cpuz_driver(const cpuz_driver&)            = delete;
  cpuz_driver(cpuz_driver&&)                 = delete;
  cpuz_driver& operator=(const cpuz_driver&) = delete;
  cpuz_driver& operator=(cpuz_driver&&)      = delete;

public:
  static cpuz_driver& instance();

  bool ensure_loaded();
  bool load();
  bool unload();

  void unload_on_exit(bool enable) { unload_ = enable; }

  std::uint64_t read_cr0();
  std::uint64_t read_cr2();
  std::uint64_t read_cr3();
  std::uint64_t translate_linear_address(std::uint64_t directoryTableBase, LPVOID virtualAddress);
  
  bool read_physical_address(std::uint64_t address, LPVOID buf, size_t len);
  bool write_physical_address(std::uint64_t address, LPVOID buf, size_t len);
  bool read_system_address(LPVOID address, LPVOID buf, size_t len);
  bool write_system_address(LPVOID address, LPVOID buf, size_t len);


  template<typename T, typename U>
  T read_physical_address(U address)
  {
    T buf;

    if(!read_physical_address((std::uint64_t)address, (uint8_t*)&buf, sizeof(T)))
      throw std::runtime_error{ "Read failed" };

    return buf;
  }

  template<typename T, typename U>
  T read_system_address(U address)
  {
    T buf;

    if(!read_system_address((LPVOID)address, (uint8_t*)&buf, sizeof(T)))
      throw std::runtime_error{ "Read failed" };

    return buf;
  }

  template<typename T, typename U>
  bool write_physical_address(T address, U value)
  {
    return write_physical_address((LPVOID)address, (uint8_t*)&value, sizeof(U));
  }

  template<typename T, typename U>
  bool write_system_address(T address, U value)
  {
    return write_system_address((LPVOID)address, (uint8_t*)&value, sizeof(U));
  }
private:
  bool is_loaded();

  HANDLE serviceHandle_;
  HANDLE deviceHandle_;
  bool   unload_;
};
//(c) Apflmus