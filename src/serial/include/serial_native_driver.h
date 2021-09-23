//
// Created by vahagn on 8/1/20.
//

#ifndef LIBGAGO_SRC_IO_USB_SERIAL_H_
#define LIBGAGO_SRC_IO_USB_SERIAL_H_
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <unistd.h>

namespace gago {
namespace io {
namespace usb {

class SerialNativeDriver {
 public:
  explicit SerialNativeDriver(std::string path);
  int Open();
  void Close();
  void WriteData(const std::vector<uint8_t> &data);

  template<typename T>
  void Write(T data) {
    write(fd_, &data, sizeof(T));
  }

  template<typename T>
  int Read(T &r) {
    int n = 0;
    do {
      n += read(fd_, (char *) &r + n, sizeof(r) - n);
    } while (n && n != sizeof(r));

    return n ? 0 : -1;
  }

  int ReadChar(char &c);

  virtual ~SerialNativeDriver();
 private:
  int fd_;
  std::string path_;
  std::mutex mutex_;

};

}
}
}
#endif //LIBGAGO_SRC_IO_USB_SERIAL_H_
