// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License
// ============================================================================
// This is part of the Windows MIDI Services App API and should be used
// in your Windows application via an official binary distribution.
// Further information: https://github.com/microsoft/MIDI/
// ============================================================================

#include "pch.h"
#include "MidiMessage128.h"
#include "MidiMessage128.g.cpp"

namespace winrt::Windows::Devices::Midi2::implementation
{
    _Use_decl_annotations_
    MidiMessage128::MidiMessage128(
        internal::MidiTimestamp const timestamp, 
        uint32_t const word0, 
        uint32_t const word1, 
        uint32_t const word2, 
        uint32_t const word3)
    {
        m_timestamp = timestamp;

        m_ump.word0 = word0;
        m_ump.word1 = word1;
        m_ump.word2 = word2;
        m_ump.word3 = word3;
    }

    // internal constructor for reading from the service callback
    _Use_decl_annotations_
    void MidiMessage128::InternalInitializeFromPointer(
        internal::MidiTimestamp timestamp, 
        PVOID data)
    {
        if (data == nullptr) return;

        m_timestamp = timestamp;

        // need to have some safeties around this
        memcpy((void*)&m_ump, data, sizeof(internal::PackedUmp128));
    }



    winrt::hstring MidiMessage128::ToString()
    {
        std::stringstream stream;

        stream << "128-bit MIDI message:"
            << " 0x" << std::hex << std::setw(8) << std::setfill('0') << Word0()
            << " 0x" << std::hex << std::setw(8) << std::setfill('0') << Word1()
            << " 0x" << std::hex << std::setw(8) << std::setfill('0') << Word2()
            << " 0x" << std::hex << std::setw(8) << std::setfill('0') << Word3();

        return winrt::to_hstring(stream.str());
    }


}