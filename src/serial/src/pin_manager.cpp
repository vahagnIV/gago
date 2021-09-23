//
// Created by vahagn on 21.09.21.
//

#include "pin_manager.h"

namespace serial {

const uint8_t PinManager::SET = 1;
const uint8_t PinManager::GET = 2;
const uint8_t PinManager::DIGITAL_FLAG = 1;
const PinType PinManager::PIN_TYPES[] = {RX, TX, DIGITAL, PWN, DIGITAL, PWN, PWN, DIGITAL, DIGITAL, PWN, PWN, PWN,
                                         DIGITAL, DIGITAL};

PinManager::PinManager(std::string device_path) : driver_(std::move(device_path)) {

}

bool PinManager::Initialize() {
  if (driver_.Open() == 0)
    return true; // TODO: synchronize
  return false;
}

void PinManager::SplitUint32(uint32_t &num,
                             uint8_t &out_byte1,
                             uint8_t &out_byte2,
                             uint8_t &out_byte3,
                             uint8_t &out_byte4) noexcept {
  out_byte1 = num & 0xFF;
  out_byte2 = (num >> 8) & 0xFF;
  out_byte3 = (num >> 16) & 0xFF;
  out_byte4 = (num >> 24) & 0xFF;
}

uint8_t PinManager::VerifySetCommand(uint8_t pin, uint8_t flags) {
  if (pin != 0xFF && pin > sizeof(PIN_TYPES))
    return 1;
  if (0 == (flags & DIGITAL_FLAG) && PIN_TYPES[pin] != PWN)
    return 2;
  return 0;
}

uint8_t PinManager::VerifyGetCommand(uint8_t pin, uint8_t flags) {
  return 0;
}

bool PinManager::Execute(setPins::Request &request, setPins::Response &response) {

  for (auto command: request.commands) {

    uint8_t cmd, pin, flags, value;
    SplitUint32(command, pin, cmd, flags, value);

    uint32_t appreciation_message = pin;
    uint8_t verification_flags = cmd == SET ? VerifySetCommand(pin, flags) : VerifyGetCommand(pin, flags);
    if (verification_flags) {
      appreciation_message |= (cmd << 8);
      appreciation_message |= (verification_flags << 16);
    } else {
      driver_.Write(command);
      driver_.Read(appreciation_message);
    }
    response.responses.push_back(appreciation_message);
  }

  return true;
}

}