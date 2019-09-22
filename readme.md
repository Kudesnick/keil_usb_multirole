# usb multirole device

## Preamble

The project was created to teach working with libraries included in Keil IDE packages. Ultimately, the project should have the following functionality:
* Work in the USB device mode. A device is defined as multiple HID devices and two mass storage devices and virtual com port device simultaneously.
* Work in the USB host mode. The device detects the connection of a mass storage device or the HID device (mouse, keyboard, joystick, etc.).
* The device can work with two SD cards connected via one SPI interface.
* I haven’t come up with anything else.

## Hardware

I used the board "blue pill" (stm32f103c8) first, but you can't build a usb host on it. I'm using a board "nucleo-F413ZH" (stm32f413zh) now.

## Software

I'm using Keil IDE v5.28

## to do

* Convert to RTX5 API v2 (complete)
* Add double MMC driver
* Add mass storage usb device
* Add autodetect otg mode (complete)
* Add usb host to keyboard (complete)
* Add usb host to flash memory (complete)
* Add SPI display
* Add wheel-button
* Refactoring for standarting of thread settings and separate functions of usb devices (complete)
* Migrate to c++11
* Add autodetect connection to PC or OTG cable (complete)

## known bugs
* if I call USBH_HID_GetMouseState and USBH_HID_GetKeyboardKey in one thread, then USBH_HID_GetKeyboardKey returned error code always. May be I must to read USBH_HID_ParseReportDescriptor and detect device type?

## license

I plan to extend the GNU GPLv3 license to my code, but at this stage I do not see the great value of the project for others. If you think otherwise, or are ready to help with the design of the code in accordance with the GNU GPLv3, write to me at kudesnick@inbox.ru or leave pull request.
At the moment, the code may contain proprietary fragments that do not allow the use of free licenses. I'm working on it.