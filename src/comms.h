/*
Copyright (C) 2013 Ben Dyer

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef _COMMS_H_
#define _COMMS_H_

#include "plog/log.h"

/*
Inititalize communications -- set up USART and clear data structures.
*/
void comms_init(void);

/*
Finalize the current tick's packet and send via PDC; handle parsing of input
messages as well.
*/
void comms_tick(void);
void comms_start_transmit(void);

void comms_set_cpu_status(uint32_t cycles_used);

#define RX_BUF_LEN 512u
#define TX_BUF_LEN 256u

struct connection_t {
    struct fcs_log_t in_log;
    struct fcs_log_t out_log;

    uint8_t tx_buf[TX_BUF_LEN];

    volatile uint8_t rx_buf[RX_BUF_LEN];
    uint16_t rx_buf_idx;

    uint8_t rx_msg[FCS_LOG_SERIALIZED_LENGTH];
    uint16_t rx_msg_idx;

    uint16_t last_rx_packet_tick;
    uint16_t last_tx_packet_tick;

	uint32_t rx_packets;
	uint32_t rx_errors;
};

#define UPDATED_ACCEL 0x01u
#define UPDATED_BARO 0x02u
#define UPDATED_MAG 0x04u
#define UPDATED_GPS 0x08u
#define UPDATED_PITOT 0x10u

struct sensor_status_t {
    uint32_t updated;

    uint32_t accel_count;
    uint32_t baro_count;
    uint32_t mag_count;
    uint32_t gps_count;
    uint32_t pitot_count;
};

extern struct connection_t cpu_conn;
extern struct connection_t gcs_conn;
extern struct sensor_status_t sensor_status;

inline static uint16_t swap_u16(uint16_t x) {
    return ((x & 0x00FFu) << 8u) | ((x & 0xFF00u) >> 8u);
}

inline static int16_t swap_i16(int16_t x) {
    return (int16_t)((((uint16_t)x & 0x00FFu) << 8u) |
                     (((uint16_t)x & 0xFF00u) >> 8u));
}

inline static uint32_t swap_u32(uint32_t x) {
    return ((x & 0x000000FFu) << 24u) | ((x & 0x0000FF00u) << 8u) |
           ((x & 0x00FF0000u) >> 8u) | ((x & 0xFF000000u) >> 24u);
}

inline static int32_t swap_i32(int32_t x) {
    return (int32_t)((((uint32_t)x & 0x000000FFu) << 24u) |
                     (((uint32_t)x & 0x0000FF00u) << 8u) |
                     (((uint32_t)x & 0x00FF0000u) >> 8u) |
                     (((uint32_t)x & 0xFF000000u) >> 24u));
}

#endif
