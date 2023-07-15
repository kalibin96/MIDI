﻿// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License
// Further information: https://github.com/microsoft/MIDI/
// ============================================================================

// Windows MIDI Services sample code

#include "pch.h"
#include <iostream>

using namespace winrt;
using namespace Microsoft::Devices::Midi2;
using namespace Windows::Devices::Midi2;

using namespace Windows::Devices::Enumeration;

int main()
{
    init_apartment();

    std::cout << "Checking for MIDI Services" << std::endl;

    // check for presence of compatible Windows MIDI Services
    auto checkResult = MidiServices::CheckForWindowsMidiServices();

    // proceed only if MIDI services is present and compatible. Your own application may decide
    // to fall back to WinRT/WinMM MIDI 1.0 APIs, or to prompt the user to install the latest 
    // version of Windows MIDI Services

    if (checkResult == WindowsMidiServicesCheckResult::PresentAndUsable)
    {
        std::cout << "MIDI Services Present and usable" << std::endl;

        // create the MIDI session, giving us access to Windows MIDI Services. An app may open 
        // more than one session. If so, the session name should be meaningful to the user, like
        // the name of a browser tab, or a project.

        std::cout << "Creating session settings." << std::endl;

        MidiSessionSettings sessionSettings = MidiSessionSettings::Default();

        std::cout << "Creating session." << std::endl;

        auto session = MidiSession::CreateSession(L"Sample Session", sessionSettings);

        // you can ask for MIDI 1.0 Byte Stream devices, MIDI 2.0 UMP devices, or both. Note that Some MIDI 2.0
        // endpoints may have MIDI 1.0 function blocks in them, so this is endpoint/device-level only.
        // Note that every device uses UMP through this API, but it can be helpful to know when a device is
        // a MIDI 1.0 device at the main interface level.

        std::cout << "Creating Device Selector." << std::endl;

        hstring deviceSelector = MidiEndpointConnection::GetDeviceSelector();

        // Enumerate UMP endpoints. Note that per C++, main cannot be a co-routine,
        // so we can't just co_await this async call, but instead use the C++/WinRT Extension "get()". 
        // We may end up wrapping this enumeration code into another class to instantly transform to 
        // MidiDeviceInformation instances, and to simplify calling code (reducing need for apps to
        // incorporate async handling).

        std::cout << "Enumerating through Windows::Devices::Enumeration." << std::endl;

        Windows::Foundation::IAsyncOperation<DeviceInformationCollection> op = DeviceInformation::FindAllAsync(deviceSelector);
        DeviceInformationCollection endpointDevices = op.get();

        if (endpointDevices.Size() > 0)
        {
            std::cout << "MIDI Endpoints were found (not really, but pretending they are for now)." << std::endl;

            // We're going to just pick the first one we find. Normally, you'd have the user pick from a list
            // or you'd otherwise have an Id at hand.
            DeviceInformation selectedEndpointInformation = endpointDevices.GetAt(0);


            // then you connect to the UMP endpoint
            std::cout << "Connecting to UMP Endpoint." << std::endl;
            std::cout << "Note: For this example to fully work, you need to the special Loopback MidiSrv installed." << std::endl;
            std::cout << "Otherwise, creating an endpoint will fail, and no messages will be sent or received." << std::endl;

            //auto endpoint = session.ConnectToEndpoint(selectedMidiEndpointInformation.Id(), MidiEndpointConnectOptions::Default());
            auto endpoint = session.ConnectBidirectionalEndpoint(L"foobarbaz", nullptr);

            // after connecting, you can send and receive messages to/from the endpoint. Sending and receiving is
            // performed one complete UMP at a time. 
            // -----------------------------
            // Wire up an event handler to receive the message. This event handler type receives an IMidiUmp type
            // but you can also wire up one which receives a uint32_t array and a uint64_t timestamp instead. The
            // performance is almost identical (within a couple ms total over 1000 send/receives despite the 
            // additional type activations and casting) but one may be more convenient than the other to you, so
            // both are provided. 

            auto MessageReceivedHandler = [](winrt::Windows::Foundation::IInspectable const& sender, MidiMessageReceivedEventArgs const& args)
                {
                    std::cout << std::endl;
                    std::cout << "Received UMP" << std::endl;
                    std::cout << "- Current Timestamp: " << std::dec << MidiClock::GetMidiTimestamp() << std::endl;
                    std::cout << "- UMP Timestamp: " << std::dec << args.Ump().Timestamp() << std::endl;
                    std::cout << "- UMP Type: " << std::hex << (uint32_t)args.Ump().MessageType() << std::endl;

                    // if you wish to cast the IMidiUmp to a specific Ump Type, you can do so using .as<T>.

                    if (args.Ump().MidiUmpPacketType() == MidiUmpPacketType::Ump32)
                    {
                        auto ump = args.Ump().as<MidiUmp32>();
                        std::cout << "Word 0: " << std::hex << ump.Word0() << std::endl;
                    }

                    std::cout << std::endl;

                };

            // the returned token is used to deregister the event later.
            auto eventRevokeToken = endpoint.MessageReceived(MessageReceivedHandler);

            MidiUmp32 ump32{};
            ump32.MessageType(MidiUmpMessageType::Midi1ChannelVoice32);
            auto ump = ump32.as<IMidiUmp>();
            endpoint.SendUmp(ump);

            std::cout << "Wait for the message to arrive, and then press any key to cleanup." << std::endl;
            system("pause");

            // deregister the event
            endpoint.MessageReceived(eventRevokeToken);

            // not strictly necessary as the session.Close() call will do it, but it's here in case you need it
            session.DisconnectEndpointConnection(endpoint.Id());
        }
        else
        {
            // no MIDI endpoints found. We'll just bail here
            std::cout << "No MIDI Endpoints were found." << std::endl;
        }

        // close the session, detaching all Windows MIDI Services resources and closing all connections
        // You can also disconnect individual Endpoint Connections when you are done with them
        session.Close();
    }
    else
    {
        if (checkResult == WindowsMidiServicesCheckResult::NotPresent)
        {
            std::cout << "MIDI Services Not Present" << std::endl;

            // allow the user to install the minimum required version
        }
        else if (checkResult == WindowsMidiServicesCheckResult::IncompatibleVersion)
        {
            std::cout << "MIDI Present, but is not a compatible version." << std::endl;
            std::cout << "Here you would prompt the user to install the latest version from " << winrt::to_string(MidiServices::LatestMidiServicesInstallUri().ToString()) << std::endl;

            // allow the user to install the minimum required version
        }
    }

}
