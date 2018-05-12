/*
 * This file is part of the coreboot project.
 *
 * Copyright 2018 Intel Corp.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
 * GNU General Public License for more details.
 */

#include <arch/acpi.h>
#include <baseboard/gpio.h>
#include <baseboard/variants.h>
#include <commonlib/helpers.h>
#include <compiler.h>

/*
 * Pad configuration in ramstage. The order largely follows the 'GPIO Muxing'
 * table found in EDS vol 1, but some pins aren't grouped functionally in
 * the table so those were moved for more logical grouping.
 */
static const struct pad_config gpio_table[] = {
	/* NORTHWEST COMMUNITY GPIOS */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_0, DN_20K, DEEP, NF1, IGNORE, ENPD), /* TCK */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_1, DN_20K, DEEP, NF1, IGNORE, ENPD), /* TRST_B */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_2, UP_20K, DEEP, NF1, IGNORE, ENPU), /* TMS */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_3, UP_20K, DEEP, NF1, IGNORE, ENPU), /* TDI */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_4, UP_20K, DEEP, NF1, IGNORE, ENPU), /* TDO */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_5, UP_20K, DEEP, NF1, IGNORE, ENPU), /* JTAGX */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_6, UP_20K, DEEP, NF1, IGNORE, ENPU), /* CX_PREQ_B */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_7, UP_20K, DEEP, NF1, IGNORE, ENPU), /* CX_PRDY_B */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_8, DN_20K, DEEP, NF5, HIZCRx0, DISPUPD), /* TRACE_0_CLK_VNN */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_9, DN_20K, DEEP, NF5, HIZCRx0, DISPUPD), /* TRACE_0_DATA0_VNN */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_10, DN_20K, DEEP, NF5, HIZCRx0, DISPUPD), /* TRACE_0_DATA1_VNN */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_11, DN_20K, DEEP, NF5, HIZCRx0, DISPUPD), /* TRACE_0_DATA2_VNN */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_12, DN_20K, DEEP, NF5, HIZCRx0, DISPUPD), /* TRACE_0_DATA3_VNN */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_13, DN_20K, DEEP, NF5, HIZCRx0, DISPUPD), /* TRACE_0_DATA4_VNN */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_14, DN_20K, DEEP, NF5, HIZCRx0, DISPUPD), /* TRACE_0_DATA5_VNN */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_15, DN_20K, DEEP, NF5, HIZCRx0, DISPUPD), /* TRACE_0_DATA6_VNN */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_16, DN_20K, DEEP, NF5, HIZCRx0, DISPUPD), /* TRACE_0_DATA7_VNN */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_17, DN_20K, DEEP, NF5, HIZCRx0, DISPUPD), /* DBG_PTI_CLK_1 */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_18, DN_20K, DEEP, NF5, HIZCRx0, DISPUPD), /* DBG_PTI_DATA_8 */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_19, DN_20K, DEEP, NF5, HIZCRx0, DISPUPD), /* DBG_PTI_DATA_9 */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_20, DN_20K, DEEP, NF5, HIZCRx0, DISPUPD), /* DBG_PTI_DATA_10 */
	PAD_CFG_NF(GPIO_21, UP_20K, DEEP, NF2), /* CNV_MFUART2_RXD */
	PAD_CFG_NF_IOSSTATE(GPIO_22, UP_20K, DEEP, NF2, TxDRxE), /* CNV_MFUART2_TXD */
	PAD_CFG_NF(GPIO_23, UP_20K, DEEP, NF2), /* CNV_GNSS_PABLANKIt */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_24, DN_20K, DEEP, NF5, HIZCRx0, DISPUPD), /* TRACE_1_DATA6_VNN */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_25, DN_20K, DEEP, NF5, HIZCRx0, DISPUPD), /* TRACE_1_DATA7_VNN */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_26, DN_20K, DEEP, NF5, HIZCRx0, DISPUPD), /* TRACE_2_CLK_VNN */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_27, DN_20K, DEEP, NF5, HIZCRx0, DISPUPD), /* TRACE_2_DATA0_VNN */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_28, DN_20K, DEEP, NF5, HIZCRx0, DISPUPD), /* TRACE_2_DATA1_VNN 0*/
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_29, DN_20K, DEEP, NF5, HIZCRx0, DISPUPD), /* TRACE_2_DATA2_VNN */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_30, DN_20K, DEEP, NF5, HIZCRx0, DISPUPD), /* TRACE_2_DATA3_VNN */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_31, DN_20K, DEEP, NF5, HIZCRx0, DISPUPD), /* TRACE_2_DATA4_VNN */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_32, DN_20K, DEEP, NF5, HIZCRx0, DISPUPD), /* TRACE_2_DATA5_VNN */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_33, DN_20K, DEEP, NF5, HIZCRx0, DISPUPD), /* TRACE_2_DATA6_VNN */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_34, DN_20K, DEEP, NF5, HIZCRx0, DISPUPD), /* TRACE_2_DATA7_VNN */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_35, DN_20K, DEEP, NF5, HIZCRx0, DISPUPD), /* TRACE_3_CLK_VNN */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_36, DN_20K, DEEP, NF5, HIZCRx0, DISPUPD), /* TRACE_3_DATA0_VNN */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_37, DN_20K, DEEP, NF5, HIZCRx0, DISPUPD), /* TRACE_3_DATA1_VNN */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_38, DN_20K, DEEP, NF5, HIZCRx0, DISPUPD), /* TRACE_3_DATA2_VNN */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_39, DN_20K, DEEP, NF5, HIZCRx0, DISPUPD), /* TRACE_3_DATA3_VNN */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_40, DN_20K, DEEP, NF5, HIZCRx0, DISPUPD), /* TRACE_3_DATA4_VNN */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_41, DN_20K, DEEP, NF5, HIZCRx0, DISPUPD), /* TRACE_3_DATA5_VNN */
	PAD_CFG_GPO_IOSSTATE_IOSTERM(GPIO_42, 0, DEEP, DN_20K, HIZCRx1, DISPUPD), /* GP_INTD_DSI_TE1 */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_43, DN_20K, DEEP, NF1, HIZCRx0, DISPUPD), /* GP_INTD_DSI_TE2 */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_44, UP_20K, DEEP, NF1, TxDRxE, ENPU), /* USB_OC0_B */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_45, UP_20K, DEEP, NF1, TxDRxE, ENPU), /* USB_OC1_B */
	PAD_CFG_GPO_IOSSTATE_IOSTERM(GPIO_46, 0, DEEP, DN_20K, HIZCRx0, DISPUPD), /* DSI_I2C_SDA */
	PAD_CFG_GPO_IOSSTATE_IOSTERM(GPIO_47, 0, DEEP, DN_20K, HIZCRx0, DISPUPD), /* DSI_I2C_SCL */
	PAD_CFG_NF_IOSTANDBY_IGNORE(GPIO_48, NONE, DEEP, NF1), /* PMC_I2C_SDA */
	PAD_CFG_NF_IOSTANDBY_IGNORE(GPIO_49, NONE, DEEP, NF1), /* PMC_I2C_SCL */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_50, NONE, DEEP, NF1, HIZCRx1, DISPUPD), /* LPSS_I2C0_SDA */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_51, NONE, DEEP, NF1, HIZCRx1, DISPUPD), /* LPSS_I2C0_SCL */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_52, NONE, DEEP, NF1, HIZCRx1, DISPUPD), /* LPSS_I2C1_SDA */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_53, NONE, DEEP, NF1, HIZCRx1, DISPUPD), /* LPSS_I2C1_SCL */
	PAD_CFG_GPO_IOSSTATE_IOSTERM(GPIO_54, 0, DEEP, NONE, HIZCRx0, ENPD), /* LPSS_I2C2_SDA */
	PAD_CFG_GPO_IOSSTATE_IOSTERM(GPIO_55, 0, DEEP, NONE, HIZCRx0, ENPU), /* LPSS_I2C2_SCL */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_56, NONE, DEEP, NF1, HIZCRx1, DISPUPD), /* LPSS_I2C3_SDA */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_57, NONE, DEEP, NF1, HIZCRx1, DISPUPD), /* LPSS_I2C2_SCL */
	PAD_CFG_GPIO_HI_Z(GPIO_58, NONE, DEEP, HIZCRx0, DISPUPD), /* LPSS_I2C4_SDA - unused */
	PAD_CFG_GPIO_HI_Z(GPIO_59, NONE, DEEP, HIZCRx0, DISPUPD), /* LPSS_I2C4_SCL - unused */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_60, UP_20K, DEEP, NF1, HIZCRx1, DISPUPD), /* LPSS_UART0_RXD */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_61, UP_20K, DEEP, NF1, HIZCRx1, DISPUPD), /* LPSS_UART0_TXD */
	PAD_CFG_GPI_APIC_IOS(GPIO_62, UP_20K, DEEP, LEVEL, INVERT, HIZCRx1, DISPUPD), /* UART0-RTS_B */
	PAD_CFG_GPI_APIC_IOS(GPIO_63, NONE, DEEP, EDGE_SINGLE, INVERT, TxDRxE, DISPUPD), /* H1_PCH_INT_ODL */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_64, UP_20K, DEEP, NF1, HIZCRx1, DISPUPD), /* LPSS_UART2_RXD */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_65, UP_20K, DEEP, NF1, TxLASTRxE, DISPUPD), /* LPSS_UART2_TXD */
	PAD_CFG_GPO_IOSSTATE_IOSTERM(GPIO_66, 0, DEEP, NONE, HIZCRx0, DISPUPD), /* UART2-RTS_B */
	PAD_CFG_GPO_IOSSTATE_IOSTERM(GPIO_67, 0, DEEP, DN_20K, Tx0RxDCRx0, DISPUPD), /* UART2-CTS_B */
	PAD_CFG_GPI(GPIO_68, NONE, DEEP), /* DRAM_ID0 */
	PAD_CFG_GPI(GPIO_69, NONE, DEEP), /* DRAM_ID1 */
	PAD_CFG_GPI(GPIO_70, NONE, DEEP), /* DRAM_ID2 */
	PAD_CFG_GPI(GPIO_71, NONE, DEEP), /* DRAM_ID3 */
	PAD_CFG_GPO_IOSSTATE_IOSTERM(GPIO_72, 0, DEEP, NONE, HIZCRx0, DISPUPD), /* PMC_SPI_TXD */
	PAD_CFG_GPO_IOSSTATE_IOSTERM(GPIO_73, 0, DEEP, NONE, HIZCRx0, DISPUPD), /* PMC_SPI_CLK */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_74, UP_20K, DEEP, NF1, TxDRxE, ENPU), /* THERMTRIP_B */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_75, NONE, DEEP, NF1, TxDRxE, DISPUPD), /* PROCHOT_B */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_211, NONE, DEEP, NF1, HIZCRx1, DISPUPD), /* EMMC_RST_B */
	PAD_CFG_GPI_APIC_IOS(GPIO_212, NONE, DEEP, LEVEL, INVERT, HIZCRx1, DISPUPD), /* Touch Panel Int */
	PAD_CFG_GPO_IOSSTATE_IOSTERM(GPIO_213, 0, DEEP, NONE, HIZCRx0, ENPD), /* EN_PP3300_TOUCHSCREEN */
	PAD_CFG_GPI_APIC_IOS(GPIO_214, NONE, DEEP, LEVEL, INVERT, HIZCRx1, DISPUPD), /* P_SENSOR_INT_L */

	/* NORTH COMMUNITY GPIOS */

	/* svid - unused */
	PAD_CFG_GPIO_HI_Z(GPIO_76, NONE, DEEP, HIZCRx0, DISPUPD),/* SVID Alert - unused */
	PAD_CFG_GPIO_HI_Z(GPIO_77, NONE, DEEP, HIZCRx0, DISPUPD),/* SVID Data - unused */
	PAD_CFG_GPIO_HI_Z(GPIO_78, NONE, DEEP, HIZCRx0, DISPUPD),/* SVID Clk - unused */

	/* LPSS */
	PAD_CFG_NF(GPIO_79, NONE, DEEP, NF1), /* H1_SLAVE_SPI_CLK_R */
	PAD_CFG_NF(GPIO_80, NONE, DEEP, NF1), /* H1_SLAVE_SPI_CS_L_R */
	PAD_CFG_GPIO_HI_Z(GPIO_81, UP_20K, DEEP, HIZCRx0, DISPUPD), /* GPIO_81_DEBUG (Boot halt) -- MIPI60 DEBUG */
	PAD_CFG_NF(GPIO_82, NONE, DEEP, NF1), /* H1_SLAVE_SPI_MISO */
	PAD_CFG_NF(GPIO_83, NONE, DEEP, NF1), /* H1_SLAVE_SPI_MOSI_R */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_84, DN_20K, DEEP, NF1, HIZCRx0, ENPD), /* LPSS_SPI_2_CLK */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_85, DN_20K, DEEP, NF1, HIZCRx1, ENPU), /* LPSS_SPI_2_FS0 */
	PAD_CFG_GPO_IOSSTATE_IOSTERM(GPIO_86, 0, DEEP, NONE, HIZCRx0, DISPUPD), /* stest_CNTRL -- stest */
	PAD_CFG_GPIO_HI_Z(GPIO_87, NONE, DEEP, HIZCRx0, DISPUPD), /* TP_PCH_GPIO_87_PD -- stest */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_88, DN_20K, DEEP, NF1, HIZCRx0, ENPD),/* LPSS_SPI_2_RXD */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_89, DN_20K, DEEP, NF1, HIZCRx0, ENPD),/* LPSS_SPI_2_TXD */

	/* Fast SPI */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_90, DN_20K, DEEP, NF1, HIZCRx1, ENPU),/* FST_SPI_CS0_B */
	PAD_CFG_GPO_IOSSTATE_IOSTERM(GPIO_91, 0, DEEP, NONE, HIZCRx0, ENPD),/* FST_SPI_CS1_B -- SPK_PA_EN_R */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_92, NONE, DEEP, NF1, HIZCRx1, ENPD),/* FST_SPI_MOSI_IO0 */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_93, NONE, DEEP, NF1, HIZCRx1, ENPD),/* FST_SPI_MISO_IO1 */
	PAD_CFG_GPIO_HI_Z(GPIO_94, NONE, DEEP, HIZCRx0, DISPUPD),/* FST_SPI_IO2 - unused */
	PAD_CFG_GPIO_HI_Z(GPIO_95, NONE, DEEP, HIZCRx0, DISPUPD),/* FST_SPI_IO3 - unused */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_96, DN_20K, DEEP, NF1, HIZCRx0, ENPD),/* FST_SPI_CLK */

	/* PMU Signals */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_98, NONE, DEEP, NF1, HIZCRx1, DISPUPD),/* PMU_PLTRST_B */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_99, UP_20K, DEEP, NF1, TxDRxE, ENPU),/* PMU_PWRBTN_B */
	PAD_CFG_NF_IOSTANDBY_IGNORE(GPIO_100, NONE, DEEP, NF1),/* PMU_SLP_S0_B */
	PAD_CFG_NF_IOSTANDBY_IGNORE(GPIO_101, NONE, DEEP, NF1),/* PMU_SLP_S3_B */
	PAD_CFG_NF_IOSTANDBY_IGNORE(GPIO_102, NONE, DEEP, NF1),/* PMU_SLP_S4_B */
	PAD_CFG_NF_IOSTANDBY_IGNORE(GPIO_103, NONE, DEEP, NF1),/* SUSPWRDNACK */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_104, NONE, DEEP, NF1, HIZCRx0, ENPD),/* EMMC_DNX_PWR_EN_B */
	PAD_CFG_GPO_IOSSTATE_IOSTERM(GPIO_105, 0, DEEP, NONE, HIZCRx0, ENPD),/* GPIO_105 -- TOUCHSCREEN_RST */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_106, UP_20K, DEEP, NF1, HIZCRx1, ENPU),/* PMU_BATLOW_B */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_107, NONE, DEEP, NF1, TxDRxE, DISPUPD),/* PMU_RESETBUTTON_B */
	PAD_CFG_NF_IOSTANDBY_IGNORE(GPIO_108, NONE, DEEP, NF1),/* PMU_SUSCLK */
	PAD_CFG_GPO_IOSSTATE_IOSTERM(GPIO_109, 1, DEEP, NONE, HIZCRx1, ENPU),/* SUS_STAT_B -- BT_DISABLE_L */

	/* I2C5 - Audio */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_110, NONE, DEEP, NF1, HIZCRx1, DISPUPD),/* LPSS_I2C5_SDA */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_111, NONE, DEEP, NF1, HIZCRx1, DISPUPD),/* LPSS_I2C5_SCL */

	/* I2C6 - Trackpad */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_112, NONE, DEEP, NF1, HIZCRx1, DISPUPD),/* LPSS_I2C6_SDA */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_113, NONE, DEEP, NF1, HIZCRx1, DISPUPD),/* LPSS_I2C6_SCL */

	/* I2C7 - Touchscreen */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_114, NONE, DEEP, NF1, HIZCRx1, DISPUPD),/* LPSS_I2C7_SDA */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_115, NONE, DEEP, NF1, HIZCRx1, DISPUPD),/* LPSS_I2C7_SCL */

	/* PCIE_WAKE[0:3]_B */
	PAD_CFG_GPO(GPIO_116, 1, DEEP), /* WIFI_DISABLE_L */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_117, NONE, DEEP, NF1, TxDRxE, DISPUPD),/* PCIE_WAKE1_B */
	PAD_CFG_GPIO_HI_Z(GPIO_118, NONE, DEEP, HIZCRx0, DISPUPD),/* PCIE_WAKE2_B -- unused */
	//TODO Reef uses PCIE_WAKE0 as GPI_SCI. Whats the difference?
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_119, NONE, DEEP, NF1, TxDRxE, DISPUPD),/* PCIE_WAKE3_B */

	/* PCIE_CLKREQ[0:3]_B */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_120, UP_20K, DEEP, NF1, HIZCRx1, ENPU),/* PCIE_CLKREQ0_B -- unused*/
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_121, UP_20K, DEEP, NF1, HIZCRx1, ENPU),/* PCIE_CLKREQ1_B -- unused */
	PAD_CFG_GPO_IOSSTATE_IOSTERM(GPIO_122, 0, DEEP, DN_20K, HIZCRx0, ENPD),/* PCIE_CLKREQ2_B -- EN_PP3300_WLAN */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_123, UP_20K, DEEP, NF1, TxDRxE, DISPUPD), /* PCIE_CLKREQ3_B */

	/* DDI[0:1] SDA and SCL -- unused */
	PAD_CFG_GPIO_HI_Z(GPIO_124, NONE, DEEP, HIZCRx0, DISPUPD),/* HV_DDI0_DDC_SDA -- unused */
	PAD_CFG_GPIO_HI_Z(GPIO_125, NONE, DEEP, HIZCRx0, DISPUPD),/* HV_DDI0_DDC_SCL -- unused */
	PAD_CFG_GPIO_HI_Z(GPIO_126, NONE, DEEP, HIZCRx0, DISPUPD),/* HV_DDI1_DDC_SDA -- unused */
	PAD_CFG_GPIO_HI_Z(GPIO_127, NONE, DEEP, HIZCRx0, DISPUPD),/* HV_DDI1_DDC_SCL -- unused */

	/* Panel 0 control */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_128, NONE, DEEP, NF1, HIZCRx0, ENPD),/* PANEL0_VDDEN*/
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_129, DN_20K, DEEP, NF1, HIZCRx0, ENPD),/* PANEL0_BKLTEN */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_130, DN_20K, DEEP, NF1, HIZCRx0, ENPD),/* PANEL0_BKLTCTL */

	/* Hot plug detect. */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_131, NONE, DEEP, NF1, HIZCRx1, DISPUPD),/* HV_DDI0_HPD */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_132, NONE, DEEP, NF1, HIZCRx1, DISPUPD),/* HV_DDI1_HPD */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_133, NONE, DEEP, NF1, HIZCRx1, DISPUPD),/* HV_EDP_HPD */

	// TODO Need to set HIZCRx1
	PAD_CFG_GPI(GPIO_134, NONE, DEEP),/* GPIO_134 -- SD_CD_OD */
	PAD_CFG_GPI_APIC_LOW(GPIO_135, NONE, DEEP),/* GPIO_135 -- TRACKPAD_INT1_1V8_ODL */
	PAD_CFG_GPI_APIC_IOS(GPIO_136, NONE, DEEP, LEVEL, INVERT, TxDRxE, DISPUPD),/* GPIO_136 -- PMIC_PCH_INT_ODL */
	PAD_CFG_GPI_APIC_IOS(GPIO_137, NONE, DEEP, LEVEL, INVERT, HIZCRx1, DISPUPD),/* GPIO_137 -- HP_INT_ODL */
	PAD_CFG_GPI(GPIO_138, NONE, DEEP),/* GPIO_138 -- PEN_PDCT_ODL */
	PAD_CFG_GPI_APIC_IOS(GPIO_139, NONE, DEEP, LEVEL, INVERT, HIZCRx1, DISPUPD),/* GPIO_138 -- PEN_INT_ODL */
	PAD_CFG_GPO_IOSSTATE_IOSTERM(GPIO_140, 0, DEEP, NONE, HIZCRx0, ENPD),/* GPIO_140 -- PEN_RESET */
	// TODO check if it is ok to set to GPIROUTSCI (as in Coral/Reef and others).
	// Settings here do not match table
	// Also we may be able to use eSPI WAKE# Virtual Wire instead
	PAD_CFG_GPI_SCI_IOS(GPIO_141, UP_20K, DEEP, EDGE_SINGLE, INVERT, IGNORE, SAME),/* GPIO_141 -- EC_PCH_WAKE_ODL */
	PAD_CFG_GPI_SCI_LOW(GPIO_142, NONE, DEEP, LEVEL),/* GPIO_142 -- TRACKPAD_INT2_1V8_ODL */
	PAD_CFG_GPO_IOSSTATE_IOSTERM(GPIO_143, 1, DEEP, UP_20K, HIZCRx1, ENPU),/* GPIO_143 -- LTE_SAR_ODL */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_144, NONE, DEEP, NF5, HIZCRx0, DISPUPD),/* PANEL1_VDDN */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_145, NONE, DEEP, NF5, HIZCRx0, DISPUPD),/* PANEL1_BKLTEN */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_146, NONE, DEEP, NF5, HIZCRx0, DISPUPD),/* PANEL1_BKLTCTL */
	PAD_CFG_GPO_IOSSTATE_IOSTERM(GPIO_154, 1, DEEP, UP_20K, HIZCRx1, DISPUPD),/* LPC_CLKRUNB */

	/* AUDIO COMMUNITY GPIOS*/
	PAD_CFG_GPO_IOSSTATE_IOSTERM(GPIO_156, 0, DEEP, NONE, HIZCRx0, DISPUPD), /* AVS_I2S0_MCLK */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_157, NONE, DEEP, NF1, HIZCRx0, DISPUPD), /* AVS_I2S0_BCLK */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_158, NONE, DEEP, NF1, HIZCRx0, DISPUPD), /* AVS_I2S0_WS_SYNC */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_159, NONE, DEEP, NF1, HIZCRx0, DISPUPD), /* AVS_I2S0_SDI */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_160, NONE, DEEP, NF1, HIZCRx0, ENPD), /* AVS_I2S0_SDO */
	PAD_CFG_GPO_IOSSTATE_IOSTERM(GPIO_161, 1, DEEP, UP_20K, HIZCRx0, DISPUPD), /* AVS_I2S1_MCLK -- LTE_OFF_ODL */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_162, NONE, DEEP, NF1, HIZCRx0, DISPUPD), /* AVS_I2S1_BCLK */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_163, NONE, DEEP, NF1, HIZCRx0, DISPUPD), /* AVS_I2S1_WS_SYNC */
	PAD_CFG_GPO(GPIO_164, 0, DEEP), /* WLAN_PE_RST */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_165, NONE, DEEP, NF1, HIZCRx0, DISPUPD), /* AVS_I2S1_SDO */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_166, NONE, DEEP, NF2, HIZCRx0, SAME), /* AVS_I2S2_BCLK */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_167, NONE, DEEP, NF2, HIZCRx0, DISPUPD), /* AVS_I2S2_WS_SYNC */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_168, NONE, DEEP, NF2, HIZCRx0, DISPUPD), /* AVS_I2S2_SDI */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_169, NONE, DEEP, NF2, HIZCRx0, DISPUPD), /* AVS_I2S2_SD0 */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_170, DN_20K, DEEP, NF2, HIZCRx0, DISPUPD), /* AVS_I2S1_MCLK */
	PAD_CFG_NF_IOSTANDBY_IGNORE(GPIO_171, DN_20K, DEEP, NF1), /* AVS_M_CLK_A1 -- DMIC_CLK1 */
	PAD_CFG_NF_IOSSTATE(GPIO_172, DN_20K, DEEP, NF1, HIZCRx0), /* AVS_M_CLK_B1 */
	PAD_CFG_NF_IOSTANDBY_IGNORE(GPIO_173, DN_20K, DEEP, NF1), /* AVS_M_DATA_1 -- DMIC_DATA */
	PAD_CFG_GPO_IOSSTATE_IOSTERM(GPIO_174, 0, DEEP, NONE, HIZCRx0, DISPUPD), /* AVS_M_CLK_AB2*/
	PAD_CFG_NF_IOSSTATE(GPIO_175, DN_20K, DEEP, NF1, HIZCRx0), /* AVS_M_DATA_2 */

	/* SCC COMMUNITY GPIOS */
	PAD_CFG_GPO_IOSSTATE_IOSTERM(GPIO_176, 0, DEEP, NONE, HIZCRx0, DISPUPD), /* SMB_ALERTB */
	PAD_CFG_GPO_IOSSTATE_IOSTERM(GPIO_177, 0, DEEP, NONE, HIZCRx0, DISPUPD), /* SMB_CLK */
	PAD_CFG_GPO(GPIO_178, 1, DEEP), /* EN_PP3300_WLAN */
	PAD_CFG_GPO_IOSSTATE_IOSTERM(GPIO_189, 0, DEEP, NONE, TxDRxE, DISPUPD), /* OSC_CLK_OUT_0 */
	PAD_CFG_NF_IOSTANDBY_IGNORE(GPIO_191, NONE, DEEP, NF1), /* CNV_BRI_DT */
	PAD_CFG_NF_IOSTANDBY_IGNORE(GPIO_192, UP_20K, DEEP, NF1), /* CNV_BRI_RSP */
	PAD_CFG_NF_IOSTANDBY_IGNORE(GPIO_193, NONE, DEEP, NF1), /* CNV_RGI_DT */
	PAD_CFG_NF_IOSTANDBY_IGNORE(GPIO_194, UP_20K, DEEP, NF1), /* CNV_RGI_RSP */
	PAD_CFG_NF_IOSTANDBY_IGNORE(GPIO_195, NONE, DEEP, NF1), /* CNV_RF_RESET_B */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_198, DN_20K, DEEP, NF1, HIZCRx0, ENPU), /* EMMC0_CLK */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_200, UP_20K, DEEP, NF1, HIZCRx1, ENPU), /* EMMC0_D0 */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_201, UP_20K, DEEP, NF1, HIZCRx1, ENPU), /* EMMC0_D1 */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_202, UP_20K, DEEP, NF1, HIZCRx1, ENPU), /* EMMC0_D2 */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_203, UP_20K, DEEP, NF1, HIZCRx1, ENPU), /* EMMC0_D3 */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_204, UP_20K, DEEP, NF1, HIZCRx1, ENPU), /* EMMC0_D4 */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_205, UP_20K, DEEP, NF1, HIZCRx1, ENPU), /* EMMC0_D5 */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_206, UP_20K, DEEP, NF1, HIZCRx1, ENPU), /* EMMC0_D6 */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_207, UP_20K, DEEP, NF1, HIZCRx1, ENPU), /* EMMC0_D7 */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_208, UP_20K, DEEP, NF1, HIZCRx1, ENPU), /* EMMC0_CMD */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_209, DN_20K, DEEP, NF1, HIZCRx0, ENPU), /* EMMC0_STROBE */
	PAD_CFG_GPO_IOSSTATE_IOSTERM(GPIO_210, 0, DEEP, NONE, HIZCRx0, DISPUPD),
};

const struct pad_config *__weak variant_gpio_table(size_t *num)
{
	*num = ARRAY_SIZE(gpio_table);
	return gpio_table;
}

/* GPIOs needed prior to ramstage. */
static const struct pad_config early_gpio_table[] = {
	PAD_CFG_GPI(GPIO_190, NONE, DEEP), /* PCH_WP_OD */
	/* GSPI0_INT */
	PAD_CFG_GPI_APIC_IOS(GPIO_63, NONE, DEEP, EDGE_SINGLE, INVERT, TxDRxE,
		DISPUPD), /* H1_PCH_INT_ODL */
	/* GSPI0_CLK */
	PAD_CFG_NF(GPIO_79, NONE, DEEP, NF1), /* H1_SLAVE_SPI_CLK_R */
	/* GSPI0_CS# */
	PAD_CFG_NF(GPIO_80, NONE, DEEP, NF1), /* H1_SLAVE_SPI_CS_L_R */
	/* GSPI0_MISO */
	PAD_CFG_NF(GPIO_82, NONE, DEEP, NF1), /* H1_SLAVE_SPI_MISO */
	/* GSPI0_MOSI */
	PAD_CFG_NF(GPIO_83, NONE, DEEP, NF1), /* H1_SLAVE_SPI_MOSI_R */

	/* Enable power to wifi early in bootblock and de-assert PERST#. */
	PAD_CFG_GPO(GPIO_178, 1, DEEP), /* EN_PP3300_WLAN */
	PAD_CFG_GPO(GPIO_164, 0, DEEP), /* WLAN_PE_RST */

	/*
	 * ESPI_IO1 acts as ALERT# (which is open-drain) and requies a weak
	 * pull-up for proper operation. Since there is no external pull present
	 * on this platform, configure an internal weak pull-up.
	 */
	PAD_CFG_NF_IOSSTATE_IOSTERM(GPIO_151, UP_20K, DEEP, NF2, HIZCRx1,
				    ENPU), /* ESPI_IO1 */
};

const struct pad_config *__weak
variant_early_gpio_table(size_t *num)
{
	*num = ARRAY_SIZE(early_gpio_table);
	return early_gpio_table;
}

/* GPIO settings before entering sleep. */
static const struct pad_config sleep_gpio_table[] = {
};

/* GPIO settings before entering slp_s5. */
static const struct pad_config sleep_s5_gpio_table[] = {
	/* BT_DISABLE_L */
	PAD_CFG_GPO_IOSSTATE_IOSTERM(GPIO_109, 0, DEEP, NONE, Tx0RXDCRx1, SAME),
};

const struct pad_config *__weak
variant_sleep_gpio_table(size_t *num, int slp_typ)
{
	if (slp_typ == ACPI_S5) {
		*num = ARRAY_SIZE(sleep_s5_gpio_table);
		return sleep_s5_gpio_table;
	}

	*num = ARRAY_SIZE(sleep_gpio_table);
	return sleep_gpio_table;
}

static const struct cros_gpio cros_gpios[] = {
	CROS_GPIO_REC_AL(CROS_GPIO_VIRTUAL, CROS_GPIO_DEVICE_NAME),
	CROS_GPIO_WP_AH(PAD_SCC(GPIO_PCH_WP), GPIO_COMM_SCC_NAME),
};

const struct cros_gpio *__weak variant_cros_gpios(size_t *num)
{
	*num = ARRAY_SIZE(cros_gpios);
	return cros_gpios;
}
