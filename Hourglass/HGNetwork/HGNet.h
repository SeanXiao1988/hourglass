#ifndef HGNET_H
#define HGNET_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include <string.h>
// Platform define
    
#define PLATFORM_UNSUPPORTED    0
#define PLATFORM_WINDOWS        1
#define PLATFORM_UNIX           2
#define PLATFORM_MACOS          3
#define PLATFORM_IPHONE         4
#define PLATFORM_IOS_SIMULATOR  5

#if defined(WIN32) || defined(_WIN32)
    #define PLATFORM PLATFORM_WINDOWS
#elif defined(__APPLE__)
    #include "TargetConditionals.h"
    #if TARGET_OS_IPHONE
        #define PLATFORM PLATFORM_IPHONE
    #elif TARGET_IPHONE_SIMULATOR
        #define PLATFORM PLATFORM_IOS_SIMULATOR
    #elif TARGET_OS_MAC
        #define PLATFORM PLATFORM_MACOS
    #else
        #define PLATFORM PLATFORM_UNSUPPORTED
    #endif
#elif defined(__linux)
    #define PLATFORM PLATFORM_UNIX
#elif defined(__unix) // all unices not caught above
    #define PLATFORM PLATFORM_UNIX
//#elif __posix
// POSIX
#else
    #define PLATFORM PLATFORM_UNSUPPORTED
#endif

#if PLATFORM == PLATFORM_WINDOWS

#include <winsock2.h>
#pragma comment( lib, "wsock32.lib" )
#include "stdint.h"

#pragma warning(disable : 4081)
#pragma warning(disable : 4706)
#pragma warning(disable : 4819)

#define WIN32_MEAN_AND_LEAN
#include <windows.h>

#define M_PI 3.14159265358979323846264338327950288
#include "stdint.h"

#elif PLATFORM == PLATFORM_MACOS || PLATFORM == PLATFORM_UNIX || PLATFORM == PLATFORM_IPHONE || PLATFORM == PLATFORM_IOS_SIMULATOR

	#include <stdint.h>

	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <fcntl.h>

#else

	#error unknown platform!

#endif

// packet
#define PACKET_BUFFER_SIZE 512
typedef struct _package_t
{
	uint32_t protocol;
	uint32_t sequence;
	uint32_t checksum;

	int32_t  datasize;
	char	 data[PACKET_BUFFER_SIZE];
}packet_t;

extern void packet_setup(packet_t *p, uint32_t prt, uint32_t seq);
extern int32_t packet_append_data(packet_t *p, void *data, int32_t size);
extern void packet_seal(packet_t *p);
extern int32_t packet_get_size(packet_t *p);

// Address
typedef struct _address_t
{
    uint32_t address;
    uint16_t port;
}address_t;

extern void address_setup_empty(address_t *addr);
extern void address_setup_ip_and_port(address_t *addr, uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint16_t port);
extern void address_setup(address_t *addr, uint32_t add, uint16_t port);
extern uint8_t address_get_a(const address_t *addr);
extern uint8_t address_get_b(const address_t *addr);
extern uint8_t address_get_c(const address_t *addr);
extern uint8_t address_get_d(const address_t *addr);
extern uint32_t address_is_equal(const address_t *addr1, const address_t *addr2);
extern uint32_t net_initialize(void);
extern void net_shutdown(void);

typedef struct _socket_t
{
    int socketHandle;
}socket_t;

extern void socket_setup(socket_t *s);
extern int32_t socket_is_open(socket_t *s);

#define SOCKET_ERROR_NO_ERROR           0
#define SOCKET_ERROR_ALREADY_OPEN       1
#define SOCKET_ERROR_CANT_BIND          2
#define SOCKET_ERROR_CANT_CREATE        3
#define SOCKET_ERROR_CANT_SET_NONBLK    4

extern int32_t socket_open(socket_t *s, uint16_t port);
extern void socket_close(socket_t *s);
extern int32_t socket_send(const socket_t *s, const address_t *destination, const void *data, int size);
extern int32_t socket_receive(const socket_t *s, address_t *sender, void *data, int size);

#ifdef __cplusplus
}
#endif
        
#endif // HGNET_H_
