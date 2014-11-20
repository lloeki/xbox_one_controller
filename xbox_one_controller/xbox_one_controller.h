#include <IOKit/hid/IOHIDDevice.h>

class com_lloeki_xbox_one_controller : public IOHIDDevice
{
    OSDeclareDefaultStructors(com_lloeki_xbox_one_controller)
protected:
    virtual IOUSBDevice *device() const;
public:
    virtual bool init(OSDictionary *dictionary = 0);
    virtual void free(void);
    virtual IOService *probe(IOService *provider, SInt32 *score);
    virtual bool start(IOService *provider);
    virtual void stop(IOService *provider);
    
    virtual IOReturn newReportDescriptor(IOMemoryDescriptor **descriptor) const;
    
    virtual OSNumber* newPrimaryUsagePageNumber() const;
    virtual OSNumber* newPrimaryUsageNumber() const;
    virtual OSNumber* newVendorIDNumber() const;
    virtual OSNumber* newProductIDNumber() const;
    virtual OSString* newManufacturerString() const;
    virtual OSString* newProductString() const;
    virtual OSString* newSerialNumberString() const;
    virtual OSString* newTransportString() const;
    virtual OSNumber* newLocationIDNumber() const;
};