from __future__ import print_function

import argparse
from contextlib import redirect_stderr
from time import sleep

import serial
import struct
from progressbar import progressbar
from progress.bar import Bar

import sys

from fontTools.afmLib import error
from intelhex import IntelHex

COMMANDS = {
    'GET': 0x00,
    'FLACH_UNLOCK': 0x01,
    'FLASH_LOCK': 0x02,
    'FLASH_PROGRAM': 0x03,
    'FLASH_READ': 0x04,
    'FLASH_ERASE': 0x05,
    'FLASH_MASS_ERASE': 0x06,
    'FLASH_COPY': 0x07,
    'TRANSFER_CTRL': 0x08,
    'OB_UNLOCK': 0x09,
    'OB_LOCK': 0x0A,
    'OB_READ': 0x0B,
    'WR_PROTECT': 0x0C,
    'WR_UNPROTECT': 0x0D
}

ACK = 0x41
NACK = 0x4E

ERRORS = {
    0xE1: ' > Programming Sequence error.',
    0xE2: ' > Programming Parallelism error.',
    0xE3: ' > Programming Alignment error.',
    0xE4: ' > Write protection error.',
    0xE5: ' > Read Protection error.',
    0xE6: ' > Operation Error.'
}

CMD_WRITE = 0x03


def toInt(byte):
    return struct.unpack('b', byte)[0]


class ProgramModeError(Exception):
    pass


class TimeoutError(Exception):
    pass


class STM32Flasher(object):
    def __init__(self, serialPort, baudrate=115200):
        self.serial = serial.Serial(serialPort, baudrate=baudrate, timeout=30)

    def writeImage(self, filename):
        # Sends an CMD_WRITE to the bootloader
        # This is method is a generator, that returns its progresses to the caller.
        # In this way, it's possible for the caller to live-print messages about
        # writing progress
        hex_file = IntelHex()
        # load the hex file image.
        hex_file.loadhex(filename)

        current_address = hex_file.minaddr()
        MAX_ADDRESS = hex_file.maxaddr()
        file_content = hex_file.todict()

        with Bar('Loading', fill='#', suffix='%(percent).1f%% - %(elapsed).1fs',
                 max=int(len(file_content) / 16)) as bar:
            while current_address <= MAX_ADDRESS:

                data = []
                block_address = current_address
                # read block (16 bytes) from the file into data
                for i in range(16):
                    try:
                        data.append(file_content[current_address])
                    except KeyError:
                        # if we exceed the max addr and the HEX file doesn't contain a value for the given address
                        # we "pad" it with 0xFF, which corresponds to erased value
                        data.append(0xFF)
                    current_address += 1
                #   block was loaded from hex file
                try:
                    # Flush input buffer, discarding all its contents.
                    self.serial.flushInput()

                    # writing the block.
                    self.serial.write([COMMANDS['FLASH_PROGRAM']] + list(struct.pack("I", block_address)) + list(data))
                    #  check for acknowledgement by read the received byte
                    ret = self.serial.read(1)

                    if ret == ACK:
                        bar.next()
                    else:
                        # raise an error if not receiving an ack
                        raise ProgramModeError("Error Occurred!")
                except ProgramModeError:  # if any error happened, increase resend count
                    yield f'\nThe following error(s) occurred while writing at address :  {hex(block_address)}\n'
                    num_errs = self.serial.read(1)
                    errors = self.serial.read(toInt(num_errs))
                    for err in errors:
                        yield ERRORS[err] + '\n'
                    yield 'Operation Failed!'
                    return
        bar.finish()
        yield 'Image has been written successfully!'

    def unlockFlash(self):
        self.serial.write([COMMANDS['FLACH_UNLOCK']])
        #  check for acknowledgement by read the received byte
        ret = self.serial.read(1)
        try:
            if ret == ACK:
                yield 'Flash has been unlocked successfully!'
            else:
                # raise an error if not receiving an ack
                raise ProgramModeError("Error Occurred!")

        except ProgramModeError:
            yield f'The following error(s) occurred while writing unlocking the flash\n'
            num_errs = self.serial.read(1)
            errors = self.serial.read(toInt(num_errs))
            for err in errors:
                yield ERRORS[err] + '\n'

    def lockFlash(self):
        self.serial.write([COMMANDS['FLACH_LOCK']])
        #  check for acknowledgement by read the received byte
        ret = self.serial.read(1)
        try:
            if ret == ACK:
                yield 'Flash has been unlocked successfully!'
            else:
                # raise an error if not receiving an ack
                raise ProgramModeError("Error Occurred!")

        except ProgramModeError:
            yield f'The following error(s) occurred while writing locking the flash\n'
            num_errs = self.serial.read(1)
            errors = self.serial.read(toInt(num_errs))
            for err in errors:
                yield ERRORS[err] + '\n'


if __name__ == '__main__':

    com_port = 'COM' + input('Serial communication on COM: ')

    file_path = input('Hex File path: ')

    flasher = STM32Flasher(com_port)

    for msg in flasher.writeImage(file_path):
        print(msg, end='')
