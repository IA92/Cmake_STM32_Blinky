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
    set_bit(RCC->APB1ENR1, RCC_APB1ENR1_PWREN);
    /* Set Range 1 (VOS_0) up to 80 MHz, while Range 2 (VOS_1) is up to 26MHz */
    modify_reg(PWR->CR1, PWR_CR1_VOS, system_voltage_regulator_range << PWR_CR1_VOS_Pos);

    /*Wait till VOSF is ready*/
    while (read_bit(PWR->SR2, PWR_SR2_VOSF) == PWR_SR2_VOSF)
    {
    }
}

void system_backup_domain_access_enable(void)
{
    /**Enable access to the backup domain (Ref: Ref Manual Section Power Control Page 131/1600)**/
    /*Enable power interface clock*/
    set_bit(RCC->APB1ENR1, RCC_APB1ENR1_PWREN);
    set_bit(PWR->CR1, PWR_CR1_DBP);
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

void system_msi_clock_init(uint32_t system_msi_range)
{
    /** Initializes MSI**/
    /*Enable MSI*/
    set_bit(RCC->CR, RCC_CR_MSION);

    /*Wait till MSI is ready*/
    while (read_bit(RCC->CR, RCC_CR_MSIRDY) == 0U)
    {
    }

    /*Config MSI Range*/
    set_bit(RCC->CR, RCC_CR_MSIRGSEL);                                          //Select MSI Range at RCC->CR
    modify_reg(RCC->CR, RCC_CR_MSIRANGE, system_msi_range << RCC_CR_MSIRANGE_Pos);                  //Config the MSI range
    modify_reg(RCC->ICSCR, RCC_ICSCR_MSITRIM, (0) << RCC_ICSCR_MSITRIM_Pos);    //Config the calibration value
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

    modify_reg(RCC->ICSCR, RCC_ICSCR_HSITRIM, (64U) << RCC_ICSCR_HSITRIM_Pos);      //Config the calibration value
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
    modify_reg(RCC->PLLCFGR, RCC_PLLCFGR_PLLR, (system_pll_pllr << RCC_PLLCFGR_PLLR_Pos)); //Set PLLR=2
    modify_reg(RCC->PLLCFGR, RCC_PLLCFGR_PLLP, (system_pll_pllp << RCC_PLLCFGR_PLLP_Pos)); //Set PLLP=7

    /*Enable PLL*/
    set_bit(RCC->CR, RCC_CR_PLLON);
    /* Enable PLL System Clock output. */
    SET_BIT(RCC->PLLCFGR, RCC_PLLCFGR_PLLREN);

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


/***Application specific configuration functions***/
/*  Init MSI and Init PLL, especially PLLN, for SAI application*/
/*  For SAI: MSI = 4MHz, PLLM = 1, PLLN = 86 (49.14MHz), PLLQ = 2, PLLR = 2, PLLP = 7 */
void system_pll_clock_init_for_sai(uint32_t system_pll_plln)
{
    system_msi_clock_init(SYSTEM_MSI_RANGE_4M);

    /** Initializes PLL not implemented yet**/
    /*Config PLL parameters*/
    modify_reg(RCC->PLLCFGR, RCC_PLLCFGR_PLLSRC, SYSTEM_PLL_PLLSRC_MSI << RCC_PLLCFGR_PLLSRC_Pos);   //Select MSI as PLL source
    modify_reg(RCC->PLLCFGR, RCC_PLLCFGR_PLLM, (SYSTEM_PLL_PLLM_1 << RCC_PLLCFGR_PLLM_Pos)); //Set PLLM=1
    modify_reg(RCC->PLLCFGR, RCC_PLLCFGR_PLLN, (system_pll_plln << RCC_PLLCFGR_PLLN_Pos)); //Set PLLN=86 for PLLP set to be 49.14MHz
    modify_reg(RCC->PLLCFGR, RCC_PLLCFGR_PLLQ, (SYSTEM_PLL_PLLQ_2 << RCC_PLLCFGR_PLLQ_Pos)); //Set PLLQ=2
    modify_reg(RCC->PLLCFGR, RCC_PLLCFGR_PLLR, (SYSTEM_PLL_PLLR_2 << RCC_PLLCFGR_PLLR_Pos)); //Set PLLR=2
    modify_reg(RCC->PLLCFGR, RCC_PLLCFGR_PLLP, (SYSTEM_PLL_PLLP_7 << RCC_PLLCFGR_PLLP_Pos)); //Set PLLP=7
    /*Enable PLL*/
    set_bit(RCC->CR, RCC_CR_PLLON);

    /*Wait till PLL is ready*/
    while (read_bit(RCC->CR, RCC_CR_PLLRDY) == 0U)
    {
    }

    /* Enable PLL PLLP output. */
    set_bit(RCC->PLLCFGR, RCC_PLLCFGR_PLLPEN);
}

/*Deinitialisation function for PLL for SAI*/
void system_pll_clock_deinit_for_sai(void)
{
    /*Disable PLL*/
    clear_bit(RCC->CR, RCC_CR_PLLON);

    /* Disable PLL PLLP output. */
    clear_bit(RCC->PLLCFGR, RCC_PLLCFGR_PLLPEN);
}


/*Clock select function for the RTC source clock*/
status_t system_rtc_clock_select(uint32_t rcc_rtcsel)
{
    /** Select the source lock for RTC**/
    modify_reg(RCC->BDCR, RCC_BDCR_RTCSEL, rcc_rtcsel << RCC_BDCR_RTCSEL_Pos);

    return STATUS_OK;
}

/*Initialisation system clock and peripheral clock configuration functions depending on hse and lse configurations*/
void system_clock_init(bool hse_en, bool lse_en)
{
    system_flash_init();

    system_voltage_regulator_init(SYSTEM_VOLTAGE_REGULATOR_RANGE_2);

    system_backup_domain_access_enable();

    hse_en ? system_hse_clock_enable() : system_hsi_clock_enable();

    lse_en ? system_lse_clock_enable() : system_lsi_clock_enable();

    //    system_msi_clock_init (SYSTEM_MSI_RANGE_4M);        //Configure this when PLL is needed

    //    system_pll_clock_init(SYSTEM_PLL_PLLSRC_MSI, SYSTEM_PLL_PLLM_1, SYSTEM_PLL_PLLN_40, SYSTEM_PLL_PLLQ_2, SYSTEM_PLL_PLLR_2, SYSTEM_PLL_PLLP_7);       //Configure this when PLL is needed

    /* Program the new number of wait states to the LATENCY bits in the FLASH_ACR register */
    modify_reg(FLASH->ACR, FLASH_ACR_LATENCY, FLASH_ACR_LATENCY_0WS);       //0WS <= 16, 1WS <= 32, 2WS <= 48, 3WS <= 64, 4WS <= 80

    /** Initializes the CPU, AHB and APB buses clocks**/
    /*-------------------------- Select HSE as System Clock --------------------------*/
    uint32_t system_clock_switch = hse_en ? SYSTEM_CLOCK_SWITCH_HSE : SYSTEM_CLOCK_SWITCH_HSI16;
    system_clock_switch_select(system_clock_switch);

    /*-------------------------- HCLK Configuration --------------------------*/
    modify_reg(RCC->CFGR, RCC_CFGR_HPRE, RCC_CFGR_HPRE_DIV1);
    /*-------------------------- APB1 CLK Configuration ---------------------------*/
    modify_reg(RCC->CFGR, RCC_CFGR_PPRE1, RCC_CFGR_PPRE1_DIV1);
    /*-------------------------- APB2 CLK Configuration ---------------------------*/
    modify_reg(RCC->CFGR, RCC_CFGR_PPRE2, RCC_CFGR_PPRE2_DIV1);

    /**Initialize the RCC extended peripherals clocks**/
    /* Get universal select bit*/
    uint32_t hse_select_bit = hse_en ? 0x1UL : 0x2UL;   //Select system clock if hse is enabled, else select hsi
    uint32_t lse_select_bit = lse_en ? 0x3UL : 0x1UL;   //Select lse if hse is enabled, else select lsi
    /* Configure the USART1 clock source */
    modify_reg(RCC->CCIPR, RCC_CCIPR_USART1SEL, hse_select_bit << RCC_CCIPR_USART1SEL_Pos);   //Select HSI16 16MHz
    /* Configure the USART2 clock source */
    modify_reg(RCC->CCIPR, RCC_CCIPR_USART2SEL, hse_select_bit << RCC_CCIPR_USART2SEL_Pos);     //Select HSI16 16MHz
    /* Configure the USART3 clock source */
    modify_reg(RCC->CCIPR, RCC_CCIPR_USART3SEL, hse_select_bit << RCC_CCIPR_USART3SEL_Pos);   //Select HSI16 16MHz
    /* Configure the UART4 clock source */
    modify_reg(RCC->CCIPR, RCC_CCIPR_UART4SEL, hse_select_bit << RCC_CCIPR_UART4SEL_Pos);       //Select HSI16 16MHz
    /* Configure the LPTIM1 clock source */
    modify_reg(RCC->CCIPR, RCC_CCIPR_LPTIM1SEL, lse_select_bit << RCC_CCIPR_LPTIM1SEL_Pos);     //Select LSI 32kHz
    /* Configure the LPTIM2 clock source */
    modify_reg(RCC->CCIPR, RCC_CCIPR_LPTIM2SEL, lse_select_bit << RCC_CCIPR_LPTIM2SEL_Pos);     //Select LSI 32kHz
    /* Configure the ADC1 clock source */
    modify_reg(RCC->CCIPR, RCC_CCIPR_ADCSEL, RCC_CCIPR_ADCSEL);     //Select System Clock
    /* Configure the I2C1 clock source*/
    modify_reg(RCC->CCIPR, RCC_CCIPR_I2C1SEL, hse_select_bit << RCC_CCIPR_I2C1SEL_Pos); //Select HSI16
    /* Configure the I2C2 clock source*/
    modify_reg(RCC->CCIPR, RCC_CCIPR_I2C2SEL, hse_select_bit << RCC_CCIPR_I2C2SEL_Pos); //Select HSI16
    /* Configure the I2C3 clock source*/
    modify_reg(RCC->CCIPR, RCC_CCIPR_I2C3SEL, hse_select_bit << RCC_CCIPR_I2C3SEL_Pos); //Select HSI16
    /* Configure the I2C4 clock source*/
    modify_reg(RCC->CCIPR2, RCC_CCIPR2_I2C4SEL, hse_select_bit << RCC_CCIPR2_I2C4SEL_Pos); //Select HSI16
    /* Configure the SAI1 clock source if PLL is enabled, else it will be automatically connected to HSI16*/
    modify_reg(RCC->CCIPR, RCC_CCIPR_SAI1SEL, RCC_CCIPR_SAI1SEL_1); //Select PLLP if PLL is enabled
    /* Configure the DFSDM clock source*/
    modify_reg(RCC->CCIPR, RCC_CCIPR_DFSDM1SEL, RCC_CCIPR_DFSDM1SEL);   //Select System Clock (No desc in Ref. Manual)
    //    /* Configure the RTC clock source to be LSE, i.e. 32.768 kHz*/        //Configure this when PLL is needed
    //    modify_reg(RCC->BDCR, RCC_BDCR_RTCSEL, RCC_BDCR_RTCSEL_0);

    /**Additional configuration**/
    hse_en ? clear_bit(RCC->CFGR, RCC_CFGR_STOPWUCK) : set_bit(RCC->CFGR, RCC_CFGR_STOPWUCK); //Choosing HSI clock when wake up from stop mode if it is enabled
}
