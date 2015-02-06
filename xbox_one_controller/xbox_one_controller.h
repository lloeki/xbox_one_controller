#include <IOKit/hid/IOHIDDevice.h>
#include <IOKit/usb/IOUSBDevice.h>
#include <IOKit/usb/IOUSBInterface.h>

class com_lloeki_xbox_one_controller : public IOHIDDevice
{
    OSDeclareDefaultStructors(com_lloeki_xbox_one_controller)
private:
    virtual IOUSBDevice *findDevice() const;
    virtual IOUSBDevice *findAndOpenDevice(IOService *device);
    virtual IOUSBInterface *findAndOpenInterface(IOUSBDevice *device);
    virtual IOUSBPipe *findAndOpenPipe(IOUSBInterface *interface, int direction);
    virtual void closeAll();

    virtual bool writeInit(IOUSBPipe *pipe);
    virtual bool readInit(IOUSBPipe *pipe);
    virtual bool queueWrite(IOUSBPipe *pipe, const void *bytes, UInt32 length);
    virtual bool queueRead(IOUSBPipe *pipe);
    virtual void writeComplete(IOBufferMemoryDescriptor *buffer, IOReturn status);
    virtual void readComplete(IOBufferMemoryDescriptor *buffer, IOReturn status, UInt32 bufferSizeRemaining);
    static void writeCompleteProxy(void *target, void *parameter, IOReturn status, UInt32 bufferSizeRemaining);
    static void readCompleteProxy(void *target, void *parameter, IOReturn status, UInt32 bufferSizeRemaining);

    virtual void processPacket(IOBufferMemoryDescriptor *buffer, UInt32 length);

    IOUSBDevice    *device;
    IOUSBInterface *interface;
    IOUSBPipe      *pipeIn;
    IOUSBPipe      *pipeOut;
    IOLock         *lock;
public:
    virtual bool init(OSDictionary *dictionary = 0);
    virtual void free(void);
    virtual IOService *probe(IOService *provider, SInt32 *score);
    virtual bool start(IOService *provider);
    virtual void stop(IOService *provider);
    virtual IOReturn message(UInt32 type, IOService* provider, void* argument);

    virtual IOReturn newReportDescriptor(IOMemoryDescriptor **descriptor) const;

    virtual OSNumber *newPrimaryUsagePageNumber() const;
    virtual OSNumber *newPrimaryUsageNumber() const;
    virtual OSNumber *newVendorIDNumber() const;
    virtual OSNumber *newProductIDNumber() const;
    virtual OSString *newManufacturerString() const;
    virtual OSString *newProductString() const;
    virtual OSString *newSerialNumberString() const;
    virtual OSString *newTransportString() const;
    virtual OSNumber *newLocationIDNumber() const;
};
