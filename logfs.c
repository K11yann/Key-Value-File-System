/**
 * Tony Givargis
 * Copyright (C), 2023
 * University of California, Irvine
 *
 * CS 238P - Operating Systems
 * logfs.c
 */

#include <pthread.h>
#include "device.h"
#include "logfs.h"

#define WCACHE_BLOCKS 32
#define RCACHE_BLOCKS 256

/**
 * Needs:
 *   pthread_create()
 *   pthread_join()
 *   pthread_mutex_init()
 *   pthread_mutex_destroy()
 *   pthread_mutex_lock()
 *   pthread_mutex_unlock()
 *   pthread_cond_init()
 *   pthread_cond_destroy()
 *   pthread_cond_wait()
 *   pthread_cond_signal()
 */

/* research the above Needed API and design accordingly */

// 记得call这些variable的初始化函数
struct logfs{
    void * buffer; /* aligned,不要call page_size() function, call device.h中的block_size */
    void * buffer_;/* malloc/free */
    size_t head;
    size_t tail;
    size_t buffer_size;/*fixed*/
    size_t size; /* head - tail ，每次read的时候check 有没有any pending data，*/
    pthread_t worker; /* 令tail追上head，每次只同步block的整数倍*/
    pthread_mutex_t lock;
    pthread_cond_t   data_avail;
    pthread_cond_t space_avail;
    int done; /*默认是0，未完成*/
};

// worker() {
//     for(;;) {
//         load();
//         if (size > block_size) {
//             do_it();
//         }
//         unlock;

//     }
// }

/* assert(tail <= head)
assert(0 == tail%BLOCK)
assert(CAPACITY >= head) */

work() {
    mutex_lock(&lock);
    while(!logfs->done) {
        // waiting ,relingush the block
        if (size >= BLOCK) {
            cond_wait(&data_avail, &lock)
            continue; //回到while，重新examine是否可以继续
        }
        // at least one block to be written
        //src(address) = buffer+tail%buffer_size
        // dex(address) = tail destination
        device_write(buffer + (tail%buffer_size), tail, BLOCK)
        tail += BLOCK
        size -= BLOCK
        cond_signal(&space_avail);

    }
    mutex_unclock(&lock);

}

append(buf, len) {
    if ((len+head) > CAPACITY) {
        error no space
    }
    assert(len <= buffer_size);
    mutex_lock(&lock);
    for(;;) {
        if (buffer_size - size < len){
            cond_wait(&space_avail);
            continue;
        }
        break;
    }
    if () {
        memccpy(buffer + (head%buffer)size, buf, len);

    } else {
        //分成两片
    }
    head += len;
    size += len;
    cond_sig(data_avail);
    mutex_unlock(&lock);

    
}

// flush最简单的方法是call append（），然后加一些garbage，之后要恢复tail和head