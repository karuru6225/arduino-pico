/*
    KeyboardBLE.cpp

    Modified by Earle F. Philhower, III <earlephilhower@yahoo.com>
    Main Arduino Library Copyright (c) 2015, Arduino LLC
    Original code (pre-library): Copyright (c) 2011, Peter Barrett

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "KeyboardBLE.h"
#include <HID_Bluetooth.h>
#include <PicoBluetoothBLEHID.h>

//================================================================================
//================================================================================
//  Keyboard

// Weak function override to add our descriptor to the list
void __BLEInstallKeyboard() { /* noop */ }

KeyboardBLE_::KeyboardBLE_(void) {
    // Base class clears the members we care about
}

uint8_t *desc_keyboardBLE;
uint16_t desc_keyboardBLE_length;

void KeyboardBLE_::begin(const char *localName, const char *hidName) {
    if (!localName) {
        localName = "PicoW BLE Keyboard";
    }
    if (!hidName) {
        hidName = localName;
    }
    __SetupHIDreportmap(__BLEInstallMouse, __BLEInstallKeyboard, __BLEInstallJoystick, false, &desc_keyboardBLE_length, &desc_keyboardBLE);

    PicoBluetoothBLEHID.startHID(localName, hidName, __BLEGetAppearance(), desc_keyboardBLE, desc_keyboardBLE_length);
}

void KeyboardBLE_::end(void) {
    PicoBluetoothBLEHID.end();
}

void KeyboardBLE_::setBattery(int lvl) {
    PicoBluetoothBLEHID.setBattery(lvl);
}

int KeyboardBLE_::send(void) {
    Serial.println("KeyboardBLE_::send");
    hid_keyboard_report_t data;
    data.modifier = _keyReport.modifiers;
    data.reserved = 0;
    memcpy(data.keycode, _keyReport.keys, sizeof(data.keycode));

    static uint8_t report[sizeof(hid_keyboard_report_t) +1];
    report[0] = __BLEGetKeyboardReportID();
    memcpy(&report[1], (uint8_t*)&data, sizeof(hid_keyboard_report_t));
    if (PicoBluetoothBLEHID.send(&report, sizeof(hid_keyboard_report_t) +1)) {
        return 1;
    }
    return 0;
}

// void KeyboardBLE_::sendConsumerReport(uint16_t key) {
//     uint8_t report[3];

//     report[0] = __BLEGetKeyboardReportID() + 1; //consumer report id
//     report[1] = key & 0xFF;
//     report[2] = (key >> 8) & 0xFF;
//     PicoBluetoothBLEHID.send(&report, 3);
// }

KeyboardBLE_ KeyboardBLE;
