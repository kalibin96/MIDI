// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License
// ============================================================================
// This is part of the Windows MIDI Services App API and should be used
// in your Windows application via an official binary distribution.
// Further information: https://github.com/microsoft/MIDI/
// ============================================================================

#include "pch.h"

_Use_decl_annotations_
HRESULT
MidiPingBidiDevice::SendMidiMessage
(
    void* Message,
    UINT32 Size,
    LONGLONG /*timestamp*/
)
{
    RETURN_HR_IF_NULL(E_POINTER, m_Callback);
    RETURN_HR_IF_NULL(E_INVALIDARG, Message);
    RETURN_HR_IF(E_INVALIDARG, Size != sizeof(Windows::Devices::Midi2::Internal::PackedPingRequestUmp));
   
    auto requestUmp = (Windows::Devices::Midi2::Internal::PackedPingRequestUmp*)Message;

    RETURN_HR_IF(E_INVALIDARG, requestUmp->Word0 != INTERNAL_PING_REQUEST_UMP_WORD0);

    // build response message
    Windows::Devices::Midi2::Internal::PackedPingResponseUmp responseUmp;
    responseUmp.Word0 = INTERNAL_PING_RESPONSE_UMP_WORD0;
    responseUmp.PingId = requestUmp->PingId;

    auto newTimestamp = Windows::Devices::Midi2::Internal::Shared::GetCurrentMidiTimestamp();

    return m_Callback->Callback(&responseUmp, sizeof(Windows::Devices::Midi2::Internal::PackedPingResponseUmp), newTimestamp, m_Context);
}

_Use_decl_annotations_
void
MidiPingBidiDevice::SetCallback
(
    IMidiCallback* Callback,
    LONGLONG Context
)
{
    m_Callback = Callback;
    m_Context = Context;
}

void
MidiPingBidiDevice::Cleanup()
{
    m_Callback = nullptr;
    m_Context = 0;
}

MidiPingBidiDevice::MidiPingBidiDevice() = default;

MidiPingBidiDevice::~MidiPingBidiDevice()
{
    Cleanup();
}
