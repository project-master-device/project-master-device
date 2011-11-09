// 9.11.2010
// 21.11.2010

//TODO: licence - MIT

#include "can_socket.h"

#ifdef CANSOCKET_LOG

#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

void info_print_frame(struct can_frame* frame) {
//	printf("	sizeof(frame)=%u;\n", sizeof(*frame));0
	printf("	frame.can_id=%u;\n", frame->can_id);
//	printf("	sizeof(frame.can_id)=%u;\n", sizeof(frame->can_id));
	printf("	hex:-");
	int i;
	for (i=0; i<frame->can_dlc; i++) {
		printf("-%X-", frame->data[i]);
	}
	printf("-\n	sizeof(frame.data)=%u;\n", sizeof(frame->data));
//	printf("	frame.can_dlc=%u;\n", frame->can_dlc);
//	printf("	sizeof(frame.can_dlc)=%u;\n", sizeof(frame->can_dlc));
	return;
}

void info_print_thread(void) {
	printf("INFO can_socket module: PID of this process: %d\n", getpid());
	printf("INFO can_socket module: The ID of this thread is: %u\n", (unsigned int)pthread_self());
	return;
}

#endif // CANSOCKET_LOG

static PyObject* can_init_socket(PyObject *self, PyObject *args) {
	char* device = NULL;
	if(!PyArg_ParseTuple(args, "s", &device))
		return Py_BuildValue("i", -1);
	
	int can_socket;
	struct sockaddr_can addr;
	struct ifreq ifr;
	
	can_socket = socket(PF_CAN, SOCK_RAW, CAN_RAW);
	
	strcpy(ifr.ifr_name, device);
	ioctl(can_socket, SIOCGIFINDEX, &ifr);
	
	addr.can_family = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;
	
	bind(can_socket, (struct sockaddr *)&addr, sizeof(addr));
	
	// make it non-blocking:
	fcntl(can_socket, F_SETFL, O_NONBLOCK);

#ifdef CAN_RECIEVE_ERRORS
// don't use - err frames support is not implemented yet
	can_err_mask_t err_mask = ( CAN_ERR_TX_TIMEOUT | CAN_ERR_BUSOFF );
	setsockopt(can_socket, SOL_CAN_RAW, CAN_RAW_ERR_FILTER, &err_mask, sizeof(err_mask));
#endif
	
#ifndef CANSOCKET_LOOPBACK
// turn off loopback sending
	int loopback = 0; /* 0 = disabled, 1 = enabled */
	setsockopt(can_socket, SOL_CAN_RAW, CAN_RAW_LOOPBACK, &loopback, sizeof(loopback));
#endif // CANSOCKET_LOOPBACK

	return Py_BuildValue("i", can_socket);
}


static PyObject* can_recieve_some_data(PyObject *self, PyObject *args) {
	static char can_incoming_buffer[16];
	static unsigned can_incoming_buffer_len = 0;

	unsigned int can_socket = 0;
	if(!PyArg_ParseTuple(args, "I", &can_socket))
		return Py_BuildValue("i", -1);

	// just full frame in buffer, nothing more:
	unsigned bytes_left = sizeof(struct can_frame) - can_incoming_buffer_len;
	int rbytes = read(can_socket, &can_incoming_buffer[can_incoming_buffer_len], bytes_left);

	if (rbytes > 0) {
		can_incoming_buffer_len += rbytes;
		if (can_incoming_buffer_len == sizeof(struct can_frame)) {
			can_incoming_buffer_len = 0;
			struct can_frame frame;
			memcpy(&frame, &can_incoming_buffer, sizeof(struct can_frame));
			
			#ifdef CANSOCKET_LOG
			printf("INFO can_socket module: recieved frame:\n");
			info_print_frame(&frame);
			#endif // CANSOCKET_LOG
			
			int int_len = (int)frame.can_dlc;
			return Py_BuildValue("s#", frame.data, int_len);
		}
	}
	
	// None if full frame not recieved:
	Py_INCREF(Py_None);
	return Py_None;
}


static PyObject* can_send_frame(PyObject *self, PyObject *args) {
	unsigned int can_socket = 0;
	char* frame_data = NULL;
	unsigned int frame_can_id = 0;
	unsigned int frame_data_len_i = 0;
	if(!PyArg_ParseTuple(args, "Is#I", &can_socket, &frame_data, &frame_data_len_i, &frame_can_id)
		 || frame_data_len_i > 8) {
		return Py_BuildValue("i", -1);
	}

	struct can_frame frame;

	frame.can_id = (__u32)frame_can_id;
	frame.can_dlc = (__u8)frame_data_len_i;
	memcpy(frame.data, frame_data, frame.can_dlc);

#ifdef CANSOCKET_LOG
	printf("INFO can_socket module: sending frame :\n");
	info_print_frame(&frame);
#endif // CANSOCKET_LOG

	int bytes_sent = write(can_socket, &frame, sizeof(frame));

	return Py_BuildValue("i", bytes_sent);
}


static PyMethodDef CANsocketMethods[] = {
	{"init_socket", can_init_socket, METH_VARARGS, "initialize can socket."},
	{"recieve_some_data", can_recieve_some_data, METH_VARARGS, "recieve data from recieved frame."},
	{"send_frame", can_send_frame, METH_VARARGS, "send frame through socket."},
	{NULL, NULL, 0, NULL}	/* Sentinel */
};

PyMODINIT_FUNC initcan_socket(void) {
	(void) Py_InitModule("can_socket", CANsocketMethods);
}
