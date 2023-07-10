// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License
// ============================================================================
// This is part of the Windows MIDI Services App API and should be used
// in your Windows application via an official binary distribution.
// Further information: https://github.com/microsoft/MIDI/
// ============================================================================

#pragma once

#include "pch.h"

#include "MidiUmp96.g.h"


namespace winrt::Windows::Devices::Midi2::implementation
{
    struct MidiUmp96 : MidiUmp96T<MidiUmp96>
    {
        MidiUmp96() = default;
        MidiUmp96(uint64_t timestamp, uint32_t word0, uint32_t word1, uint32_t word2);

        // internal
        MidiUmp96(PVOID data);


        uint32_t Word0() { return _ump->word0; }
        void Word0(uint32_t value) { _ump->word0 = value; }

        uint32_t Word1() { return _ump->word1; }
        void Word1(uint32_t value) { _ump->word1 = value; }

        uint32_t Word2() { return _ump->word2; }
        void Word2(uint32_t value) { _ump->word2 = value; }

        uint64_t Timestamp() { return _ump->timestamp; }
        void Timestamp(uint64_t value) { _ump->timestamp = value; }


        winrt::Windows::Devices::Midi2::MidiUmpMessageType MessageType() { return (winrt::Windows::Devices::Midi2::MidiUmpMessageType)(internal::GetUmpMessageTypeFromFirstWord(_ump->word0)); }
        winrt::Windows::Devices::Midi2::MidiUmpPacketType MidiUmpPacketType() { return winrt::Windows::Devices::Midi2::MidiUmpPacketType::Ump96; }

        winrt::Windows::Foundation::IMemoryBuffer RawData();

    private:
        intshared::PackedUmp96* _ump{};

    };
}
namespace winrt::Windows::Devices::Midi2::factory_implementation
{
    struct MidiUmp96 : MidiUmp96T<MidiUmp96, implementation::MidiUmp96>
    {
    };
}
