/*
 * common.h
 *
 *  Created on: 25.03.2011
 */
#ifndef COMMON_H_
#define COMMON_H_

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#define CAN_NET_LINUX
#include "../../common/net/can_net.h"

//#define TEST_ERRORS

extern unsigned sleep_time_us;
extern unsigned sleep_time_us_inter;

extern uint32_t test_short_msg_int;

extern char* test_long_msg;
extern uint32_t test_long_msg_len;

#ifdef TEST_ERRORS
extern uint32_t test_unknown_hwa;
extern uint8_t test_unknown_version;
#endif

extern uint32_t test_succesful_test_msg;

#define TEST_SHORT						1

#define TEST_LONG						2
//char* test_long_msg = "****_Build_of_configuration_Debug_for_project_Project_MasterDevice_****make:_Nothing_to_be_done_for_`all'._****";

#ifdef TEST_ERRORS

#define TEST_ERR_WRONG_FRAMES_ORDER		3

#define TEST_ERR_MSG_FROM_UNKNOWN_NODE	4

#define TEST_ERR_UNKNOWN_PROTOCOL		5

#define TEST_ERR_NEW_MSG_BEFORE_END_OF_OLD	6

#endif

#define TEST_RANDOM_TRASH				7

#define TEST_SUCCESFUL_TEST				8

void int_msg_data(msg_lvl2_t* msg, uint32_t data);

void test_print_msg(msg_lvl2_t* msg);

int test_send_msg(uint32_t hwa, uint16_t id, uint8_t smb, uint8_t port, int msg_type, can_net_send_callback_t cb);

int test_cmp_msg(msg_lvl2_t* msg, uint32_t hwa, uint16_t id, uint8_t smb, int msg_type);

int test_msg(uint32_t hwa, uint8_t id, uint8_t smb, int my_port, int msg_type, msg_lvl2_t* msg_in);

void test_send_callback(const int rc, msg_lvl2_t* msg, void* context);

void test_recv_callback(msg_lvl2_t* msg, void* context);

#ifdef TEST_ERRORS
int test_err(int my_port, int err_type);
#endif

void test_print_time();

#endif /* COMMON_H_ */
