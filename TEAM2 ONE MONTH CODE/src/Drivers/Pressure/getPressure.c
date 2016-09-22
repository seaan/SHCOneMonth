/*
 * getPressure.c
 *
 * Created: 9/22/2016 11:45:21 AM
 *  Author: seanw
 */ 

//TEMPLATE FOUND ON GITHUB, PROTOTYPING
#include <asf.h>
#include <avr32/io.h>
#include <string.h>
#include "fcsassert.h"
#include "comms.h"
#include "drivers/i2cdevice.h"
#include "ms5611.h"
#include "plog/parameter.h"

struct ms5611_read_t {
    int32_t temp; /* 1/100ths of a deg C, from -4000 to 8500 */
    int32_t p;    /* 1/100ths of an mbar, from 1000 to 120000 */
    uint8_t err;
};

static volatile uint8_t d1_buf[3], d2_buf[3], c1_buf[2], c2_buf[2], c3_buf[2],
                        c4_buf[2], c5_buf[2], c6_buf[2];
static uint32_t ms5611_last_conv_requested;

static struct twim_transaction_t init_sequence[] = {
    /* Device address, TX byte count, TX bytes (0-4), RX byte count, RX buffer */
    {MS5611_DEVICE_ADDR, 1u, {0xA2u}, 2u, c1_buf, 0},  /* READ C1: sens_t1 */
    {MS5611_DEVICE_ADDR, 1u, {0xA4u}, 2u, c2_buf, 0},  /* READ C2: off_t1 */
    {MS5611_DEVICE_ADDR, 1u, {0xA6u}, 2u, c3_buf, 0},  /* READ C3: tcs */
    {MS5611_DEVICE_ADDR, 1u, {0xA8u}, 2u, c4_buf, 0},  /* READ C4: tco */
    {MS5611_DEVICE_ADDR, 1u, {0xAAu}, 2u, c5_buf, 0},  /* READ C5: t_ref */
    {MS5611_DEVICE_ADDR, 1u, {0xACu}, 2u, c6_buf, 0},  /* READ C6: tempsens */
    TWIM_TRANSACTION_SENTINEL
};

static struct twim_transaction_t read_sequence[] = {
    {MS5611_DEVICE_ADDR, 1u, {0x52u}, 0, NULL, 0},     /* CONV D2, OSR=512 */
    {MS5611_DEVICE_ADDR, 1u, {0x00u}, 3u, d2_buf, 0},  /* ADC READ initiate */
    {MS5611_DEVICE_ADDR, 1u, {0x42u}, 0, NULL, 0},     /* CONV D1, OSR=512 */
    {MS5611_DEVICE_ADDR, 1u, {0x00u}, 3u, d1_buf, 0},  /* ADC READ initiate */
    TWIM_TRANSACTION_SENTINEL
};

static struct i2c_device_t ms5611 = {
    .speed = 250000u,
    .power_delay = 100u,
    .init_timeout = 300u,
    .read_timeout = 150u,

    .sda_pin_id = MS5611_TWI_TWD_PIN,
    .sda_function = MS5611_TWI_TWD_FUNCTION,
    .scl_pin_id = MS5611_TWI_TWCK_PIN,
    .scl_function = MS5611_TWI_TWCK_FUNCTION,
    .enable_pin_id = MS5611_ENABLE_PIN,

    .twim_cfg = {
        .twim = MS5611_TWI,
        .tx_pdca_num = PDCA_CHANNEL_MS5611_TX,
        .rx_pdca_num = PDCA_CHANNEL_MS5611_RX,
        .tx_pid = MS5611_TWI_PDCA_PID_TX,
        .rx_pid = MS5611_TWI_PDCA_PID_RX
    },

    .init_sequence = init_sequence,
    .read_sequence = read_sequence
};

/* The number of packets (ms) between temperature readings */
#define MS5611_TEMP_PERIOD 100u

static inline struct ms5611_read_t ms5611_actual_pressure_temp(uint32_t d1,
        uint32_t d2) {
    /* Perform 1st-order temperature compensation as described on page 7-8 of
       the MS5611-01BA datasheet. */
    uint16_t sens_t1, off_t1, tcs, tco, t_ref, tempsens;
    int64_t dT, temp, off, sens, p;
    struct ms5611_read_t result = { 0, 0, 1u };

    sens_t1 = c1_buf[1] + (c1_buf[0] << 8u);
    off_t1 = c2_buf[1] + (c2_buf[0] << 8u);
    tcs = c3_buf[1] + (c3_buf[0] << 8u);
    tco = c4_buf[1] + (c4_buf[0] << 8u);
    t_ref = c5_buf[1] + (c5_buf[0] << 8u);
    tempsens = c6_buf[1] + (c6_buf[0] << 8u);

    fcs_assert((d1 & 0xff000000u) == 0 && (d2 & 0xff000000u) == 0);

    dT = (int64_t)d2 - ((int64_t)t_ref * 256);
    fcs_assert(-16776960 <= dT && dT <= 16777216);

    temp = 2000 + ((dT * (int64_t)tempsens) / 8388608);
    fcs_assert(-4000 <= temp && temp <= 8500);

    off = ((int64_t)off_t1 * 65536) +
        (((int64_t)tco * dT) / 128);
    fcs_assert(-8589672450 <= off && off <= 12884705280);

    sens = ((int64_t)sens_t1 * 32768) +
        (((int64_t)tcs * dT) / 256);
    fcs_assert(-4294836225 <= sens && sens <= 6442352640);

    if (temp < 2000) {
        int64_t t2, off2, sens2;

        /* Low temperature conversion (2nd order correction) */
        t2 = (dT * dT) / 2147483648;
        off2 = (5 * (temp - 2000) * (temp - 2000)) / 2;
        sens2 = (5 * (temp - 2000) * (temp - 2000)) / 4;

        if (temp < -1500) {
            /* Very low temperature conversion */
            off2 = off2 + (7 * (temp + 1500) * (temp + 1500));
            sens2 = sens2 + ((11 * (temp + 1500) * (temp + 1500)) / 2);
        }

        temp = temp - t2;
        off = off - off2;
        sens = sens - sens2;

        fcs_assert(-4000 <= temp && temp <= 8500);
    }

    p = ((((int64_t)d1 * sens) / 2097152) - off) / 32768;
    fcs_assert(1000 <= p && p <= 120000);

    /*
    Conversion from int64_t -> int32_t is safe because we already know it's
    in the range [-4000, 8500]
    */
    result.temp = temp;
    /* p is in the range [1000, 120000] */
    result.p = p;
    result.err = 0;

    return result;
}

void ms5611_init(void) {
    i2c_device_init(&ms5611);
}

void ms5611_tick(void) {
    struct ms5611_read_t conv_result;
    enum twim_transaction_result_t result;
    struct fcs_parameter_t param;

    i2c_device_tick(&ms5611);

    if (ms5611.state != I2C_READ_SEQUENCE) {
        return;
    } else if ((ms5611.sequence_idx == 1u || ms5611.sequence_idx == 3u) &&
            ms5611.state_timer - ms5611_last_conv_requested <= 1u) {
        /* Insert a 2ms delay after each sample is requested, otherwise
           the ADC read command returns invalid results */
        return;
    }

    /*
    Attempt to read the value for the last requested sample; abort if not
    ready.
    */
    result = twim_run_sequence(&ms5611.twim_cfg, ms5611.read_sequence,
                               ms5611.sequence_idx);
    if (result != TWIM_TRANSACTION_EXECUTED) {
        return;
    }

    ms5611.sequence_idx++;

    switch (ms5611.sequence_idx) {
        case 1u:
        case 3u:
            ms5611_last_conv_requested = ms5611.state_timer;
            break;
        case 4u:
            /* Convert the result and update the comms module */
            conv_result = ms5611_actual_pressure_temp(
                d1_buf[2] + (d1_buf[1] << 8u) + (d1_buf[0] << 16u),
                d2_buf[2] + (d2_buf[1] << 8u) + (d2_buf[0] << 16u));

            if (!conv_result.err) {
                fcs_parameter_set_header(&param, FCS_VALUE_UNSIGNED, 16u, 2u);
                fcs_parameter_set_type(&param, FCS_PARAMETER_PRESSURE_TEMP);
                fcs_parameter_set_device_id(&param, 0);

				/*
				Convert temp to range [0, 12500] by adding 4000; convert
				pressure to range [500, 60000] by dividing by 2.
				*/
                param.data.u16[0] = swap_u16(conv_result.p >> 1u);
                param.data.u16[1] = swap_u16(conv_result.temp + 4000);
                (void)fcs_log_add_parameter(&cpu_conn.out_log, &param);

                sensor_status.updated |= UPDATED_BARO;
                sensor_status.baro_count++;
            } else {
                /* Something went wrong */
                fcs_assert(false);
            }

            /*
            Do pressure conversions as frequently as possible, but a temp
            conversion no more than 10 times per second -- just loop back to
            sequence idx #2, which is the D1 convert command.
            */
            if (ms5611.state_timer >= MS5611_TEMP_PERIOD) {
                i2c_device_state_transition(&ms5611, I2C_READ_SEQUENCE);
            } else {
                ms5611.sequence_idx = 2u;
            }
            break;
    }
}