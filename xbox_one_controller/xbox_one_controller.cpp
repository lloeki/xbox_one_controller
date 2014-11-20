#include <IOKit/IOLib.h>
#include <IOKit/usb/IOUSBDevice.h>

#include "xbox_one_controller.h"
#include "xbox_one_controller_report_descriptor.h"

// This required macro defines the class's constructors, destructors,
// and several other methods I/O Kit requires.
OSDefineMetaClassAndStructors(com_lloeki_xbox_one_controller, IOHIDDevice)

// Define the driver's superclass.
#define super IOHIDDevice
bool com_lloeki_xbox_one_controller::init(OSDictionary *dict)
{
    bool result = super::init(dict);
    IOLog("[xbox_one_controller] Initializing\n");
    return result;
}

void com_lloeki_xbox_one_controller::free(void)
{
    IOLog("[xbox_one_controller] Freeing\n");
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
    return result;
}

void com_lloeki_xbox_one_controller::stop(IOService *provider)
{
    IOLog("[xbox_one_controller] Stopping\n");
    super::stop(provider);
}

IOUSBDevice *com_lloeki_xbox_one_controller::device() const
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
    return OSNumber::withNumber(device()->GetVendorID(), 16);
}

OSNumber *com_lloeki_xbox_one_controller::newProductIDNumber() const
{
    return OSNumber::withNumber(device()->GetProductID(), 16);
}

OSString *com_lloeki_xbox_one_controller::newManufacturerString() const
{
    IOReturn err;
    char manufacturer[1024];
    UInt8 index = device()->GetManufacturerStringIndex();
    
    err = device()->GetStringDescriptor(index, manufacturer, sizeof(manufacturer));
    if (err != kIOReturnSuccess) {
        manufacturer[0] = '\0';
    }
    
    return OSString::withCString(manufacturer);
}

OSString *com_lloeki_xbox_one_controller::newProductString() const
{
    IOReturn err;
    char product[1024];
    UInt8 index = device()->GetProductStringIndex();
    
    err = device()->GetStringDescriptor(index, product, sizeof(product));
    if (err != kIOReturnSuccess) {
        product[0] = '\0';
    }

    return OSString::withCString(product);
}

OSString *com_lloeki_xbox_one_controller::newSerialNumberString() const
{
    IOReturn err;
    char serial[1024];
    UInt8 index = device()->GetSerialNumberStringIndex();

    err = device()->GetStringDescriptor(index, serial, sizeof(serial));
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
    
    number = OSDynamicCast(OSNumber, device()->getProperty("locationID"));
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
