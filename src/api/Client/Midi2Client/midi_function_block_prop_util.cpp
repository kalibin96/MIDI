// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License
// ============================================================================
// This is part of the Windows MIDI Services App API and should be used
// in your Windows application via an official binary distribution.
// Further information: https://github.com/microsoft/MIDI/
// ============================================================================

#include "pch.h"

namespace Windows::Devices::Midi2::Internal
{
    _Use_decl_annotations_
    winrt::hstring BuildFunctionBlockPropertyKey(uint8_t functionBlockNumber)
    {
        return winrt::hstring(MIDI_STRING_PKEY_GUID) + winrt::hstring(MIDI_STRING_PKEY_PID_SEPARATOR) + winrt::to_hstring(functionBlockNumber + MIDI_FUNCTION_BLOCK_PROPERTY_INDEX_START);
    }

    _Use_decl_annotations_
    winrt::hstring BuildFunctionBlockNamePropertyKey(uint8_t functionBlockNumber)
    {
        return winrt::hstring(MIDI_STRING_PKEY_GUID) + winrt::hstring(MIDI_STRING_PKEY_PID_SEPARATOR) + winrt::to_hstring(functionBlockNumber + MIDI_FUNCTION_BLOCK_NAME_PROPERTY_INDEX_START);
    }

    _Use_decl_annotations_
    bool PropertyMapContainsAnyFunctionBlockProperty(collections::IMapView<winrt::hstring, foundation::IInspectable> map)
    {
        for (uint8_t fb = 0; fb < MIDI_MAX_FUNCTION_BLOCKS; fb++)
        {
            winrt::hstring functionBlockProperty = internal::BuildFunctionBlockPropertyKey(fb);
            winrt::hstring functionBlockNameProperty = internal::BuildFunctionBlockNamePropertyKey(fb);

            if (map.HasKey(functionBlockProperty) || map.HasKey(functionBlockNameProperty))
            {
                return true;
            }
        }

        return false;
    }


}