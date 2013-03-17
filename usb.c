#include <string.h>
#include "p18cxxx.h"

// PIC18F87J94 Configuration Bit Settings
#pragma config STVREN   = ON      	// Stack overflow reset
#pragma config XINST    = OFF   	// Extended instruction set
#pragma config BOREN    = ON		// BOR Enabled
#pragma config BORV     = 0		// BOR Set to "2.0V" nominal setting
#pragma config CP0      = OFF      	// Code protect disabled
#pragma config FOSC     = FRCPLL	// Firmware should also enable active clock tuning for this setting
#pragma config SOSCSEL  = LOW		// SOSC circuit configured for crystal driver mode
#pragma config CLKOEN   = OFF		// Disable clock output on RA6
#pragma config IESO     = OFF      	// Internal External (clock) Switchover
#pragma config PLLDIV   = NODIV		// 4 MHz input (from 8MHz FRC / 2) provided to PLL circuit
#pragma config POSCMD   = NONE		// Primary osc disabled, using FRC
#pragma config FSCM     = CSECMD	// Clock switching enabled, fail safe clock monitor disabled
#pragma config WPDIS    = WPDIS		// Program memory not write protected
#pragma config WPCFG    = WPCFGDIS	// Config word page of program memory not write protected
#pragma config IOL1WAY  = OFF		// IOLOCK can be set/cleared as needed with unlock sequence
#pragma config LS48MHZ  = SYSX2		// Low Speed USB clock divider
#pragma config WDTCLK   = LPRC		// WDT always uses INTOSC/LPRC oscillator
#pragma config WDTEN    = ON		// WDT disabled; SWDTEN can control WDT
#pragma config WINDIS   = WDTSTD	// Normal non-window mode WDT.
#pragma config VBTBOR   = OFF		// VBAT BOR disabled

#define MAX_PACKET_SIZE	    64	// maximum packet size for low-speed peripherals is 8 bytes, for full-speed peripherals it can be 8, 16, 32, or 64 bytes
#define	DEVICE		    1
#define	CONFIGURATION	    2
#define	STRING		    3
#define	INTERFACE	    4
#define	ENDPOINT	    5
#define HID		    0x21
#define REPORT		    0x22
#define	GET_STATUS	    0x00
#define CLEAR_FEATURE	    0x01
#define SET_FEATURE	    0x03
#define	SET_ADDRESS	    0x05
#define	GET_DESCRIPTOR	    0x06
#define GET_CONFIGURATION   0x08
#define	SET_CONFIGURATION   0x09
#define UOWN 0x80
#define DTSEN 0x08
#define BSTALL 0x04
#define DTS 0x40
#define PID 0x3C
#define EndPoint0Out bufferDescriptors[0]
#define EndPoint0In bufferDescriptors[1]
#define EndPoint1In bufferDescriptors[3]

BYTE Device[] = {
    0x12,	    // bLength
    DEVICE,	    // bDescriptorType
    0x00, 0x02,	    // bcdUSB (v2.0)
    0x00, 0x00, 0x00, // bDeviceClass, bDeviceSubClass, bDeviceProtocol
    MAX_PACKET_SIZE, // bMaxPacketSize
    0xD8, 0x04,	    // idVendor
    0x02, 0x00,	    // idProduct
    0x00, 0x00,	    // bcdDevice
    0x01, 0x02, 0x00, // iManufacturer, iProduct, iSerialNumber (none)
    0x01	    // bNumConfigurations
};

BYTE Configuration1[] = {
    0x09,	    // bLength
    CONFIGURATION,  // bDescriptorType
    0x22, 0x00,	    // wTotalLength
    0x01,	    // bNumInterfaces
    0x01, 0x00,	    // bConfigurationValue, iConfiguration (none)
    0x80, 0x32,	    // bmAttributes (non-self powered, no remote wake up), bMaxPower (100 mA)

    0x09,	    // bLength (Interface1 descriptor starts here)
    INTERFACE,	    // bDescriptorType
    0x00, 0x00, 0x01, // bInterfaceNumber, bAlternateSetting, bNumEndpoints (excluding EP0)
    0x03, 0x01,	    // bInterfaceClass (HID code), bInterfaceSubClass (Boot subclass)
    0x01, 0x00,	    // bInterfaceProtocol (Keyboard protocol), iInterface (none)
    
    0x09,	    // bLength (HID1 descriptor starts here)
    HID,	    // bDescriptorType
    0x00, 0x01,	    // bcdHID
    0x00, 0x01,	    // bCountryCode (none), bNumDescriptors
    REPORT,	    // bDescriptorType
    0x3F, 0x00,	    // wDescriptorLength

    0x07,	    // bLength (Endpoint1 descriptor starts here)
    ENDPOINT,	    // bDescriptorType
    0x81, 0x03,	    // bEndpointAddress (EP1 IN), bmAttributes (Interrupt)
    0x08, 0x00,	    // wMaxPacketSize
    0x01	    // bInterval (1 ms)
};

BYTE Report1[] = {
    0x05, 0x01, 0x09, 0x06, // Usage Page (Generic Desktop), Usage (Keyboard)
    0xA1, 0x01, 0x05, 0x07, // Collection (Application), Usage Page (Key Codes)
    0x19, 0xE0, 0x29, 0xE7, // Usage Minimum (224), Usage Maximum (231)
    0x15, 0x00, 0x25, 0x01, // Logical Minimum (0), Logical Maximum (1)
    0x75, 0x01, 0x95, 0x08, // Report Size (1), Report Count (8)
    0x81, 0x02,		    // Input (Data, Variable, Absolute),
    0x95, 0x01, 0x75, 0x08, // Report Count (1), Report Size (8)
    0x81, 0x01,		    // Input (Constant),
    0x95, 0x05, 0x75, 0x01, // Report Count (5), Report Size (1)
    0x05, 0x08,		    // Usage Page (Page# for LEDs),
    0x19, 0x01, 0x29, 0x05, // Usage Minimum (1), Usage Maxmimum (5)
    0x91, 0x02,		    // Output (Data, Variable, Absolute),
    0x95, 0x01, 0x75, 0x03, // Report Count (1), Report Size (3)
    0x91, 0x01,		    // Output (Constant),
    0x95, 0x06, 0x75, 0x08, // Report Count (6), Report Size (8)
    0x15, 0x00, 0x25, 0x65, // Logical Minimum (0), Logical Maximum (101)
    0x05, 0x07,		    // Usage Page (Key Codes),
    0x19, 0x00, 0x29, 0x65, // Usage Minimum (0), Usage Maximum (101)
    0x81, 0x00, 0xC0	    // Input (Data, Array), End Collection
};

BYTE String0[] = { 0x04, STRING, 0x09, 0x04 };
BYTE String1[] = { 0x0A, STRING, 'D', 0x00, 'e', 0x00, 'm', 0x00, '0', 0x00 };
BYTE String2[] = { 0x0A, STRING, 'D', 0x00, 'e', 0x00, 'm', 0x00, '0', 0x00 };

typedef enum State
{
    STATE_POWERED = 0x00,
    STATE_DEFAULT = 0x01,
    STATE_ADDRESS = 0x02,
    STATE_CONFIG = 0x03
} State;

typedef struct
{
    unsigned Recipient : 5;
    unsigned RequestType : 2;
    unsigned Direction : 1;
    BYTE Request;
    union {
	struct {
	    short Value;
	};
	struct {
	    BYTE ValueLow;
	    BYTE ValueHigh;
	};
    };
    short Index;
    short Length;
} SetupPacket;

typedef struct {
    BYTE Status;
    BYTE ByteCount;
    BYTE* Address;
} BufferDescriptor;

typedef struct {
    State State;
    BYTE HasPendingAddress;
    BYTE PendingAddress;
    BYTE LastError;
} USB;

USB usb;
BYTE EP0_OUT_buffer[MAX_PACKET_SIZE];
BYTE EP0_IN_buffer[MAX_PACKET_SIZE];
BYTE EP1_IN_buffer[8];
BufferDescriptor bufferDescriptors[4] @ 0x100;

static void ResetEndpointZero(BOOL includeInBuffer)
{
    EndPoint0Out.ByteCount = MAX_PACKET_SIZE;
    EndPoint0Out.Status = UOWN | DTSEN;

    if (includeInBuffer)
    {
	EndPoint0In.Status = DTSEN;
    }
}

static void SetBufferDescriptor(BufferDescriptor* descriptor, void* data, BYTE length)
{
    memcpy(descriptor->Address, data, length);
    descriptor->ByteCount = length;

    BYTE toggledDTS = (descriptor->Status ^ DTS) & DTS;
    descriptor->Status = toggledDTS | UOWN | DTSEN;
}

static BOOL ReturnDescriptor(BufferDescriptor* bufferDescriptor) // TODO: this entire function is a bit too hard coded...
{
    BOOL success = TRUE;
    SetupPacket* setupPacket = (SetupPacket*)bufferDescriptor->Address;

    if (setupPacket->ValueHigh == DEVICE)
    {
	SetBufferDescriptor(&EndPoint0In, Device, Device[0]);
    }
    else if (setupPacket->ValueHigh == CONFIGURATION && setupPacket->ValueLow == 0)
    {
	SetBufferDescriptor(&EndPoint0In, Configuration1, Configuration1[2]);
    }
    else if (setupPacket->ValueHigh == STRING && setupPacket->ValueLow == 0)
    {
	SetBufferDescriptor(&EndPoint0In, String0, String0[0]);
    }
    else if (setupPacket->ValueHigh == STRING && setupPacket->ValueLow == 1)
    {
	SetBufferDescriptor(&EndPoint0In, String1, String1[0]);
    }
    else if (setupPacket->ValueHigh == STRING && setupPacket->ValueLow == 2)
    {
	SetBufferDescriptor(&EndPoint0In, String2, String2[0]);
    }
    else if (setupPacket->ValueHigh == REPORT && setupPacket->ValueLow == 0)
    {
	SetBufferDescriptor(&EndPoint0In, Report1, 0x3F);
    }
    else if (setupPacket->ValueHigh == HID && setupPacket->ValueLow == 0)
    {
	BYTE* data = Configuration1 + 0x12;
	SetBufferDescriptor(&EndPoint0In, data, data[0]);
    }
    else
    {
	success = FALSE;
    }

    return success;
}

static BOOL ProcessSetupToken(BufferDescriptor* bufferDescriptor)
{
    BOOL success = TRUE;
    
    SetupPacket* setupPacket = (SetupPacket*)bufferDescriptor->Address;

    ResetEndpointZero(TRUE);

    if (setupPacket->RequestType != 0) // we only care about "Standard" request types
    {
	success = FALSE;
    }
    else if (setupPacket->Request == GET_CONFIGURATION)
    {
	BYTE data[] = { 0x01 }; // always return 1, because we only support one configuration
	SetBufferDescriptor(&EndPoint0In, data, sizeof(data));
    }
    else if (setupPacket->Request == GET_DESCRIPTOR)
    {
	success = ReturnDescriptor(bufferDescriptor);
    }
    else if (setupPacket->Request == GET_STATUS && setupPacket->Recipient == 0) // only handle if the recipient is "Device" (i.e. 0)
    {
	BYTE data[] = { 0x00, 0x00 }; // non-self powered, no remote wakeup
	SetBufferDescriptor(&EndPoint0In, data, sizeof(data));
    }
    else if (setupPacket->Request == SET_ADDRESS)
    {
	usb.HasPendingAddress = TRUE;
	usb.PendingAddress = setupPacket->Value; // save new address
	SetBufferDescriptor(&EndPoint0In, NULL, 0);
    }
    else if (setupPacket->Request == SET_CONFIGURATION)
    {
	usb.State = (setupPacket->Value == 0) ? STATE_ADDRESS : STATE_CONFIG; // configure the device (assumes there's only one available configuration)
	SetBufferDescriptor(&EndPoint0In, NULL, 0);
    }
    else
    {
	success = FALSE;
    }

    UCONbits.PKTDIS = 0; // clear the packet disable bit

    return success;
}

static void ProcessInToken(BYTE endPoint)
{
    if (endPoint == 0 && usb.HasPendingAddress)
    {
	usb.HasPendingAddress = FALSE;
	UADDR = usb.PendingAddress;
	usb.State = (UADDR == 0) ? STATE_DEFAULT : STATE_ADDRESS;
    }
}

static void ProcessOutToken(BYTE endPoint)
{
    if (endPoint == 0)
    {
	ResetEndpointZero(FALSE);
	SetBufferDescriptor(&EndPoint0In, NULL, 0);
    }
}

static void ServiceUSB()
{
    if (UIRbits.UERRIF) // usb error interupt
    {
	usb.LastError = UEIR; // store for debugging
	UEIR = 0x00; // clear error flags
    }
    else if (UIRbits.SOFIF) // Start-of-Frame Token Interrupt bit
    {
	UIRbits.SOFIF = 0; // clear interupt
    }
    else if (UIRbits.IDLEIF) // Idle Detect Interrupt bit
    {
	UCONbits.SUSPND = 1;
	UIRbits.IDLEIF = 0; // clear interupt
    }
    else if (UIRbits.ACTVIF) // Bus Activity Detect Interrupt bit
    {
	UCONbits.SUSPND = 0; // un-suspend usb module
	while (UIRbits.ACTVIF) // clear flag (repeat until flag actually clears per documentation)
	{
	    UIRbits.ACTVIF = 0;
	}
    }
    else if (UIRbits.STALLIF) // A STALL Handshake Interrupt bit
    {
	UIRbits.STALLIF = 0; // clear interupt
    }
    else if (UIRbits.URSTIF) // USB Reset Interrupt bit
    {
	UIRbits.TRNIF = 0; // clear TRNIF four times to clear out the USTAT FIFO
	UIRbits.TRNIF = 0;
	UIRbits.TRNIF = 0;
	UIRbits.TRNIF = 0;

	ResetEndpointZero(TRUE);

	UADDR = 0x00; // set USB Address to 0
	UIR = 0x00; // clear all the USB interrupt flags

	UEP0bits.EPOUTEN = 1; // endpoint enable out
	UEP0bits.EPINEN = 1; // endpoint enable in
	UEP0bits.EPHSHK = 1; // endpoint enable handshake

	UEP1bits.EPINEN = 1; // endpoint enable in
	UEP1bits.EPHSHK = 1; // endpoint enable handshake

	usb.State = STATE_DEFAULT;
    }
    else if (UIRbits.TRNIF) // Transaction Complete Interrupt bit
    {
	BOOL success = TRUE;

	BYTE endPointIndex = (USTAT & 0x7C) / (BYTE)sizeof(BufferDescriptor); // mask out bits 0, 1, and 7 of USTAT for offset into the buffer descriptor table
	BufferDescriptor* bufferDescriptor = &bufferDescriptors[endPointIndex];

	BYTE pid = (bufferDescriptor->Status & PID) >> 2; // extract Packet Identifier (PID) bits
	if (pid == 0x0D) // "Setup" token
	{
	    success = ProcessSetupToken(bufferDescriptor);
	}
	else if (pid == 0x09) // "In" token
	{
	    ProcessInToken(USTATbits.ENDP);
	}
	else if (pid == 0x01) // "Out" token
	{
	    ProcessOutToken(USTATbits.ENDP);
	}

	if (!success)
	{
	    ResetEndpointZero(TRUE);
	    EndPoint0Out.Status = UOWN | BSTALL; // issue a protocol stall on EP0 Out
	    EndPoint0In.Status = UOWN | BSTALL; // issue a protocol stall on EP0 In
	}
	
	UIRbits.TRNIF = 0; // clear TRNIF interrupt flag
    }
}

void interrupt InterruptHandler(void)
{
    if (PIR2bits.USBIF)
    {
	ServiceUSB();
	PIR2bits.USBIF = 0; // clear interupt
    }
}

void InitInterupts()
{
    INTCONbits.GIE = 1; // enable global interrupts
    INTCONbits.PEIE = 1; // enable peripheral interrupts

    PIR2bits.USBIF = 0;
    PIE2bits.USBIE = 1;
}

static void InitUSB()
{
    usb.State = STATE_POWERED;
    usb.HasPendingAddress = FALSE;
    usb.LastError = 0x00;

    EndPoint0Out.Address = EP0_OUT_buffer;
    EndPoint0In.Address = EP0_IN_buffer;
    EndPoint1In.Address = EP1_IN_buffer;

    ACTCONbits.ACTEN = 1; // Active Clock Tuning (ACT) module is enabled, updates to OSCTUNE are exclusive to the ACT module
    ACTCONbits.ACTSRC = 1; // The FRC oscillator is tuned to approximately match the USB host clock tolerance    
    while (OSCCON2bits.LOCK == 0); // wait for PLL to lock

    UIR = 0x00; // clear all USB interrupt flags
    UIE = 0xFF; // enable all USB interrupts
    UEIR = 0x00; // clear all USB Error interrupt flags
    UEIE = 0xFF; // enable all USB Error interrupts

    UCFGbits.UPUEN = 1; // use on chip pull up resistors (on D+ for full speed usb)
    UCFGbits.UTRDIS = 0; // use on chip transceiver
    UCFGbits.FSEN = 1; // use full speed usb
    UCFGbits.PPB0 = 0; // setup ping-pong buffer state
    UCFGbits.PPB1 = 0;

    UCONbits.USBEN = 1; // enable USB system

    while (UCONbits.SE0); // wait for the first SE0 to end
}

void main()
{
    TRISBbits.TRISB4 = 1; // onboard switch as input
    TRISEbits.TRISE0 = 0; // set led pin as output
    TRISEbits.TRISE1 = 0; // set led pin as output

    LATEbits.LATE0 = 0; // turn off "button" led
    LATEbits.LATE1 = 1; // turn on "power" led

    InitInterupts();
    InitUSB();

    BYTE counter = 0x00;

    while (1)
    {
	if (usb.State == STATE_CONFIG && !(EndPoint1In.Status & UOWN))
	{
	    LATEbits.LATE0 = !PORTBbits.RB4; // show button state w/ led

	    BYTE buffer[8] = {0};
	    if (!PORTBbits.RB4)
	    {
		buffer[2] = 0x04 + counter++;
		counter &= 0x1F;
	    }

	    SetBufferDescriptor(&EndPoint1In, buffer, 8);
	}
    }
}
