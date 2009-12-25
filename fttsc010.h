/*
 * Faraday FTTSC010 Touch Screen Controller
 *
 * (C) Copyright 2009 Faraday Technology
 * Po-Yu Chuang <ratbert@faraday-tech.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef __FTTSC010_H
#define __FTTSC010_H

#define FTTSC010_OFFSET_ADC_CTRL	0x00
#define FTTSC010_OFFSET_INT_STATUS	0x04
#define FTTSC010_OFFSET_INT_ENABLE	0x08
#define FTTSC010_OFFSET_VERSION		0x0c
#define FTTSC010_OFFSET_ADC_DATA0	0x10
#define FTTSC010_OFFSET_ADC_DATA1	0x14
#define FTTSC010_OFFSET_ADC_DATA2	0x18
#define FTTSC010_OFFSET_ADC_DATA3	0x1c
#define FTTSC010_OFFSET_ADC_DATA4	0x20
#define FTTSC010_OFFSET_ADC_DATA5	0x24
#define FTTSC010_OFFSET_ADC_DATA6	0x28
#define FTTSC010_OFFSET_ADC_DATA7	0x2c
#define FTTSC010_OFFSET_ADC_CLK		0x30
#define FTTSC010_OFFSET_PANEL		0x34
#define FTTSC010_OFFSET_ADC_DELAY	0x38
#define FTTSC010_OFFSET_TP_POS		0x3c
#define FTTSC010_OFFSET_DMA_CTRL	0x40
#define FTTSC010_OFFSET_DMA_DATA	0x44
#define FTTSC010_OFFSET_TP_ZREF		0x48
#define FTTSC010_OFFSET_TP_Z		0x4c
#define FTTSC010_OFFSET_ADC_CAT0	0x50
#define FTTSC010_OFFSET_ADC_CAT1	0x54
#define FTTSC010_OFFSET_MVFC		0x58
#define FTTSC010_OFFSET_AVFC		0x5c
#define FTTSC010_OFFSET_CH0TC		0x60
#define FTTSC010_OFFSET_CH1TC		0x64
#define FTTSC010_OFFSET_CH2TC		0x68
#define FTTSC010_OFFSET_CH3TC		0x6c

/*
 * ADC control register
 */
#define FTTSC010_ADC_CTRL_XSELMUX(x)	(((x) & 0xf) << 0)
#define FTTSC010_ADC_CTRL_XPWDB		(1 << 8)
#define FTTSC010_ADC_CTRL_AUTO_XPWDB	(1 << 9)
#define FTTSC010_ADC_CTRL_CH0DELAY_EN	(1 << 16)
#define FTTSC010_ADC_CTRL_CH1DELAY_EN	(1 << 17)
#define FTTSC010_ADC_CTRL_CH2DELAY_EN	(1 << 18)
#define FTTSC010_ADC_CTRL_CH3DELAY_EN	(1 << 19)
#define FTTSC010_ADC_CTRL_CH4DELAY_EN	(1 << 20)
#define FTTSC010_ADC_CTRL_CH5DELAY_EN	(1 << 21)
#define FTTSC010_ADC_CTRL_CH6DELAY_EN	(1 << 22)
#define FTTSC010_ADC_CTRL_CH7DELAY_EN	(1 << 23)
#define FTTSC010_ADC_CTRL_CH3GAIN(x)	(((x) & 0x3f) << 24)
#define FTTSC010_ADC_CTRL_READ_START	(1 << 30)
#define FTTSC010_ADC_CTRL_AUTOSCAN	(1 << 31)

/*
 * Interrupt status/mask register
 */
#define FTTSC010_INT_CH0_EOC		(1 << 0)
#define FTTSC010_INT_CH1_EOC		(1 << 1)
#define FTTSC010_INT_CH2_EOC		(1 << 2)
#define FTTSC010_INT_CH3_EOC		(1 << 3)
#define FTTSC010_INT_CH4_EOC		(1 << 4)
#define FTTSC010_INT_CH5_EOC		(1 << 5)
#define FTTSC010_INT_CH6_EOC		(1 << 6)
#define FTTSC010_INT_CH7_EOC		(1 << 7)
#define FTTSC010_INT_STYLUS_DOWN	(1 << 8)
#define FTTSC010_INT_STYLUS_UP		(1 << 9)
#define FTTSC010_INT_AUTO_PST		(1 << 10)
#define FTTSC010_INT_UNDERRUN		(1 << 11)
#define FTTSC010_INT_OVERRUN		(1 << 12)
#define FTTSC010_INT_CH0_LTH		(1 << 16)
#define FTTSC010_INT_CH1_LTH		(1 << 17)
#define FTTSC010_INT_CH2_LTH		(1 << 18)
#define FTTSC010_INT_CH3_LTH		(1 << 19)
#define FTTSC010_INT_CH0_HTH		(1 << 20)
#define FTTSC010_INT_CH1_HTH		(1 << 21)
#define FTTSC010_INT_CH2_HTH		(1 << 22)
#define FTTSC010_INT_CH3_HTH		(1 << 23)

/*
 * ADC conversion data 0/1/2/3/4/5/6/7 register
 */
#define FTTSC010_ADC_DATA_MASK	0xfff

/*
 * ADC clock and sampling rate configuration register
 */
#define FTTSC010_ADC_CLK_ADC(x)		(((x) & 0xff) << 0)
#define FTTSC010_ADC_CLK_SAMPLE(x)	(((x) & 0xff) << 8)

/*
 * Panel function control register
 */
#define FTTSC010_PANEL_SWXN			(1 << 0)
#define FTTSC010_PANEL_SWXP			(1 << 1)
#define FTTSC010_PANEL_SWYN			(1 << 2)
#define FTTSC010_PANEL_SWYP			(1 << 3)
#define FTTSC010_PANEL_STYLUS_UPDOWN		(1 << 4)
#define FTTSC010_PANEL_STYLUS_MODE		(1 << 5)
#define FTTSC010_PANEL_AUTO_PST			(1 << 6)
#define FTTSC010_PANEL_PDW			(1 << 7)
#define FTTSC010_PANEL_ENIRQ			(1 << 8)
#define FTTSC010_PANEL_AUTO_ZPST		(1 << 9)
#define FTTSC010_PANEL_AUTO_PWD			(1 << 10)
#define FTTSC010_PANEL_AUTO_DISCHG		(1 << 11)
#define FTTSC010_PANEL_DEBOUNCE_2MS		(0x0 << 12)
#define FTTSC010_PANEL_DEBOUNCE_315MS		(0x1 << 12)
#define FTTSC010_PANEL_DEBOUNCE_159MS		(0x2 << 12)
#define FTTSC010_PANEL_DEBOUNCE_81MS		(0x3 << 12)
#define FTTSC010_PANEL_DEBOUNCE_42MS		(0x4 << 12)
#define FTTSC010_PANEL_DEBOUNCE_22MS		(0x5 << 12)
#define FTTSC010_PANEL_DEBOUNCE_12MS		(0x6 << 12)
#define FTTSC010_PANEL_DEBOUNCE_4MS		(0x7 << 12)
#define FTTSC010_PANEL_PWD_DEBOUNCE		(1 << 15)
#define FTTSC010_PANEL_PRECHG_DELAY(x)		(((x) & 0xf) << 16)
#define FTTSC010_PANEL_DRIVER_TYPE		(1 << 20)
#define FTTSC010_PANEL_SENSE_DELAY_1023MS	(0x0 << 24)
#define FTTSC010_PANEL_SENSE_DELAY_8MS		(0x1 << 24)
#define FTTSC010_PANEL_SENSE_DELAY_16MS		(0x2 << 24)
#define FTTSC010_PANEL_SENSE_DELAY_32MS		(0x3 << 24)
#define FTTSC010_PANEL_SENSE_DELAY_64MS		(0x4 << 24)
#define FTTSC010_PANEL_SENSE_DELAY_128MS	(0x5 << 24)
#define FTTSC010_PANEL_SENSE_DELAY_256MS	(0x6 << 24)
#define FTTSC010_PANEL_SENSE_DELAY_512MS	(0x7 << 24)
#define FTTSC010_PANEL_PEN_DISCHG_OFF		(1 << 27)
#define FTTSC010_PANEL_PWD_DELAY_0MS		(0x0 << 28)
#define FTTSC010_PANEL_PWD_DELAY_8MS		(0x1 << 28)
#define FTTSC010_PANEL_PWD_DELAY_16MS		(0x2 << 28)
#define FTTSC010_PANEL_PWD_DELAY_32MS		(0x3 << 28)
#define FTTSC010_PANEL_PWD_DELAY_64MS		(0x4 << 28)
#define FTTSC010_PANEL_PWD_DELAY_128MS		(0x5 << 28)
#define FTTSC010_PANEL_PWD_DELAY_256MS		(0x6 << 28)
#define FTTSC010_PANEL_PWD_DELAY_512MS		(0x7 << 28)
#define FTTSC010_PANEL_AUTO_DEBPWD		(1 << 31)

/*
 * ADC sampling delay control register
 */
#define FTTSC010_ADC_DELAY_MASK		0xffff

/*
 * Touch screen position data register
 */
#define FTTSC010_TP_POS_X(tp)	(((tp) >> 0) & 0xfff)
#define FTTSC010_TP_POS_Y(tp)	(((tp) >> 16) & 0xfff)

/*
 * DMA function control register
 */
#define FTTSC010_DMA_CTRL_CHSEL(x)	(((x) & 0x7) << 0)
#define FTTSC010_DMA_CTRL_ENABLE	(1 << 3)
#define FTTSC010_DMA_CTRL_FIFOCNT(ctrl)	(((ctrl) >> 8) & 0x1f)
#define FTTSC010_DMA_CTRL_FIFO_HFULL	(1 << 13)
#define FTTSC010_DMA_CTRL_FIFO_EMPTY	(1 << 14)
#define FTTSC010_DMA_CTRL_FIFO_FULL	(1 << 15)
#define FTTSC010_DMA_CTRL_DELAY(x)	(((x) & 0xffff) << 16)

/*
 * DMA data output port
 */
#define FTTSC010_DMA_DATA0(data)	(((data) >> 0) & 0xfff)
#define FTTSC010_DMA_DATA1(data)	(((data) >> 16) & 0xfff)

/*
 * Pressure measurement reference conversion data register
 */
#define FTTSC010_TP_ZREF_XN(pmrcd)	(((pmrcd) >> 0) & 0xfff)
#define FTTSC010_TP_ZREF_YP(pmrcd)	(((pmrcd) >> 16) & 0xfff)

/*
 * Pressure measurement position conversion data register
 */
#define FTTSC010_TP_Z_Z1(pmpcd)		(((pmpcd) >> 0) & 0xfff)
#define FTTSC010_TP_Z_Z2(pmpcd)		(((pmpcd) >> 16) & 0xfff)

/*
 * Channel assignment table for time slot 0 to 7 register
 */
#define FTTSC010_ADC_CAT0_TS0(x)	(((x) & 0xf) << 0)
#define FTTSC010_ADC_CAT0_TS1(x)	(((x) & 0xf) << 4)
#define FTTSC010_ADC_CAT0_TS2(x)	(((x) & 0xf) << 8)
#define FTTSC010_ADC_CAT0_TS3(x)	(((x) & 0xf) << 12)
#define FTTSC010_ADC_CAT0_TS4(x)	(((x) & 0xf) << 16)
#define FTTSC010_ADC_CAT0_TS5(x)	(((x) & 0xf) << 20)
#define FTTSC010_ADC_CAT0_TS6(x)	(((x) & 0xf) << 24)
#define FTTSC010_ADC_CAT0_TS7(x)	(((x) & 0xf) << 28)

/*
 * Channel assignment table for time slot 8 to 15 register
 */
#define FTTSC010_ADC_CAT1_TS8(x)	(((x) & 0xf) << 0)
#define FTTSC010_ADC_CAT1_TS9(x)	(((x) & 0xf) << 4)
#define FTTSC010_ADC_CAT1_TS10(x)	(((x) & 0xf) << 8)
#define FTTSC010_ADC_CAT1_TS11(x)	(((x) & 0xf) << 12)
#define FTTSC010_ADC_CAT1_TS12(x)	(((x) & 0xf) << 16)
#define FTTSC010_ADC_CAT1_TS13(x)	(((x) & 0xf) << 20)
#define FTTSC010_ADC_CAT1_TS14(x)	(((x) & 0xf) << 24)
#define FTTSC010_ADC_CAT1_TS15(x)	(((x) & 0xf) << 28)

/*
 * Median value filter configuration register
 */
#define FTTSC010_MVFC_CH0_MODE(x)	(((x) & 0x7) << 0)
#define FTTSC010_MVFC_CH1_MODE(x)	(((x) & 0x7) << 4)
#define FTTSC010_MVFC_CH2_MODE(x)	(((x) & 0x7) << 8)
#define FTTSC010_MVFC_CH3_MODE(x)	(((x) & 0x7) << 12)
#define FTTSC010_MVFC_CH4567_MODE(x)	(((x) & 0x7) << 16)

/*
 * Averaging value filter configuration register
 */
#define FTTSC010_AVFC_CH0_MODE(x)	(((x) & 0x7) << 0)
#define FTTSC010_AVFC_CH1_MODE(x)	(((x) & 0x7) << 4)
#define FTTSC010_AVFC_CH2_MODE(x)	(((x) & 0x7) << 8)
#define FTTSC010_AVFC_CH3_MODE(x)	(((x) & 0x7) << 12)
#define FTTSC010_AVFC_CH4567_MODE(x)	(((x) & 0x7) << 16)

/*
 * Channel 0/1/2/3 threshold configuration register
 */
#define FTTSC010_CHTC_LTH(ch0tc)	(((tc) >> 0) & 0xfff)
#define FTTSC010_CHTC_HTH(ch0tc)	(((tc) >> 16) & 0xfff)
#define FTTSC010_CHTC_DIS		(1 << 31)

#endif	/* __FTTSC010_H */
