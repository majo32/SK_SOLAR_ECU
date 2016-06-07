#include "can_socket.h"

int can_socket_start_socket(const char* ifname, int * error) {
    /*int s;
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
    return s;*/
    return 0;
}

int can_socket_send_msg(int socket, int id, int len, const unsigned char * buff, int * error) {

    /*int nbytes;
    struct can_frame frame;

    frame.can_id = id;
    frame.can_dlc = len;
    memcpy(frame.data, buff, len);

    nbytes = write(socket, &frame, sizeof (struct can_frame));
    if (nbytes < 0) {
        *error = 1;
    }
    return nbytes;*/
    return 0;
}

int can_socket_wait_for_msg(int socket, int * id, int * len, unsigned char * buff, int * error) {
    /*int nbytes;
    struct can_frame frame;

    nbytes = read(socket, &frame, sizeof (struct can_frame));
    if (nbytes < 0) {
        *error = 1;
    }
    *id = frame.can_id;
    *len = frame.can_dlc;
    memcpy(buff, frame.data, *len);
    return nbytes;*/
    return 0;
}

int can_socket_destroy_socket(int socket, int * error) {
    /*close(socket);*/
    return 0;
}
#ifdef PHY_TEST
int main(int argc, char** argv) {
    int error;
    int socket = can_socket_start_socket("can0", &error);
    printf("%d %d\n", error, socket);
    char buff[8] = {1, 0, 0, 0, 0, 0, 0, 0};
    int l = can_socket_send_msg(socket, 0x99, 2, buff, &error);
    printf("%d %d\n", error, l);
    int len;
    int adr;
    l = can_socket_wait_for_msg(socket, &adr, &len, buff, &error);
    printf("%d %d %x %d %d\n", error, l, adr, len, sizeof(struct can_frame));
    printf("%x %x %x %x\n", buff[0], buff[1], buff[2], buff[3]);
    close(socket);
    l = can_socket_send_msg(socket, 0x99, 2, buff, &error);
    printf("%d %d\n", error, l);
}
#endif
