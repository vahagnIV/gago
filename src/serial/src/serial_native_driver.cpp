//
// Created by vahagn on 8/1/20.
// https://blog.mbedded.ninja/programming/operating-systems/linux/linux-serial-ports-using-c-cpp/#overview
//

#include "serial_native_driver.h"
// C library headers
#include <stdio.h>
#include <string.h>

// Linux headers
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()

#include <utility>

namespace gago {
namespace io {
namespace usb {

SerialNativeDriver::SerialNativeDriver(std::string path) : fd_(-1), path_(std::move(path)) {

}

int SerialNativeDriver::Open() {
  if (fd_ >= 0)
    return 0;
  fd_ = open(path_.c_str(), O_RDWR);
  if (fd_ < 0) {
    printf("Error %i from open: %s\n", errno, strerror(errno));
    fd_ = 0;
    return 1;
  }

  // Create new termios struct, we call it 'tty' for convention
  struct termios tty;
  memset(&tty, 0, sizeof tty);

// Read in existing settings, and handle any error
  if (tcgetattr(fd_, &tty) != 0) {
    printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
    close(fd_);
    fd_ = 0;
    return 1;
  }

  tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
  // tty.c_cflag |= PARENB;  // Set parity bit, enabling parity

  tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
  //tty.c_cflag |= CSTOPB;  // Set stop field, two stop bits used in communication

//  tty.c_cflag |= CS5; // 5 bits per byte
//  tty.c_cflag |= CS6; // 6 bits per byte
//  tty.c_cflag |= CS7; // 7 bits per byte
  tty.c_cflag |= CS8; // 8 bits per byte (most common)

  tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
//  tty.c_cflag |= CRTSCTS;  // Enable RTS/CTS hardware flow control

  tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)



  tty.c_lflag &= ~ICANON;

  tty.c_lflag &= ~ECHO; // Disable echo
  tty.c_lflag &= ~ECHOE; // Disable erasure
  tty.c_lflag &= ~ECHONL; // Disable new-line echo

  tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP

  tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl

  tty.c_iflag &=
      ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL); // Disable any special handling of received bytes

  tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
  tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
// tty.c_oflag &= ~OXTABS; // Prevent conversion of tabs to spaces (NOT PRESENT IN LINUX)
// tty.c_oflag &= ~ONOEOT; // Prevent removal of C-d chars (0x004) in output (NOT PRESENT IN LINUX)


  tty.c_cc[VTIME] = 10;    // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
  tty.c_cc[VMIN] = 0;

  // Set in/out baud rate to be 9600
  cfsetispeed(&tty, B9600);
  cfsetospeed(&tty, B9600);

  // Save tty settings, also checking for error
  if (tcsetattr(fd_, TCSANOW, &tty) != 0) {
    printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
    close(fd_);
    fd_ = 0;
    return 1;
  }

  return 0;
}

void SerialNativeDriver::WriteData(const std::vector<uint8_t> &data) {
  write(fd_, data.data(), data.size());

}

int SerialNativeDriver::ReadChar(char &c) {

  return read(fd_, (char *) &c, 1) == 1 ? 0 : 1;
}

int SerialNativeDriver::Read(int &r) {

  int n = 0;
  do {
    n += read(fd_, (char *) &r + n, sizeof(r) - n);
  } while (n && n != sizeof(r));

  return n ? 0 : -1;
}

void SerialNativeDriver::Close() {
  if (fd_ >= 0)
    close(fd_);
  fd_ = -1;

}

SerialNativeDriver::~SerialNativeDriver() {
  Close();
}

}
}
}