/*
 * system_config.c
 *
 *  Created on: Aug 19, 2021
 *      Author: Indri92
 */

#include "stm32f401xe.h"
#include "system_config.h"

/*System flash init*/
void system_flash_init(void)
{
    set_bit(FLASH->ACR, FLASH_ACR_PRFTEN);      /* PREFETCH_ENABLE */
    set_bit(FLASH->ACR, FLASH_ACR_ICEN);        /* INSTRUCTION_CACHE_ENABLE */
    set_bit(FLASH->ACR, FLASH_ACR_DCEN);        /* DATA_CACHE_ENABLE */
}

/***Individual initialisation functions, called by the system_clock_init function***/
void system_voltage_regulator_init(uint32_t system_voltage_regulator_range)
{
    /** Configure the main internal regulator output voltage**/
    set_bit(RCC->APB1ENR, RCC_APB1ENR_PWREN);
    /* Set Range 1 (VOS_0) up to 80 MHz, while Range 2 (VOS_1) is up to 26MHz */
    modify_reg(PWR->CR, PWR_CR_VOS, system_voltage_regulator_range << PWR_CR_VOS_Pos);

    /*Wait till VOSF is ready*/
    while (read_bit(PWR->CR, PWR_CSR_VOSRDY) == PWR_CSR_VOSRDY)
    {
    }
}

void system_backup_domain_access_enable(void)
{
    /**Enable access to the backup domain (Ref: Ref Manual Section Power Control Page 131/1600)**/
    /*Enable power interface clock*/
    set_bit(RCC->APB1ENR, RCC_APB1ENR_PWREN);
    set_bit(PWR->CR, PWR_CR_DBP);
}

void system_lse_clock_enable(void)
{
    /** Initializes LSE**/
    /*Enable LSE*/
    //    set_bit(RCC->BDCR, RCC_BDCR_LSEBYP);
    if (read_bit(RCC->BDCR, RCC_BDCR_LSERDY) == 0U)
    {
        set_bit(RCC->BDCR, RCC_BDCR_LSEON);
    }

    /*Wait till LSE is ready*/
    while (read_bit(RCC->BDCR, RCC_BDCR_LSERDY) == 0U)
    {
    }
}

void system_hse_clock_enable(void)
{
    /** Initializes HSE**/
    /*Enable HSE*/
    //    set_bit(RCC->CR, RCC_CR_HSEBYP);
    set_bit(RCC->CR, RCC_CR_HSEON);

    /*Wait till HSE is ready*/
    while (read_bit(RCC->CR, RCC_CR_HSERDY) == 0U)
    {
    }
}

void system_hsi_clock_enable(void)
{
    /** Initializes HSI**/
    /*Enable HSI*/
    set_bit(RCC->CR, RCC_CR_HSION);

    /*Wait till HSI is ready*/
    while (read_bit(RCC->CR, RCC_CR_HSIRDY) == 0U)
    {
    }

    modify_reg(RCC->CR, RCC_CR_HSITRIM, (64U) << RCC_CR_HSITRIM_Pos);      //Config the calibration value
}

void system_lsi_clock_enable(void)
{
    /** Initializes LSI**/
    /*Enable LSI 32kHz*/
    set_bit(RCC->CSR, RCC_CSR_LSION);

    /*Wait till LSI is ready*/
    while (read_bit(RCC->CSR, RCC_CSR_LSIRDY) == 0U)
    {
    }
}

/*System PLL clock init*/
void system_pll_clock_init(
    uint32_t system_pll_pllsrc,
    uint32_t system_pll_pllm,
    uint32_t system_pll_plln,
    uint32_t system_pll_pllq,
    uint32_t system_pll_pllr,
    uint32_t system_pll_pllp
)
{
    /** Initializes PLL not implemented yet**/
    /*Config PLL parameters*/
    modify_reg(RCC->PLLCFGR, RCC_PLLCFGR_PLLSRC, system_pll_pllsrc << RCC_PLLCFGR_PLLSRC_Pos);   //Select MSI as PLL source
    modify_reg(RCC->PLLCFGR, RCC_PLLCFGR_PLLM, (system_pll_pllm << RCC_PLLCFGR_PLLM_Pos)); //Set PLLM=1
    modify_reg(RCC->PLLCFGR, RCC_PLLCFGR_PLLN, (system_pll_plln << RCC_PLLCFGR_PLLN_Pos)); //Set PLLN=40
    modify_reg(RCC->PLLCFGR, RCC_PLLCFGR_PLLQ, (system_pll_pllq << RCC_PLLCFGR_PLLQ_Pos)); //Set PLLQ=2
    modify_reg(RCC->PLLI2SCFGR, RCC_PLLI2SCFGR_PLLI2SR, (system_pll_pllr << RCC_PLLI2SCFGR_PLLI2SR_Pos)); //Set PLLR=2
    modify_reg(RCC->PLLCFGR, RCC_PLLCFGR_PLLP, (system_pll_pllp << RCC_PLLCFGR_PLLP_Pos)); //Set PLLP=7

    /*Enable PLL*/
    set_bit(RCC->CR, RCC_CR_PLLON);

    /*Wait till PLL is ready*/
    while (read_bit(RCC->CR, RCC_CR_PLLRDY) == 0U)
    {
    }
}

void system_clock_switch_select(uint32_t system_clock_switch)
{
    /** Initializes the CPU, AHB and APB buses clocks**/
    /*-------------------------- Select System Clock --------------------------*/
    modify_reg(RCC->CFGR, RCC_CFGR_SW, system_clock_switch << RCC_CFGR_SW_Pos);

    while (read_bit(RCC->CFGR, RCC_CFGR_SWS) != (system_clock_switch << RCC_CFGR_SWS_Pos))
    {
    }
}


/*Initialisation system clock and peripheral clock configuration functions depending on hse and lse configurations*/
void system_clock_init(bool hse_en, bool lse_en)
{
    system_flash_init();

    system_voltage_regulator_init(SYSTEM_VOLTAGE_REGULATOR_RANGE_2);

    system_backup_domain_access_enable();

    hse_en ? system_hse_clock_enable() : system_hsi_clock_enable();

    lse_en ? system_lse_clock_enable() : system_lsi_clock_enable();

    //    system_pll_clock_init(SYSTEM_PLL_PLLSRC_MSI, SYSTEM_PLL_PLLM_1, SYSTEM_PLL_PLLN_40, SYSTEM_PLL_PLLQ_2, SYSTEM_PLL_PLLR_2, SYSTEM_PLL_PLLP_7);       //Configure this when PLL is needed

    /* Program the new number of wait states to the LATENCY bits in the FLASH_ACR register */
    modify_reg(FLASH->ACR, FLASH_ACR_LATENCY, FLASH_ACR_LATENCY_0WS);       //0WS <= 16, 1WS <= 32, 2WS <= 48, 3WS <= 64, 4WS <= 80

    /** Initializes the CPU, AHB and APB buses clocks**/
    /*-------------------------- Select HSE as System Clock --------------------------*/
    uint32_t system_clock_switch = hse_en ? SYSTEM_CLOCK_SWITCH_HSE : SYSTEM_CLOCK_SWITCH_HSI;
    system_clock_switch_select(system_clock_switch);

    /*-------------------------- HCLK Configuration --------------------------*/
    modify_reg(RCC->CFGR, RCC_CFGR_HPRE, RCC_CFGR_HPRE_DIV1);
    /*-------------------------- APB1 CLK Configuration ---------------------------*/
    modify_reg(RCC->CFGR, RCC_CFGR_PPRE1, RCC_CFGR_PPRE1_DIV1);
    /*-------------------------- APB2 CLK Configuration ---------------------------*/
    modify_reg(RCC->CFGR, RCC_CFGR_PPRE2, RCC_CFGR_PPRE2_DIV1);
}
