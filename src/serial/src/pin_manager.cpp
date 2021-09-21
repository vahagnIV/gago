//
// Created by vahagn on 21.09.21.
//

#include "pin_manager.h"

namespace serial {

const uint8_t PinManager::SET = 1;
const uint8_t PinManager::GET = 2;

PinManager::PinManager(std::string device_path) : driver_(std::move(device_path)) {

}

bool PinManager::Initialize() {
  if (driver_.Open() == 0)
    return true; // TODO: synchronize
  return false;
}

bool PinManager::Execute(setPins::Request &request, setPins::Response &response) {
  static uint8_t DIGITAL_FLAG = 1;
  static uint8_t OK_FLAG = 1;
  static uint8_t ERROR_FLAG = 2;
  for (auto &command: request.commands) {
    static const uint64_t mask = 0xFF;
    uint8_t CMD = command & mask;
    command = command >> 8;
    uint8_t pin = command & mask;

    switch (CMD) {
      case SET: {
        command = command >> 8;
        uint8_t flags = command & mask;

        command = command >> 8;
        uint8_t velocity = command & mask;

        uint32_t arduino_command = 0;
        arduino_command = flags & DIGITAL_FLAG ? 129 : 1;
//        arduino_command = (arduino_command << 8) | velocity;
        arduino_command |= (pin << 8);
        arduino_command |= (velocity << 16);

        driver_.Write(arduino_command);

      }

    }
  }
  return true;
}

}