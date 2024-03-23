/*
 * system_config.h
 *
 *  Created on: Aug 19, 2021
 *      Author: Indri92
 */

#ifndef SYSTEM_CONFIG_H_
#define SYSTEM_CONFIG_H_

#include <stdint.h>
#include <stdbool.h>

#define set_bit(reg, bit) ((reg) |= (bit))

#define clear_bit(reg, bit) ((reg) &= ~(bit))

#define read_bit(reg, bit) ((reg) & (bit))

#define modify_reg(reg, clearmask, setmask) (reg = (((reg) & (~(clearmask))) | (setmask)))


typedef enum status_t
{
    STATUS_OK = 0x00,
    STATUS_ERROR = 0x01,
    STATUS_BUSY = 0x02,
    STATUS_TIMEOUT = 0x03
} status_t;

/***For system setup***/
typedef enum
{
    SYSTEM_CLOCK_SWITCH_MSI,
    SYSTEM_CLOCK_SWITCH_HSI16,
    SYSTEM_CLOCK_SWITCH_HSE,
    SYSTEM_CLOCK_SWITCH_PLL,
} system_clock_switch_t;

typedef enum
{
    SYSTEM_VOLTAGE_REGULATOR_RANGE_1 = 1,
    SYSTEM_VOLTAGE_REGULATOR_RANGE_2 = 2,
} system_voltage_regulator_range_t;

typedef enum
{
    SYSTEM_MSI_RANGE_100K,
    SYSTEM_MSI_RANGE_200K,
    SYSTEM_MSI_RANGE_400K,
    SYSTEM_MSI_RANGE_800K,
    SYSTEM_MSI_RANGE_1M,
    SYSTEM_MSI_RANGE_2M,
    SYSTEM_MSI_RANGE_4M,    //6, default value
    SYSTEM_MSI_RANGE_8M,
    SYSTEM_MSI_RANGE_16M,
    SYSTEM_MSI_RANGE_24M,
    SYSTEM_MSI_RANGE_32M,
    SYSTEM_MSI_RANGE_48M,
} system_msi_range_t;

typedef enum
{
    SYSTEM_PLL_PLLSRC_NONE,
    SYSTEM_PLL_PLLSRC_MSI,
    SYSTEM_PLL_PLLSRC_HSI,
    SYSTEM_PLL_PLLSRC_HSE,
} system_pll_pllsrc_t;

typedef enum
{
    SYSTEM_PLL_PLLM_1,
    SYSTEM_PLL_PLLM_2,
    SYSTEM_PLL_PLLM_3,
    SYSTEM_PLL_PLLM_4,
    SYSTEM_PLL_PLLM_5,
    SYSTEM_PLL_PLLM_6,
    SYSTEM_PLL_PLLM_7,
    SYSTEM_PLL_PLLM_8,
} system_pll_pllm_t;

/*PLLN value range is 0-127*/
typedef enum
{
    SYSTEM_PLL_PLLN_20 = 20U,   //PLL setup for SAI
    SYSTEM_PLL_PLLN_40 = 40U,
    SYSTEM_PLL_PLLN_86 = 86U,   //PLL setup for SAI
} system_pll_plln_t;

typedef enum
{
    SYSTEM_PLL_PLLQ_2,
    SYSTEM_PLL_PLLQ_4,
    SYSTEM_PLL_PLLQ_6,
    SYSTEM_PLL_PLLQ_8,
} system_pll_pllq_t;

typedef enum
{
    SYSTEM_PLL_PLLR_2,
    SYSTEM_PLL_PLLR_4,
    SYSTEM_PLL_PLLR_6,
    SYSTEM_PLL_PLLR_8,
} system_pll_pllr_t;

typedef enum
{
    SYSTEM_PLL_PLLP_7,
    SYSTEM_PLL_PLLP_17,
} system_pll_pllp_t;

/***For RTC clock source select***/
typedef enum
{
    SYSTEM_RTCSEL_NO_CLOCK,
    SYSTEM_RTCSEL_LSE,
    SYSTEM_RTCSEL_LSI,
    SYSTEM_RTCSEL_HSE_PSC_32,
} system_rtcsel_t;

/***Init functions***/
void system_flash_init(void);
void system_voltage_regulator_init(uint32_t system_voltage_regulator_range);
void system_backup_domain_access_enable(void);
void system_lse_clock_enable(void);
void system_hse_clock_enable(void);
void system_msi_clock_init(uint32_t system_msi_range);
void system_hsi_clock_enable(void);
void system_lsi_clock_enable(void);
void system_pll_clock_init(
    uint32_t system_pll_pllsrc,
    uint32_t system_pll_pllm,
    uint32_t system_pll_plln,
    uint32_t system_pll_pllq,
    uint32_t system_pll_pllr,
    uint32_t system_pll_pllp
);
void system_clock_switch_select(uint32_t system_clock_switch);

/***System clock init***/
void system_clock_init(bool hse_en, bool lse_en);

/***For PLL setup for SAI***/
void system_pll_clock_init_for_sai(uint32_t system_pll_plln);
void system_pll_clock_deinit_for_sai(void);

/***For RTC clock source select***/
/*  Note: Currently LSE is used in the initial initialisation,
 *        This function can be used to change it if necessary */
status_t system_rtc_clock_select(uint32_t system_rtcsel);

#endif /* SYSTEM_CONFIG_H_ */
