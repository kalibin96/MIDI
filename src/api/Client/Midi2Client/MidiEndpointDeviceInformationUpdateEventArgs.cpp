// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License
// ============================================================================
// This is part of the Windows MIDI Services App API and should be used
// in your Windows application via an official binary distribution.
// Further information: https://github.com/microsoft/MIDI/
// ============================================================================

#include "pch.h"
#include "MidiEndpointDeviceInformationUpdateEventArgs.h"
#include "MidiEndpointDeviceInformationUpdateEventArgs.g.cpp"


namespace winrt::Windows::Devices::Midi2::implementation
{
    _Use_decl_annotations_
    void MidiEndpointDeviceInformationUpdateEventArgs::InternalInitialize(
        winrt::hstring endpointDeviceId,
        winrt::Windows::Devices::Enumeration::DeviceInformationUpdate deviceInformationUpdate,
        bool updatedName,
        bool updatedInProtocolEndpointInformation,
        bool updatedDeviceIdentity,
        bool updatedStreamConfiguration,
        bool updatedFunctionBlocks,
        bool updatedUserMetadata,
        bool updatedAdditionalCapabilities
    )
    {

        m_updatedName = updatedName;
        m_updatedInProtocolEndpointInformation = updatedInProtocolEndpointInformation;
        m_updatedDeviceIdentity = updatedDeviceIdentity;
        m_updatedStreamConfiguration = updatedStreamConfiguration;
        m_updatedFunctionBlocks = updatedFunctionBlocks;
        m_updatedUserMetadata = updatedUserMetadata;
        m_updatedAdditionalCapabilities = updatedAdditionalCapabilities;

        m_endpointDeviceId = endpointDeviceId;
        m_deviceInformationUpdate = deviceInformationUpdate;
    }

}
