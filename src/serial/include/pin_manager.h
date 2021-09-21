//
// Created by vahagn on 21.09.21.
//

#ifndef SRC_SERIAL_INCLUDE_PIN_MANAGER_H_
#define SRC_SERIAL_INCLUDE_PIN_MANAGER_H_

#include "serial_native_driver.h"
#include "serial/setPins.h"
namespace serial{

class PinManager {
 public:
  PinManager(std::string device_path);
  bool Initialize();
  bool Execute(serial::setPins::Request & request, serial::setPins::Response & response);
 private:
  gago::io::usb::SerialNativeDriver driver_;
  static const uint8_t SET;
  static const uint8_t GET;

};
}

#endif //SRC_SERIAL_INCLUDE_PIN_MANAGER_H_
