// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License
// ============================================================================
// This is part of the Windows MIDI Services App API and should be used
// in your Windows application via an official binary distribution.
// Further information: https://github.com/microsoft/MIDI/
// ============================================================================

#pragma once
#include "MidiGroupTerminalBlock.g.h"


namespace winrt::Windows::Devices::Midi2::implementation
{
    struct MidiGroupTerminalBlock : MidiGroupTerminalBlockT<MidiGroupTerminalBlock>
    {
        MidiGroupTerminalBlock() = default;

        uint8_t Number() { return m_number; }
        winrt::hstring Name() { return m_name; }
        midi2::MidiGroupTerminalBlockDirection Direction() { return m_direction; }
        midi2::MidiGroupTerminalBlockProtocol Protocol() { return m_protocol; }

        uint8_t FirstGroupIndex() { return m_firstGroupIndex; }
        uint8_t NumberOfGroupsSpanned() { return m_numberOfGroupsSpanned; }
        bool IncludesGroup(_In_ midi2::MidiGroup const& group) { return group.Index() >= FirstGroupIndex() && group.Index() < FirstGroupIndex() + NumberOfGroupsSpanned(); }


        uint16_t MaxDeviceInputBandwidthIn4KBSecondUnits() { return m_maxDeviceInputBandwidthIn4KBSecondUnits; }
        uint16_t MaxDeviceOutputBandwidthIn4KBSecondUnits() { return m_maxDeviceOutputBandwidthIn4KBSecondUnits; }

    private:
        uint8_t m_number{};
        winrt::hstring m_name{};
        midi2::MidiGroupTerminalBlockDirection m_direction{ MidiGroupTerminalBlockDirection::Bidirectional };
        midi2::MidiGroupTerminalBlockProtocol m_protocol{ 0 };
        uint16_t m_maxDeviceInputBandwidthIn4KBSecondUnits{ 0 };
        uint16_t m_maxDeviceOutputBandwidthIn4KBSecondUnits{ 0 };

        uint8_t m_firstGroupIndex{ 0 };
        uint8_t m_numberOfGroupsSpanned{ 0 };

    };
}
//namespace winrt::Windows::Devices::Midi2::factory_implementation
//{
//    struct MidiGroupTerminalBlock : MidiGroupTerminalBlockT<MidiGroupTerminalBlock, implementation::MidiGroupTerminalBlock>
//    {
//    };
//}
