/*
  Copyright (c) 2017 Arduino LLC.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef _SERIALFLASH_STORAGE_H_INCLUDED
#define _SERIALFLASH_STORAGE_H_INCLUDED

#include <SerialFlash.h>

#include "OTAStorage.h"

#define SERIAL_FLASH_BUFFER_SIZE    64
#define SERIAL_FLASH_CS             5

class SerialFlashStorageClass : public ExternalOTAStorage {
public:

  int open(int length) override {
    if (!SerialFlash.begin(SERIAL_FLASH_CS)) {
      return 0;
    }

    while (!SerialFlash.ready()) {}

    if (SerialFlash.exists(updateFileName)) {
      SerialFlash.remove(updateFileName);
    }

    if (SerialFlash.create(updateFileName, length)) {
      _file = SerialFlash.open(updateFileName);
    }

    if (!_file) {
      return 0;
    }

    return 1;
  }

  size_t write(uint8_t b) override {
    while (!SerialFlash.ready()) {}
    int ret = _file.write(&b, 1);
    return ret;
  }

  void close() override {
    _file.close();
  }

  void clear() override {
    SerialFlash.remove(updateFileName);
  }

private:
  SerialFlashFile _file;
};

#endif
