// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License
// ============================================================================
// This is part of the Windows MIDI Services App API and should be used
// in your Windows application via an official binary distribution.
// Further information: https://github.com/microsoft/MIDI/
// ============================================================================


#include "pch.h"
#include "midi2.VirtualMidiAbstraction.h"

using namespace wil;
using namespace Microsoft::WRL;
using namespace Microsoft::WRL::Wrappers;

#define MAX_DEVICE_ID_LEN 200 // size in chars

GUID AbstractionLayerGUID = __uuidof(Midi2VirtualMidiAbstraction);

_Use_decl_annotations_
HRESULT
CMidi2VirtualMidiEndpointManager::Initialize(
    IUnknown* midiDeviceManager, 
    LPCWSTR configurationJson
)
{
//    OutputDebugString(L"" __FUNCTION__ " Enter");

    TraceLoggingWrite(
        MidiVirtualMidiAbstractionTelemetryProvider::Provider(),
        __FUNCTION__,
        TraceLoggingLevel(WINEVENT_LEVEL_INFO),
        TraceLoggingPointer(this, "this")
    );

    RETURN_HR_IF(E_INVALIDARG, nullptr == midiDeviceManager);

    RETURN_IF_FAILED(midiDeviceManager->QueryInterface(__uuidof(IMidiDeviceManagerInterface), (void**)&m_MidiDeviceManager));

    m_transportAbstractionId = AbstractionLayerGUID;   // this is needed so MidiSrv can instantiate the correct transport
    m_containerId = m_transportAbstractionId;                           // we use the transport ID as the container ID for convenience

    RETURN_IF_FAILED(CreateParentDevice());

    if (configurationJson != nullptr)
    {
        try
        {
            std::wstring json{ configurationJson };

            if (!json.empty())
            {
                m_jsonObject = json::JsonObject::Parse(json);

                LOG_IF_FAILED(CreateConfiguredEndpoints(json));
            }
        }
        catch (...)
        {
            OutputDebugString(L"Exception processing json for virtual MIDI abstraction");

            // we return S_OK here because otherwise this prevents the service from starting up.
            return S_OK;
        }

    }
    else
    {
        // empty / null is fine. We just continue on.

        OutputDebugString(L"Configuration json is null for virtual MIDI abstraction");

        return S_OK;
    }

    return S_OK;
}




void SwMidiParentDeviceCreateCallback(__in HSWDEVICE /*hSwDevice*/, __in HRESULT CreationResult, __in_opt PVOID pContext, __in_opt PCWSTR pszDeviceInstanceId)
{
    if (pContext == nullptr)
    {
        // TODO: Should log this.

        return;
    }

    PPARENTDEVICECREATECONTEXT creationContext = (PPARENTDEVICECREATECONTEXT)pContext;


    // interface registration has started, assume failure
    creationContext->MidiParentDevice->SwDeviceState = SWDEVICESTATE::Failed;

    LOG_IF_FAILED(CreationResult);

    if (SUCCEEDED(CreationResult))
    {
        // success, mark the port as created
        creationContext->MidiParentDevice->SwDeviceState = SWDEVICESTATE::Created;

        // get the new device instance ID. This is usually modified from what we started with
        creationContext->MidiParentDevice->InstanceId = std::wstring(pszDeviceInstanceId);
    }
    else
    {
        OutputDebugString(L"" __FUNCTION__ " - CreationResult FAILURE");
    }

    // success or failure, signal we have completed.
    creationContext->creationCompleted.SetEvent();
}



HRESULT
CMidi2VirtualMidiEndpointManager::CreateParentDevice()
{
    // the parent device parameters are set by the transport (this)

    std::wstring parentDeviceName{ TRANSPORT_PARENT_DEVICE_NAME };
    std::wstring parentDeviceId{ TRANSPORT_PARENT_ID };

    SW_DEVICE_CREATE_INFO CreateInfo = {};
    CreateInfo.cbSize = sizeof(CreateInfo);
    CreateInfo.pszInstanceId = parentDeviceId.c_str();
    CreateInfo.CapabilityFlags = SWDeviceCapabilitiesNone;
    CreateInfo.pszDeviceDescription = parentDeviceName.c_str();

    SW_DEVICE_CREATE_INFO* createInfo = (SW_DEVICE_CREATE_INFO*)&CreateInfo;

    if (m_parentDevice != nullptr)
    {
        return S_OK;
    }

    m_parentDevice = std::make_unique<MidiEndpointParentDeviceInfo>();

    RETURN_IF_NULL_ALLOC(m_parentDevice);

    PARENTDEVICECREATECONTEXT creationContext;

    // lambdas can only be converted to a function pointer if they
    // don't do capture, so copy everything into the CREATECONTEXT
    // to share with the SwDeviceCreate callback.
    creationContext.MidiParentDevice = m_parentDevice.get();

    //creationContext.InterfaceDevProperties = (DEVPROPERTY*)InterfaceDevProperties;
    //creationContext.IntPropertyCount = IntPropertyCount;

    m_parentDevice->SwDeviceState = SWDEVICESTATE::CreatePending;

    //m_parentDevice->InstanceId = createInfo->pszInstanceId;

    DEVPROP_BOOLEAN devPropTrue = DEVPROP_TRUE;

    DEVPROPERTY deviceDevProperties[] = {
        {{DEVPKEY_Device_PresenceNotForDevice, DEVPROP_STORE_SYSTEM, nullptr},
            DEVPROP_TYPE_BOOLEAN, static_cast<ULONG>(sizeof(devPropTrue)), &devPropTrue},
        {{DEVPKEY_Device_NoConnectSound, DEVPROP_STORE_SYSTEM, nullptr},
            DEVPROP_TYPE_BOOLEAN, static_cast<ULONG>(sizeof(devPropTrue)),&devPropTrue}
    };


    std::wstring rootDeviceId = LOOPBACK_PARENT_ROOT;
    std::wstring enumeratorName = TRANSPORT_ENUMERATOR;

    createInfo->pContainerId = &m_containerId;

    RETURN_IF_FAILED(SwDeviceCreate(
        enumeratorName.c_str(),             // this really should come from the service
        rootDeviceId.c_str(),               // root device
        createInfo,
        ARRAYSIZE(deviceDevProperties),     // count of properties
        (DEVPROPERTY*)deviceDevProperties,  // pointer to properties
        SwMidiParentDeviceCreateCallback,   // callback
        &creationContext,
        wil::out_param(m_parentDevice->SwDevice)));

    // wait for creation to complete
    creationContext.creationCompleted.wait();

    // confirm we were able to register the interface
    RETURN_HR_IF(E_FAIL, m_parentDevice->SwDeviceState != SWDEVICESTATE::Created);

    return S_OK;
}


#define MIDI_VIRTUAL_ENDPOINTS_ARRAY_KEY L"endpoints"

#define MIDI_VIRTUAL_ENDPOINT_PROPERTY_KEY L"key"
#define MIDI_VIRTUAL_ENDPOINT_PROPERTY_NAME L"name"
#define MIDI_VIRTUAL_ENDPOINT_PROPERTY_UNIQUEID L"uniqueIdentifier"
#define MIDI_VIRTUAL_ENDPOINT_PROPERTY_MULTICLIENT L"supportsMulticlient"
#define MIDI_VIRTUAL_ENDPOINT_PROPERTY_DESCRIPTION L"description"
#define MIDI_VIRTUAL_ENDPOINT_PROPERTY_SMALLIMAGE L"smallImagePath"
#define MIDI_VIRTUAL_ENDPOINT_PROPERTY_LARGEIMAGE L"largeImagePath"




#define MIDI_JSON_ADD_NODE L"add"
#define MIDI_JSON_UPDATE_NODE L"update"
#define MIDI_JSON_REMOVE_NODE L"remove"


json::JsonObject GetAddNode(json::JsonObject parent)
{
    if (parent.HasKey(MIDI_JSON_ADD_NODE))
    {
        return parent.GetNamedObject(MIDI_JSON_ADD_NODE);
    }
    else
    {
        return nullptr;
    }
}

winrt::hstring GetStringValue(json::JsonObject parent, winrt::hstring key, winrt::hstring default)
{
    if (parent.HasKey(key))
    {
        return parent.GetNamedString(key);
    }
    else
    {
        return default;
    }
}

_Use_decl_annotations_
HRESULT
CMidi2VirtualMidiEndpointManager::CreateConfiguredEndpoints(std::wstring configurationJson)
{
    // if nothing to configure, that's ok
    if (configurationJson.empty()) return S_OK;

    try
    {
        auto jsonObject = json::JsonObject::Parse(configurationJson);


        // check to see if we have an "add" node. No point in checking for "update" or "remove" for initial configuration
        auto addNode = GetAddNode(jsonObject);

        if (addNode != nullptr && addNode.HasKey(MIDI_VIRTUAL_ENDPOINTS_ARRAY_KEY))
        {
            auto endpoints = addNode.GetNamedArray(MIDI_VIRTUAL_ENDPOINTS_ARRAY_KEY);

            for (auto endpointElement : endpoints)
            {
                // GetObjectW here is because wingdi redefines it to GetObject. It's a stupid preprocessor conflict
                try
                {
                    auto endpoint = endpointElement.GetObjectW();

                    //  auto key = endpoint.GetNamedString(MIDI_VIRTUAL_ENDPOINT_PROPERTY_KEY, L"");
                    auto name = endpoint.GetNamedString(MIDI_VIRTUAL_ENDPOINT_PROPERTY_NAME, L"");
                    auto uniqueIdentifier = endpoint.GetNamedString(MIDI_VIRTUAL_ENDPOINT_PROPERTY_UNIQUEID, L"");
                    //   auto supportsMultiClient = endpoint.GetNamedBoolean(MIDI_VIRTUAL_ENDPOINT_PROPERTY_MULTICLIENT, true);

                    auto instanceId = TRANSPORT_MNEMONIC L"_" + uniqueIdentifier;

                    auto description = GetStringValue(endpoint, MIDI_VIRTUAL_ENDPOINT_PROPERTY_DESCRIPTION, L"");
                    auto smallImage = GetStringValue(endpoint, MIDI_VIRTUAL_ENDPOINT_PROPERTY_SMALLIMAGE, L"");
                    auto largeImage = GetStringValue(endpoint, MIDI_VIRTUAL_ENDPOINT_PROPERTY_LARGEIMAGE, L"");

                    bool multiclient = true; // TODO
                    bool virtualResponder = false; // TODO


                    // TODO: Need to add this to the table for routing, and also add the other properties to the function
                    CreateEndpoint(
                        (std::wstring)instanceId,
                        (std::wstring)uniqueIdentifier,
                        multiclient,
                        virtualResponder,
                        (std::wstring)name,
                        (std::wstring)largeImage,
                        (std::wstring)smallImage,
                        (std::wstring)description
                    );
                }
                catch (...)
                {
                    // couldn't get an object. Garbage data
                    OutputDebugString(L"" __FUNCTION__ " Exception getting endpoint properties from json");

                    return E_FAIL;
                }
            }
        }
        else
        {
            // nothing to add
        }


    }
    catch (...)
    {
        // exception parsing the json. It's likely empty or malformed

        OutputDebugString(L"" __FUNCTION__ " Exception parsing JSON. Json string follows.");
        OutputDebugString(configurationJson.c_str());

        TraceLoggingWrite(
            MidiVirtualMidiAbstractionTelemetryProvider::Provider(),
            __FUNCTION__,
            TraceLoggingLevel(WINEVENT_LEVEL_ERROR),
            TraceLoggingPointer(this, "this")
        );

        return E_FAIL;
    }

    return S_OK;
}

// this will be called from the runtime endpoint creation interface

_Use_decl_annotations_
HRESULT 
CMidi2VirtualMidiEndpointManager::CreateEndpoint(
    std::wstring const instanceId,
    std::wstring const uniqueId,
    bool const multiclient,
    bool const isVirtualEndpointResponder,
    std::wstring const name,
    std::wstring const largeImagePath,
    std::wstring const smallImagePath,
    std::wstring const description
    )
{
    //put all of the devproperties we want into arrays and pass into ActivateEndpoint:

    std::wstring mnemonic(TRANSPORT_MNEMONIC);
    MidiFlow const flow = MidiFlow::MidiFlowBidirectional;

    DEVPROP_BOOLEAN devPropTrue = DEVPROP_TRUE;
    DEVPROP_BOOLEAN devPropFalse = DEVPROP_FALSE;

    DEVPROP_BOOLEAN devPropMulticlient = multiclient ? DEVPROP_TRUE : DEVPROP_FALSE;
    DEVPROP_BOOLEAN devPropVirtualResponder = isVirtualEndpointResponder ? DEVPROP_TRUE : DEVPROP_FALSE;


    BYTE nativeDataFormat = MIDI_PROP_NATIVEDATAFORMAT_UMP;

    DEVPROPERTY interfaceDevProperties[] = {
        {{DEVPKEY_DeviceInterface_FriendlyName, DEVPROP_STORE_SYSTEM, nullptr},
            DEVPROP_TYPE_STRING, static_cast<ULONG>((name.length() + 1) * sizeof(WCHAR)), (PVOID)name.c_str()},
        
        // essential to instantiate the right endpoint types
        {{PKEY_MIDI_AbstractionLayer, DEVPROP_STORE_SYSTEM, nullptr},
            DEVPROP_TYPE_GUID, static_cast<ULONG>(sizeof(GUID)), (PVOID)&AbstractionLayerGUID },        

        {{PKEY_MIDI_TransportMnemonic, DEVPROP_STORE_SYSTEM, nullptr},
            DEVPROP_TYPE_STRING, static_cast<ULONG>((mnemonic.length() + 1) * sizeof(WCHAR)), (PVOID)mnemonic.c_str()},

        {{PKEY_MIDI_NativeDataFormat, DEVPROP_STORE_SYSTEM, nullptr},
            DEVPROP_TYPE_BYTE, static_cast<ULONG>(sizeof(BYTE)), (PVOID)&nativeDataFormat},

        {{PKEY_MIDI_UniqueIdentifier, DEVPROP_STORE_SYSTEM, nullptr},
            DEVPROP_TYPE_STRING, static_cast<ULONG>((uniqueId.length() + 1) * sizeof(WCHAR)), (PVOID)uniqueId.c_str()},

        {{PKEY_MIDI_SupportsMulticlient, DEVPROP_STORE_SYSTEM, nullptr},
            DEVPROP_TYPE_BOOLEAN, static_cast<ULONG>(sizeof(devPropMulticlient)), (PVOID)&devPropMulticlient},

        {{PKEY_MIDI_TransportSuppliedEndpointName, DEVPROP_STORE_SYSTEM, nullptr},
            DEVPROP_TYPE_STRING, static_cast<ULONG>((name.length() + 1) * sizeof(WCHAR)), (PVOID)name.c_str()},

        {{PKEY_PKEY_MIDI_IsVirtualDeviceResponder, DEVPROP_STORE_SYSTEM, nullptr},
            DEVPROP_TYPE_BOOLEAN, static_cast<ULONG>(sizeof(devPropVirtualResponder)), (PVOID)&devPropVirtualResponder},


        {{PKEY_MIDI_UmpPing, DEVPROP_STORE_SYSTEM, nullptr},
            DEVPROP_TYPE_BOOLEAN, static_cast<ULONG>(sizeof(devPropFalse)), (PVOID)&devPropFalse},

        {{PKEY_MIDI_UmpLoopback, DEVPROP_STORE_SYSTEM, nullptr},
            DEVPROP_TYPE_BOOLEAN, static_cast<ULONG>(sizeof(devPropFalse)), (PVOID)&devPropFalse},


        {{PKEY_MIDI_UserSuppliedLargeImagePath, DEVPROP_STORE_SYSTEM, nullptr},
            DEVPROP_TYPE_STRING, static_cast<ULONG>((largeImagePath.length() + 1) * sizeof(WCHAR)), (PVOID)largeImagePath.c_str() },

        {{PKEY_MIDI_UserSuppliedSmallImagePath, DEVPROP_STORE_SYSTEM, nullptr},
            DEVPROP_TYPE_STRING, static_cast<ULONG>((smallImagePath.length() + 1) * sizeof(WCHAR)), (PVOID)smallImagePath.c_str() },

        {{PKEY_MIDI_UserSuppliedDescription, DEVPROP_STORE_SYSTEM, nullptr},
            DEVPROP_TYPE_STRING, static_cast<ULONG>((description.length() + 1) * sizeof(WCHAR)), (PVOID)description.c_str() },

        {{PKEY_MIDI_UserSuppliedEndpointName, DEVPROP_STORE_SYSTEM, nullptr},
            DEVPROP_TYPE_STRING, static_cast<ULONG>((name.length() + 1) * sizeof(WCHAR)), (PVOID)name.c_str() }
    };



    // Additional metadata properties added here to avoid having to add them in every transport
// and to ensure they are registered under the same identity that is creating the device
// Several of these are later updated through discovery, user-supplied json metadata, etc.
    //DEVPROP_BOOLEAN devPropFalse = DEVPROP_FALSE;
    //uint8_t zeroByte = 0;

    //std::wstring emptyString = L"";

    //interfaceProperties.push_back({ {PKEY_MIDI_SupportsMultiClient, DEVPROP_STORE_SYSTEM, nullptr },
    //    DEVPROP_TYPE_BOOLEAN, static_cast<ULONG>(sizeof(devPropTrue)), &devPropTrue });

    //// from endpoint discovery ============
    //interfaceProperties.push_back({ {PKEY_MIDI_EndpointSupportsMidi2Protocol, DEVPROP_STORE_SYSTEM, nullptr},
    //    DEVPROP_TYPE_BOOLEAN, static_cast<ULONG>(sizeof(devPropFalse)), &devPropFalse });

    //interfaceProperties.push_back({ {PKEY_MIDI_EndpointSupportsMidi1Protocol, DEVPROP_STORE_SYSTEM, nullptr},
    //    DEVPROP_TYPE_BOOLEAN, static_cast<ULONG>(sizeof(devPropFalse)), &devPropFalse });

    //interfaceProperties.push_back({ {PKEY_MIDI_EndpointSupportsReceivingJRTimestamps, DEVPROP_STORE_SYSTEM, nullptr},
    //    DEVPROP_TYPE_BOOLEAN, static_cast<ULONG>(sizeof(devPropFalse)), &devPropFalse });

    //interfaceProperties.push_back({ {PKEY_MIDI_EndpointSupportsSendingJRTimestamps, DEVPROP_STORE_SYSTEM, nullptr},
    //    DEVPROP_TYPE_BOOLEAN, static_cast<ULONG>(sizeof(devPropFalse)), &devPropFalse });

    //interfaceProperties.push_back({ {PKEY_MIDI_EndpointUmpVersionMajor, DEVPROP_STORE_SYSTEM, nullptr},
    //    DEVPROP_TYPE_BYTE, static_cast<ULONG>(sizeof(zeroByte)), &zeroByte });

    //interfaceProperties.push_back({ {PKEY_MIDI_EndpointUmpVersionMinor, DEVPROP_STORE_SYSTEM, nullptr},
    //    DEVPROP_TYPE_BYTE, static_cast<ULONG>(sizeof(zeroByte)), &zeroByte });

    //interfaceProperties.push_back({ {PKEY_MIDI_EndpointProvidedName, DEVPROP_STORE_SYSTEM, nullptr},
    //    DEVPROP_TYPE_STRING, static_cast<ULONG>((emptyString.length() + 1) * sizeof(WCHAR)), (PVOID)emptyString.c_str() });

    //interfaceProperties.push_back({ {PKEY_MIDI_EndpointProvidedProductInstanceId, DEVPROP_STORE_SYSTEM, nullptr},
    //    DEVPROP_TYPE_STRING, static_cast<ULONG>((emptyString.length() + 1) * sizeof(WCHAR)), (PVOID)emptyString.c_str() });

    //interfaceProperties.push_back({ {PKEY_MIDI_FunctionBlocksAreStatic, DEVPROP_STORE_SYSTEM, nullptr},
    //    DEVPROP_TYPE_BOOLEAN, static_cast<ULONG>(sizeof(devPropFalse)),&devPropFalse });

    ////interfaceProperties.push_back({ {PKEY_MIDI_DeviceIdentification, DEVPROP_STORE_SYSTEM, nullptr},
    ////    DEVPROP_TYPE_BINARY, static_cast<ULONG>(0),  });


    //// from function block discovery =============================

    ////interfaceProperties.push_back({ {PKEY_MIDI_FunctionBlocks, DEVPROP_STORE_SYSTEM, nullptr},
    ////    DEVPROP_TYPE_BINARY, static_cast<ULONG>(0), &functionBlockCount });




    DEVPROPERTY deviceDevProperties[] = {
        {{DEVPKEY_Device_PresenceNotForDevice, DEVPROP_STORE_SYSTEM, nullptr},
            DEVPROP_TYPE_BOOLEAN, static_cast<ULONG>(sizeof(devPropTrue)), &devPropTrue},
        {{DEVPKEY_Device_NoConnectSound, DEVPROP_STORE_SYSTEM, nullptr},
            DEVPROP_TYPE_BOOLEAN, static_cast<ULONG>(sizeof(devPropTrue)),&devPropTrue}
    };

    SW_DEVICE_CREATE_INFO createInfo = {};
    createInfo.cbSize = sizeof(createInfo);

    createInfo.pszInstanceId = instanceId.c_str();
    createInfo.CapabilityFlags = SWDeviceCapabilitiesNone;
    createInfo.pszDeviceDescription = name.c_str();


    const ULONG deviceInterfaceIdMaxSize = 255;
    wchar_t newDeviceInterfaceId[deviceInterfaceIdMaxSize]{ 0 };

    RETURN_IF_FAILED(m_MidiDeviceManager->ActivateEndpoint(
        std::wstring(m_parentDevice->InstanceId).c_str(),       // parent instance Id
        true,                                                   // UMP-only
        flow,                                                   // MIDI Flow
        ARRAYSIZE(interfaceDevProperties),
        ARRAYSIZE(deviceDevProperties),
        (PVOID)interfaceDevProperties,
        (PVOID)deviceDevProperties,
        (PVOID)&createInfo,
        (LPWSTR)&newDeviceInterfaceId,
        deviceInterfaceIdMaxSize));

    OutputDebugString(__FUNCTION__ L": Created device interface id:");
    OutputDebugString(newDeviceInterfaceId);

    // store the created device in the device table, along with the interface id



    return S_OK;
}



HRESULT
CMidi2VirtualMidiEndpointManager::Cleanup()
{
    OutputDebugString(L"" __FUNCTION__ " Enter");

    TraceLoggingWrite(
        MidiVirtualMidiAbstractionTelemetryProvider::Provider(),
        __FUNCTION__,
        TraceLoggingLevel(WINEVENT_LEVEL_INFO),
        TraceLoggingPointer(this, "this")
    );


    return S_OK;
}


//
//_Use_decl_annotations_
//HRESULT
//CMidi2VirtualMidiEndpointManager::ApplyConfiguration(
//    LPCWSTR /*configurationJson*/,
//    LPWSTR /*resultJson*/
//)
//{
//    return E_NOTIMPL;
//}


