//
// Created by vahagn on 21.09.21.
//

#ifndef SRC_SERIAL_INCLUDE_PIN_MANAGER_H_
#define SRC_SERIAL_INCLUDE_PIN_MANAGER_H_

#include "serial_native_driver.h"
#include "serial/setPins.h"
namespace serial {

enum PinType {
  DIGITAL,
  PWN,
  RX,
  TX
};

class PinManager {
 public:
  explicit PinManager(std::string device_path);
  bool Initialize();
  bool Execute(serial::setPins::Request &request, serial::setPins::Response &response);
 private:
  static void SplitUint32(uint32_t &num,
                          uint8_t &out_byte1,
                          uint8_t &out_byte2,
                          uint8_t &out_byte3,
                          uint8_t &out_byte4) noexcept;
  static uint8_t VerifySetCommand(uint8_t pin, uint8_t flags);
  static uint8_t VerifyGetCommand(uint8_t pin, uint8_t flags);
 private:
  gago::io::usb::SerialNativeDriver driver_;
  static const uint8_t SET;
  static const uint8_t GET;
  static const uint8_t DIGITAL_FLAG;
  static const PinType PIN_TYPES[14];

};
}

#endif //SRC_SERIAL_INCLUDE_PIN_MANAGER_H_
