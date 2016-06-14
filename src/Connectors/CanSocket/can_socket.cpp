#include "can_socket.h"


int can_socket_start_socket(const char* ifname, int * error) {
    int s = 0;
#if SOLAR_SIMULATION == 0
    struct sockaddr_can addr;
    struct ifreq ifr;

    if ((s = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
        *error = 1;
        return -1;
    }

    strcpy(ifr.ifr_name, ifname);
    ioctl(s, SIOCGIFINDEX, &ifr);

    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(s, (struct sockaddr *) &addr, sizeof (addr)) < 0) {
        *error = 2;
        return -1;
    }
    *error = 0;
#endif
    return s;
}

int can_socket_send_msg(int socket, int id, int len, const unsigned char * buff, int * error) {
    int nbytes = 0;
#if SOLAR_SIMULATION == 0
    struct can_frame frame;

    frame.can_id = id;
    frame.can_dlc = len;
    memcpy(frame.data, buff, len);

    nbytes = write(socket, &frame, sizeof (struct can_frame));
    if (nbytes < 0) {
        *error = 1;
    }
#endif
    return nbytes;
}

int can_socket_wait_for_msg(int socket, int * id, int * len, unsigned char * buff, int * error) {
    int nbytes = 0;
#if SOLAR_SIMULATION == 0
    struct can_frame frame;

    nbytes = read(socket, &frame, sizeof (struct can_frame));
    if (nbytes < 0) {
        *error = 1;
    }
    *id = frame.can_id;
    *len = frame.can_dlc;
    memcpy(buff, frame.data, *len);
#endif
    return nbytes;
}

int can_socket_destroy_socket(int socket, int * error) {
#if SOLAR_SIMULATION == 0
    close(socket);
#endif
    return 1;
}
