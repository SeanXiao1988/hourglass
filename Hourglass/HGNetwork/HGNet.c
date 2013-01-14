#include "HGNet.h"

// packet
void packet_setup(packet_t *p, uint32_t prt, uint32_t seq)
{
	if (p == NULL)
		return;
    
	p->protocol = prt;
	p->sequence = seq;
    p->datasize = 0;
}

int32_t packet_append_data(packet_t *p, void *data, int32_t size)
{
	int32_t bytesAppended = 0;
    
	do
	{
		if (p == NULL || size <= 0)
			break;
        
        if (p->datasize + size >= PACKET_BUFFER_SIZE)
            break;
        
        memcpy(p->data + size, data, size);
        p->datasize += size;
        
        bytesAppended = size;
        
	} while (0);
    
	return bytesAppended;
}

void packet_seal(packet_t *p)
{
    if (p == NULL)
        return;
    // calculate checksum here
    // pack->checksum = ...;
    
    p->protocol = htonl(p->protocol);
    p->sequence = htonl(p->sequence);
    p->checksum = htonl(p->checksum);
    p->datasize = htonl(p->datasize);
}

int32_t packet_get_size(packet_t *p)
{
    int32_t size = 0;
    
    if (p != NULL)
    {
        size += sizeof(packet_t);
        size -= PACKET_BUFFER_SIZE;
        size += p->datasize;
    }
    
    return size;
}

// Address
void address_setup_empty(address_t *addr)
{
    if (addr == NULL)
        return;

    addr->address = 0;
    addr->port = 0;
}

void address_setup_ip_and_port(address_t *addr, uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint16_t port)
{
    if (addr == NULL)
        return;

    addr->address = (a << 24) | (b << 16) | (c << 8) | d;
    addr->port = port;
}

void address_setup(address_t *addr, uint32_t add, uint16_t port)
{
    if (addr == NULL)
        return;

    addr->address = add;
    addr->port = port;
}

uint8_t address_get_a(const address_t *addr)
{
    if (addr == NULL)
        return 0;

    return (uint8_t)(addr->address >> 24);
}

uint8_t address_get_b(const address_t *addr)
{
    if (addr == NULL)
        return 0;

    return (uint8_t)(addr->address >> 16);
}

uint8_t address_get_c(const address_t *addr)
{
    if (addr == NULL)
        return 0;

    return (uint8_t)(addr->address >> 8);
}

uint8_t address_get_d(const address_t *addr)
{
    if (addr == NULL)
        return 0;

    return (uint8_t)(addr->address);
}

uint32_t address_is_equal(const address_t *addr1, const address_t *addr2)
{
    if (addr1 == NULL || addr2 == NULL)
        return 0;

    return ((addr1 == addr2) || (addr1->address == addr2->address && addr1->port == addr2->port));
}

// net
uint32_t net_initialize(void)
{
#if PLATFORM == PLATFORM_WINDOWS
    WSADATA wsaData;
    return WSAStartup(MAKEWORD(2, 2), &wsaData) != NO_ERROR;
#else
    return 1;
#endif
}

void net_shutdown(void)
{
#if PLATFORM == PLATFORM_WINDOWS
    WSACleanup();
#endif
}

// socket
void socket_setup(socket_t *s)
{
    if (s != NULL)
        s->socketHandle = 0;
}

int32_t socket_is_open(socket_t *s)
{
    int32_t isOpen = 0;
    if (s != NULL && s->socketHandle != 0)
        isOpen = 1;

    return isOpen;
}

int32_t socket_open(socket_t *s, uint16_t port)
{
	struct sockaddr_in addr;

#if PLATFORM == PLATFORM_WINDOWS
	DWORD nonBlocking = 1;
#else
    int nonBlocking = 1;
#endif

    if (socket_is_open(s))
        return SOCKET_ERROR_ALREADY_OPEN;

    s->socketHandle = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (s->socketHandle <= 0)
    {
        s->socketHandle = 0;
        return SOCKET_ERROR_CANT_CREATE;
    }

    // bind
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons((uint16_t)port);

    if (bind(s->socketHandle, (const struct sockaddr *)&addr, sizeof(struct sockaddr_in)) < 0)
    {
        socket_close(s);
        return SOCKET_ERROR_CANT_BIND;
    }

    // set non-blocking io
#if PLATFORM == PLATFORM_WINDOWS
    if (ioctlsocket(s->socketHandle, FIONBIO, &nonBlocking) != 0)
    {
        socket_close(s);
        return SOCKET_ERROR_CANT_SET_NONBLK;
    }
#else

    if (fcntl(s->socketHandle, F_SETFL, O_NONBLOCK, nonBlocking) == -1)
    {
        socket_close(s);
        return SOCKET_ERROR_CANT_SET_NONBLK;
    }
#endif

    return SOCKET_ERROR_NO_ERROR;
}

void socket_close(socket_t *s)
{
    if (s == NULL || s->socketHandle <= 0)
        return;

#if PLATFORM == PLATFORM_WINDOWS
    closesocket(s->socketHandle);
#else
    shutdown(s->socketHandle, 0);
#endif

    s->socketHandle = 0;
}

int32_t socket_send(const socket_t *s, const address_t *destination, const void *data, int size)
{
	int sent_bytes = 0;
    struct sockaddr_in addr;

    if (data == NULL || s == NULL || destination == NULL || size <= 0)
        return sent_bytes;

    if (s->socketHandle == 0)
        return sent_bytes;

    if (destination->address == 0 || destination->port == 0)
        return sent_bytes;

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(destination->address);
    addr.sin_port = htons((uint16_t)destination->port);

    sent_bytes = (int)sendto(s->socketHandle, (const char*)data, size, 0, (struct sockaddr *)&addr, sizeof(struct sockaddr_in));

    return sent_bytes;
}

int32_t socket_receive(const socket_t *s, address_t *sender, void *data, int size)
{
	int receive_bytes = 0;
    struct sockaddr_in from;
	uint32_t addr = 0;
	uint16_t port = 0;

#if PLATFORM == PLATFORM_WINDOWS
    typedef int socklen_t;
#endif
	socklen_t fromLength;

    if (data == NULL || size <= 0 || s == NULL || sender == NULL)
        return receive_bytes;

    if (s->socketHandle == 0)
        return receive_bytes;

    fromLength = sizeof(from);

    receive_bytes = (int)recvfrom(s->socketHandle, (char*)data, size, 0, (struct sockaddr *)&from, &fromLength);

    if (receive_bytes <= 0)
        return 0;

    addr = ntohl(from.sin_addr.s_addr);
    port = ntohs(from.sin_port);

    sender->address = addr;
    sender->port = port;

    return receive_bytes;
}
