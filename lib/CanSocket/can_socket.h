/* 
 * File:   can_socket.h
 * Author: majo32
 *
 * Created on June 7, 2016, 5:50 PM
 */

#ifndef CAN_SOCKET_H
#define	CAN_SOCKET_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <net/if.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>

//#include <linux/can.h>
//#include <linux/can/raw.h>

#ifdef	__cplusplus
extern "C" {
#endif

    extern int can_socket_start_socket(const char* ifname,int * error);
    extern int can_socket_wait_for_msg(int socket, int * id, int * len, unsigned char * buff,int * error);
    extern int can_socket_send_msg(int socket, int id ,int len, const unsigned char * buff,int * error);
    extern int can_socket_destroy_socket(int socket,int * error);
   


#ifdef	__cplusplus
}
#endif

#endif	/* CAN_SOCKET_H */

