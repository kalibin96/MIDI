// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License
// ============================================================================
// This is part of the Windows MIDI Services App API and should be used
// in your Windows application via an official binary distribution.
// Further information: https://github.com/microsoft/MIDI/
// ============================================================================

#pragma once
#include "MidiServiceLoopbackEndpointDefinition.g.h"

namespace winrt::Windows::Devices::Midi2::implementation
{
    struct MidiServiceLoopbackEndpointDefinition : MidiServiceLoopbackEndpointDefinitionT<MidiServiceLoopbackEndpointDefinition>
    {
        MidiServiceLoopbackEndpointDefinition() = default;

        winrt::hstring Name() const { return m_name; }
        void Name(winrt::hstring const& value) { m_name = internal::TrimmedHStringCopy(value); }

        winrt::hstring UniqueId() const { return m_uniqueId; }
        void UniqueId(winrt::hstring const& value) { m_uniqueId = internal::TrimmedHStringCopy(value); }

        winrt::hstring Description() const { return m_description; }
        void Description(winrt::hstring const& value) { m_description = internal::TrimmedHStringCopy(value); }

    private:
        winrt::hstring m_name{};
        winrt::hstring m_uniqueId{};
        winrt::hstring m_description{};
    };
}
namespace winrt::Windows::Devices::Midi2::factory_implementation
{
    struct MidiServiceLoopbackEndpointDefinition : MidiServiceLoopbackEndpointDefinitionT<MidiServiceLoopbackEndpointDefinition, implementation::MidiServiceLoopbackEndpointDefinition>
    {
    };
}
