#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <libusb-1.0/libusb.h>
#include "ftusb.h"
#include "ftd_configs.h"

extern unsigned char buffer[1024];
extern unsigned char Desc_buffer[1024];
unsigned char *int_buffer;
int *actLen;
uint16_t cnt = 0;

void usb_device(uint16_t idv)
{
    struct libusb_device **list, *device;
    struct libusb_device_descriptor devDesc, devDesc2;
    struct libusb_config_descriptor config, config2, **config_ptr;
    struct libusb_interface_descriptor intfs, intfs2;
    struct libusb_endpoint_descriptor endpin, endpout, endpintr;
    struct libusb_transfer *transfer;
    libusb_device_handle *handle;
    handle = usbGetDevice(idv);
    // printf("device----           %p \n", handle);
    devDesc = usbDevDesc(handle, 0x80, LIBUSB_REQUEST_GET_DESCRIPTOR, 0, Desc_buffer, 50, 1);
    config = usbConfDesc(handle, 0x80, LIBUSB_REQUEST_GET_DESCRIPTOR, 0, Desc_buffer, 50);
    intfs = intfDescp();
    endpin = endPointDescp_in();
    endpout = endPointDescp_out();
    endpintr = endPointDescp_intr();
    printf("ep in  0x%.2X\n", endpin.bEndpointAddress);
    printf("ep out 0x%.2X\n\n", endpout.bEndpointAddress);
    usbStrDesc(handle, devDesc);
    // usbConfDesc2(handle, 0x80, LIBUSB_REQUEST_GET_DESCRIPTOR, 0, Desc_buffer, 50);
    // config2 = configDescp();
    if (libusb_kernel_driver_active(handle, intfs.bInterfaceNumber) == 1) // 0-active 1-not active
    {
        if (libusb_detach_kernel_driver(handle, intfs.bInterfaceNumber) != 0)
            printf("auto detach() Error\n\n");
    }
    if (libusb_claim_interface(handle, intfs.bInterfaceNumber) == 0)
        printf("********interface free******** \n\n");

    ftd_open(handle);
    ftd_reset_dev(handle);
    ftd_setBaud(handle, 9600);
    // usb_send_string(handle, devDesc, endpout, endpin, 100, "U123456789012345678900");
    // usb_file_read(handle, devDesc,endpin,10, "myfile.txt");
    usb_read_string(handle, endpin);
    
    libusb_close(handle);
    printf("\nEnd: \n");
    libusb_exit(0);
}
int main(int argc, char const *argv[])
{
    usb_device(id_vid);

    return 0;
}
