#ifndef DEVICE_H_
#define DEVICE_H_

typedef int dev_type; //types in enum in platform config

typedef struct {
    void * device;
    dev_type type;
} device_t;

unsigned device_create(void *, dev_type);

unsigned devices_init();

#endif /* DEVICE_H_ */
