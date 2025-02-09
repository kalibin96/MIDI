// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License
// ============================================================================
// This is part of the Windows MIDI Services App API and should be used
// in your Windows application via an official binary distribution.
// Further information: https://github.com/microsoft/MIDI/
// ============================================================================

#pragma once
#include "MidiUniqueId.g.h"

#define MIDI_MUID_BROADCAST         (uint32_t)0x0FFFFFFF

#define MIDI_MUID_RESERVED_START    (uint32_t)0x0FFFFF00
#define MIDI_MUID_RESERVED_END      (uint32_t)0x0FFFFFFE

#define MIDI_MUID_MIN_VALUE         (uint32_t)0x00000000
#define MIDI_MUID_MAX_VALUE         (uint32_t)0x0FFFFFFF


namespace winrt::Windows::Devices::Midi2::implementation
{
    struct MidiUniqueId : MidiUniqueIdT<MidiUniqueId>
    {
        MidiUniqueId() = default;

        MidiUniqueId(
            _In_ uint8_t const sevenBitByte1, 
            _In_ uint8_t const sevenBitByte2,
            _In_ uint8_t const sevenBitByte3,
            _In_ uint8_t const sevenBitByte4
        ) noexcept
        {
            Byte1(sevenBitByte1);
            Byte2(sevenBitByte2);
            Byte3(sevenBitByte3);
            Byte4(sevenBitByte4);
        }
        
        MidiUniqueId(_In_ uint32_t const integer28bit) noexcept;


        static midi2::MidiUniqueId CreateBroadcast();
        static midi2::MidiUniqueId CreateRandom();

        uint8_t Byte1() const noexcept { return m_byte1; }
        void Byte1(_In_ uint8_t value) noexcept { m_byte1 = value & 0x7F; }

        uint8_t Byte2() const noexcept { return m_byte2; }
        void Byte2(_In_ uint8_t value) noexcept { m_byte2 = value & 0x7F; }

        uint8_t Byte3() const noexcept { return m_byte3; }
        void Byte3(_In_ uint8_t value) noexcept { m_byte3 = value & 0x7F; }

        uint8_t Byte4() const noexcept { return m_byte4; }
        void Byte4(_In_ uint8_t value) noexcept { m_byte4 = value & 0x7F; }

        static winrt::hstring LabelShort() { return internal::ResourceManager::GetHString(IDS_MIDI_COMMON_LABEL_MUID_SHORT); }
        static winrt::hstring LabelFull() { return internal::ResourceManager::GetHString(IDS_MIDI_COMMON_LABEL_MUID_FULL); }
        
        uint32_t As28BitInteger() const noexcept;

        bool IsBroadcast() noexcept { return (As28BitInteger() == MIDI_MUID_BROADCAST); }
        bool IsReserved() noexcept { return As28BitInteger() >= MIDI_MUID_RESERVED_START && As28BitInteger() <= MIDI_MUID_RESERVED_END; }

    private:
        uint8_t m_byte1{};    // byte 1 is LSB
        uint8_t m_byte2{};
        uint8_t m_byte3{};
        uint8_t m_byte4{};
    };
}
namespace winrt::Windows::Devices::Midi2::factory_implementation
{
    struct MidiUniqueId : MidiUniqueIdT<MidiUniqueId, implementation::MidiUniqueId>
    {
    };
}
