/*
 * Copyright (c) 2020 ndeadly
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once
#include "../bluetooth/bluetooth_types.hpp"
#include "../bluetooth/bluetooth_hid_report.hpp"

namespace ams::controller {

    constexpr auto UINT12_MAX  = 0xfff;
    constexpr auto STICK_ZERO  = 0x800;
    constexpr auto BATTERY_MAX = 8;

    struct HardwareID {
        uint16_t vid;
        uint16_t pid;
    };
        
    struct SwitchStickData {
        uint8_t xy[3];
    } __attribute__ ((__packed__));

    struct SwitchButtonData {
        uint8_t Y              : 1;
        uint8_t X              : 1;
        uint8_t B              : 1;
        uint8_t A              : 1;
        uint8_t                : 2; // SR, SL (Right Joy)
        uint8_t R              : 1;
        uint8_t ZR             : 1;

        uint8_t minus          : 1;
        uint8_t plus           : 1;
        uint8_t rstick_press   : 1;
        uint8_t lstick_press   : 1;
        uint8_t home           : 1;
        uint8_t capture        : 1;
        uint8_t                : 0;

        uint8_t dpad_down      : 1;
        uint8_t dpad_up        : 1;
        uint8_t dpad_right     : 1;
        uint8_t dpad_left      : 1;
        uint8_t                : 2; // SR, SL (Left Joy)
        uint8_t L              : 1;
        uint8_t ZL             : 1;
    } __attribute__ ((__packed__));

    struct Switch6AxisData {
        uint16_t    accel_x;
        uint16_t    accel_y;
        uint16_t    accel_z;
        uint16_t    gyro_1;
        uint16_t    gyro_2;
        uint16_t    gyro_3;
    } __attribute__ ((__packed__));

    struct SwitchOutputReport0x01;
    struct SwitchOutputReport0x03;
    struct SwitchOutputReport0x10;
    struct SwitchOutputReport0x11;
    struct SwitchOutputReport0x12;

    struct SwitchInputReport0x21 {
        uint8_t             timer;
        uint8_t             conn_info      : 4;
        uint8_t             battery        : 4;
        SwitchButtonData    buttons;
        SwitchStickData     left_stick;
        SwitchStickData     right_stick;
        uint8_t             vibrator;
        
        struct {
            uint8_t         ack;
            uint8_t         id;
            uint8_t         reply;
            uint8_t         data[0x22];
        } subcmd;
    } __attribute__ ((__packed__));

    struct SwitchInputReport0x23;

    struct SwitchInputReport0x30 {
        uint8_t             timer;
        uint8_t             conn_info      : 4;
        uint8_t             battery        : 4;
        SwitchButtonData    buttons;
        SwitchStickData     left_stick;
        SwitchStickData     right_stick;
        uint8_t             vibrator;

        Switch6AxisData     imu_0ms;
        Switch6AxisData     imu_5ms;
        Switch6AxisData     imu_10ms;
    } __attribute__ ((__packed__));

    struct SwitchInputReport0x31;
    struct SwitchInputReport0x32;
    struct SwitchInputReport0x33;
    struct SwitchInputReport0x3f;

    struct SwitchReportData {
        uint8_t id;
        union {
            //SwitchOutputReport0x01 output0x01;
            //SwitchOutputReport0x03 output0x03;
            //SwitchOutputReport0x10 output0x10;
            //SwitchOutputReport0x11 output0x11;
            //SwitchOutputReport0x12 output0x12;
            SwitchInputReport0x21  input0x21;
            SwitchInputReport0x30  input0x30;
            //SwitchInputReport0x31  input0x31;
            //SwitchInputReport0x32  input0x32;
            //SwitchInputReport0x33  input0x33;
            //SwitchInputReport0x3f  input0x3f;

        };
    } __attribute__ ((__packed__));

    class SwitchController {

        public: 
            static constexpr const HardwareID hardware_ids[] = { 
                {0x057e, 0x2006},   // Official Joycon(L) Controller
                {0x057e, 0x2007},   // Official Joycon(R) Controller
                {0x057e, 0x2009}    // Official Switch Pro Controller
            };

            SwitchController(const bluetooth::Address *address)
                : m_address(*address) { };

            const bluetooth::Address& Address(void) const { return m_address; };

            virtual Result Initialize(void) { return ams::ResultSuccess(); };
            virtual Result HandleIncomingReport(const bluetooth::HidReport *report);
            virtual Result HandleOutgoingReport(const bluetooth::HidReport *report);

        protected:
            bluetooth::Address m_address;
    };

}
