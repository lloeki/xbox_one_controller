#include <IOKit/IOLib.h>

#include "xbox_one_controller.h"
#include "xbox_one_controller_packet.h"
#include "xbox_one_controller_report_descriptor.h"

// This required macro defines the class's constructors, destructors,
// and several other methods I/O Kit requires.
OSDefineMetaClassAndStructors(com_lloeki_xbox_one_controller, IOHIDDevice)


/********************************************************************************************************/
// driver lifecycle

// Define the driver's superclass.
#define super IOHIDDevice
bool com_lloeki_xbox_one_controller::init(OSDictionary *dict)
{
    bool result = super::init(dict);
    IOLog("[xbox_one_controller] Initializing\n");
    lock = IOLockAlloc();
    return result;
}

void com_lloeki_xbox_one_controller::free(void)
{
    IOLog("[xbox_one_controller] Freeing\n");
    IOLockFree(lock);
    super::free();
}

IOService *com_lloeki_xbox_one_controller::probe(IOService *provider,
                                                 SInt32 *score)
{
    IOService *result = super::probe(provider, score);
    IOLog("[xbox_one_controller] Probing\n");
    return result;
}

bool com_lloeki_xbox_one_controller::start(IOService *provider)
{
    bool result = super::start(provider);
    IOLog("[xbox_one_controller] Starting\n");

    device = findAndOpenDevice(provider);
    if (device == NULL) {
        IOLog("[xbox_one_controller] Failed to open device\n");
        closeAll();
        return false;
    }

    interface = findAndOpenInterface(device);
    if (interface == NULL) {
        IOLog("[xbox_one_controller] Failed to open interface\n");
        closeAll();
        return false;
    }

    pipeIn = findAndOpenPipe(interface, kUSBIn);
    if (pipeIn == NULL) {
        IOLog("[xbox_one_controller] Failed to open pipe in\n");
        closeAll();
        return false;
    }

    pipeOut = findAndOpenPipe(interface, kUSBOut);
    if (pipeOut == NULL) {
        IOLog("[xbox_one_controller] Failed to open pipe out\n");
        closeAll();
        return false;
    }

    if (!readInit(pipeIn)) {
        IOLog("[xbox_one_controller] Failed to queue first read\n");
    }

    if (!writeInit(pipeOut)) {
        IOLog("[xbox_one_controller] Failed to write init\n");
    }

    IOLog("[xbox_one_controller] Started\n");

    return result;
}

void com_lloeki_xbox_one_controller::stop(IOService *provider)
{
    IOLog("[xbox_one_controller] Stopping\n");
    closeAll();

    IOLog("[xbox_one_controller] Stopped\n");
    super::stop(provider);
}


/********************************************************************************************************/
// USB device access

IOUSBDevice *com_lloeki_xbox_one_controller::findAndOpenDevice(IOService *provider) {
    const IOUSBConfigurationDescriptor *descriptor;
    IOUSBDevice *device = OSDynamicCast(IOUSBDevice, provider);

    if (device->GetNumConfigurations() < 1) { return NULL; }
    if ((descriptor = device->GetFullConfigurationDescriptor(0)) == NULL) { return NULL; }
    if (!device->open(this)) { return NULL; }
    if (device->SetConfiguration(this, descriptor->bConfigurationValue, true) != kIOReturnSuccess) { return NULL; }
    IOLog("[xbox_one_controller] device @%08x opened: config=%d\n",
          device->GetLocationID(),
          descriptor->bConfigurationValue);

    return device;
}

IOUSBInterface *com_lloeki_xbox_one_controller::findAndOpenInterface(IOUSBDevice *device) {
    IOUSBFindInterfaceRequest ifRequest;
    IOUSBInterface *interface;

    ifRequest.bInterfaceClass    = 0x00FF;
    ifRequest.bInterfaceSubClass = 0x0047;
    ifRequest.bInterfaceProtocol = 0x00D0;
    ifRequest.bAlternateSetting  = kIOUSBFindInterfaceDontCare;

    interface = device->FindNextInterface(NULL, &ifRequest);
    if (interface == NULL) { return NULL; }
    if (!interface->open(this)) { return NULL; }
    IOLog("[xbox_one_controller] interface #%d opened\n",
          interface->GetInterfaceNumber());

    return interface;
}

IOUSBPipe *com_lloeki_xbox_one_controller::findAndOpenPipe(IOUSBInterface *interface, int direction) {
    IOUSBFindEndpointRequest pipeRequest;
    IOUSBPipe *pipe;

    pipeRequest.direction = direction;
    pipeRequest.type = kUSBInterrupt;
    pipeRequest.interval = 0;
    pipeRequest.maxPacketSize = 0;
    pipe = interface->FindNextPipe(NULL, &pipeRequest);
    if (pipe == NULL) { return NULL; }

    IOLog("[xbox_one_controller] pipe #%d opened: type=%d direction=%d, packet_size=%d\n",
          pipe->GetEndpointNumber(),
          pipe->GetType(),
          pipe->GetDirection(),
          pipe->GetMaxPacketSize());
    pipe->retain();

    return pipe;
}

IOUSBDevice *com_lloeki_xbox_one_controller::findDevice() const
{
    IOService *service = getProvider();
    if (service == NULL) {
        return NULL;
    }

    IOUSBDevice *device = OSDynamicCast(IOUSBDevice, service);
    if (device == NULL) {
        return NULL;
    }

    return device;
}

void com_lloeki_xbox_one_controller::closeAll() {
    if (pipeOut != NULL) { pipeOut->Abort(); pipeOut->release(); pipeOut = NULL; }
    if (pipeIn != NULL) { pipeIn->Abort(); pipeIn->release(); pipeIn = NULL; }
    if (interface !=  NULL) { if (interface->isOpen()) { interface->close(this); } interface = NULL; }
    if (device != NULL) { if (device->isOpen()) { device->close(this); } device = NULL; }
}


/********************************************************************************************************/
// Xbox One controller initialisation and communication

void com_lloeki_xbox_one_controller::writeCompleteProxy(void *target, void *parameter, IOReturn status, UInt32 bufferSizeRemaining)
{
    IOLog("[xbox_one_controller] data written (proxy)\n");

    if (target != NULL) {
        IOBufferMemoryDescriptor *buffer = (IOBufferMemoryDescriptor*)parameter;
        ((com_lloeki_xbox_one_controller*)target)->writeComplete(buffer, status);
    }
}

void com_lloeki_xbox_one_controller::writeComplete(IOBufferMemoryDescriptor *buffer, IOReturn status)
{
    if (status != kIOReturnSuccess) {
        IOLog("[xbox_one_controller] write error: 0x%08x\n", status);
    } else {
        IOLog("[xbox_one_controller] data written\n");
    }

    if (buffer != NULL) { buffer->release(); }
}

void com_lloeki_xbox_one_controller::readCompleteProxy(void *target, void *parameter, IOReturn status, UInt32 bufferSizeRemaining)
{
    IOLog("[xbox_one_controller] incoming data (proxy)\n");

    if (target != NULL) {
        IOBufferMemoryDescriptor *buffer = (IOBufferMemoryDescriptor*)parameter;
        ((com_lloeki_xbox_one_controller*)target)->readComplete(buffer, status, bufferSizeRemaining);
    }
}

void com_lloeki_xbox_one_controller::readComplete(IOBufferMemoryDescriptor *buffer, IOReturn status, UInt32 bufferSizeRemaining)
{
    IOLog("[xbox_one_controller] incoming data\n");
    IOLockLock(lock);

    switch (status) {
        case kIOReturnOverrun:
            IOLog("[xbox_one_controller] read: stalled\n");
            if (pipeIn != NULL) { pipeIn->ClearStall(); }
            break;
        case kIOReturnNotResponding:
            IOLog("[xbox_one_controller] read: not responding\n");
            break;
        case kIOReturnSuccess:
            IOLog("[xbox_one_controller] read: success\n");
            processPacket(buffer, bufferSizeRemaining);
            break;
        default:
            IOLog("[xbox_one_controller] read: unhandled status\n");
            break;
    }

    if (buffer != NULL) { buffer->release(); }
    if (!isInactive()) { queueRead(pipeIn); }

    IOLockUnlock(lock);
}

bool com_lloeki_xbox_one_controller::writeInit(IOUSBPipe *pipe) {
    char init_pkt_1[] = { 0x05, 0x20 };
    if (!queueWrite(pipe, init_pkt_1, sizeof(init_pkt_1))) {
        IOLog("[xbox_one_controller] init failed\n");
        return false;
    }

    return true;
}

bool com_lloeki_xbox_one_controller::readInit(IOUSBPipe *pipe) {
    return queueRead(pipe);
}

bool com_lloeki_xbox_one_controller::queueWrite(IOUSBPipe *pipe, const void *bytes, UInt32 length) {
    IOReturn err;
    IOUSBCompletion completion;

    if (pipe == NULL) { return false; }

    IOMemoryDescriptor *buffer = IOBufferMemoryDescriptor::inTaskWithOptions(kernel_task,
                                                                             0,
                                                                             length);
    if (buffer == NULL) {
        IOLog("[xbox_one_controller] write buffer error\n");
        return false;
    }

    buffer->writeBytes(0, bytes, length);
    completion.target = this;
    completion.action = writeCompleteProxy;
    completion.parameter = buffer;

    if ((err = pipe->Write(buffer, &completion)) != kIOReturnSuccess) {
        IOLog("[xbox_one_controller] write error: 0x%08x\n", err);
        return false;
    }

    IOLog("[xbox_one_controller] write queued: 0x%08x\n", err);
    return true;
}

bool com_lloeki_xbox_one_controller::queueRead(IOUSBPipe *pipe) {
    IOReturn err;
    IOUSBCompletion completion;

    if (pipe == NULL) { return false; }

    IOMemoryDescriptor *buffer = IOBufferMemoryDescriptor::inTaskWithOptions(kernel_task,
                                                                             0,
                                                                             pipe->GetMaxPacketSize());

    buffer->retain();
    completion.target = this;
    completion.action = readCompleteProxy;
    completion.parameter = buffer;

    if ((err = pipe->Read(buffer, &completion)) != kIOReturnSuccess) {
        IOLog("[xbox_one_controller] read queue error: 0x%08x\n", err);
        return false;
    }

    IOLog("[xbox_one_controller] read queued: 0x%08x\n", err);
    return true;
}


/********************************************************************************************************/
// Service events

IOReturn com_lloeki_xbox_one_controller::message(UInt32 type, IOService* provider, void* argument) {
    switch (type) {
        case kIOMessageDeviceSignaledWakeup:
            IOLog("[xbox_one_controller] message: wake up\n");
            writeInit(pipeOut);
            return kIOReturnSuccess;
        case kIOMessageServiceIsTerminated:
            IOLog("[xbox_one_controller] message: terminated\n");
            closeAll();
            return kIOReturnSuccess;
        default:
            return super::message(type, provider, argument);
    }
}


/********************************************************************************************************/
// Processing

void com_lloeki_xbox_one_controller::processPacket(IOBufferMemoryDescriptor *buffer, UInt32 length) {
    IOReturn err;
    UInt8 *raw_packet;
    xbox_one_controller_packet_btn *btn_packet;
    xbox_one_controller_packet_head *packet = (xbox_one_controller_packet_head *)buffer->getBytesNoCopy();

    switch (packet->type) {
        case xbox_one_controller_packet_type_btn:
            btn_packet = (xbox_one_controller_packet_btn *)packet;
            btn_packet->ly = -(btn_packet->ly + 1);
            btn_packet->ry = -(btn_packet->ry + 1);
        case xbox_one_controller_packet_type_x:
            err = handleReport(buffer, kIOHIDReportTypeInput);
            if (err != kIOReturnSuccess) {
                IOLog("[xbox_one_controller] failed to handle report\n");
            }

            IOLog("[xbox_one_controller] known packet (type=0x%02x, length=%d): 0x", packet->type, packet->len + 4);
            raw_packet = (UInt8 *)packet;
            for (int i = 0; i < (packet->len + 4); i++) {
                IOLog("%02x", raw_packet[i]);
            }
            IOLog("\n");
            break;
        default:
            IOLog("[xbox_one_controller] unknown packet type (buffer length=%d): 0x", length);
            raw_packet = (UInt8 *)packet;
            for (int i = 0; i < length; i++) {
                IOLog("%02x", raw_packet[i]);
            }
            IOLog("\n");
    }
}


/********************************************************************************************************/
// HID descriptor

IOReturn com_lloeki_xbox_one_controller::newReportDescriptor(IOMemoryDescriptor **descriptor) const
{
    IOLog("[xbox_one_controller] Descriptor\n");
    IOBufferMemoryDescriptor *buffer = IOBufferMemoryDescriptor::inTaskWithOptions(kernel_task,0,sizeof(xbox_one_controller_report_descriptor));

    if (buffer == NULL) return kIOReturnNoResources;

    buffer->writeBytes(0, xbox_one_controller_report_descriptor,
                       sizeof(xbox_one_controller_report_descriptor));
    *descriptor = buffer;

    return kIOReturnSuccess;
}

OSNumber *com_lloeki_xbox_one_controller::newPrimaryUsagePageNumber() const
{
    return OSNumber::withNumber(xbox_one_controller_report_descriptor[1], 8);
}

OSNumber *com_lloeki_xbox_one_controller::newPrimaryUsageNumber() const
{
    return OSNumber::withNumber(xbox_one_controller_report_descriptor[3], 8);
}

OSNumber *com_lloeki_xbox_one_controller::newVendorIDNumber() const
{
    return OSNumber::withNumber(findDevice()->GetVendorID(), 16);
}

OSNumber *com_lloeki_xbox_one_controller::newProductIDNumber() const
{
    return OSNumber::withNumber(findDevice()->GetProductID(), 16);
}

OSString *com_lloeki_xbox_one_controller::newManufacturerString() const
{
    IOReturn err;
    char manufacturer[1024];
    UInt8 index = findDevice()->GetManufacturerStringIndex();

    err = findDevice()->GetStringDescriptor(index, manufacturer, sizeof(manufacturer));
    if (err != kIOReturnSuccess) {
        manufacturer[0] = '\0';
    }

    return OSString::withCString(manufacturer);
}

OSString *com_lloeki_xbox_one_controller::newProductString() const
{
    IOReturn err;
    char product[1024];
    UInt8 index = findDevice()->GetProductStringIndex();

    err = findDevice()->GetStringDescriptor(index, product, sizeof(product));
    if (err != kIOReturnSuccess) {
        product[0] = '\0';
    }

    return OSString::withCString(product);
}

OSString *com_lloeki_xbox_one_controller::newSerialNumberString() const
{
    IOReturn err;
    char serial[1024];
    UInt8 index = findDevice()->GetSerialNumberStringIndex();

    err = findDevice()->GetStringDescriptor(index, serial, sizeof(serial));
    if (err != kIOReturnSuccess) {
        serial[0] = '\0';
    }

    return OSString::withCString(serial);
}

OSString *com_lloeki_xbox_one_controller::newTransportString() const
{
    return OSString::withCString("USB");
}

OSNumber *com_lloeki_xbox_one_controller::newLocationIDNumber() const
{
    OSNumber *number;
    UInt32 location_id;

    number = OSDynamicCast(OSNumber, findDevice()->getProperty("locationID"));
    if (!number)
    {
        return 0;
    }

    location_id = number->unsigned32BitValue();
    if (!location_id)
    {
        return 0;
    }

    return OSNumber::withNumber(location_id, 32);
}


/********************************************************************************************************/
