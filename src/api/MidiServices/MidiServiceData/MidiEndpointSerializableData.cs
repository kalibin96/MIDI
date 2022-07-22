﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MidiService.Data
{
    public struct MidiEndpointSerializableData
    {
        public Guid Id;
        public Guid ParentDeviceId;
        public string Name;
        public string DeviceSuppliedName;
        public string IconFileName;
        public string Description;
    }
}
