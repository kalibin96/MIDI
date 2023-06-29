#pragma once
#include "MidiMessageReader.g.h"


namespace winrt::Microsoft::Devices::Midi2::implementation
{
    struct MidiMessageReader : MidiMessageReaderT<MidiMessageReader>
    {
        MidiMessageReader() = default;

        winrt::Microsoft::Devices::Midi2::MidiMessageReaderEndOfMessagesBehavior EndOfMessagesBehavior();
        void EndOfMessagesBehavior(winrt::Microsoft::Devices::Midi2::MidiMessageReaderEndOfMessagesBehavior const& value);
        bool EndOfMessages();
        uint64_t PeekNextTimestamp();
        winrt::Microsoft::Devices::Midi2::MidiMessageType PeekNextUmpMessageType();
        winrt::Microsoft::Devices::Midi2::MidiUmpSize PeekNextUmpSize();
        winrt::Microsoft::Devices::Midi2::MidiUmpWithTimestamp PeekNextMessage();
        winrt::Microsoft::Devices::Midi2::MidiUmpWithTimestampList ReadToEnd();
        winrt::Microsoft::Devices::Midi2::MidiUmpWithTimestamp ReadNextMessage();
        winrt::Microsoft::Devices::Midi2::MidiUmp32 ReadUmp32();
        winrt::Microsoft::Devices::Midi2::MidiUmp64 ReadUmp64();
        winrt::Microsoft::Devices::Midi2::MidiUmp96 ReadUmp96();
        winrt::Microsoft::Devices::Midi2::MidiUmp128 ReadUmp128();
        uint32_t ReadTimestampedUmpStructsIntoBuffer(winrt::Windows::Foundation::IMemoryBufferReference const& buffer, uint32_t byteOffsetinBuffer, uint32_t maxBytesToRead);
    };
}
