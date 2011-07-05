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

#include <linux/version.h>
#include <linux/platform_device.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/input.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/slab.h>

#include <asm/io.h>

#include "fttsc010.h"

#define SAMPLES_PER_SEC	10

#define INPUT_CLK	33000000	/* Main clock oscillator on A369 */
#define ADC_MAIN_CLK	2000000	/* must < 2.8 MHz */
#define ADC_SLOT_SIZE	0x10	/* must > 14 */

#define PRESSURE_MAX	1000

struct fttsc010 {
	struct input_dev *input;
	struct resource *res;
	void *base;
	int irq;
};

/******************************************************************************
 * internal functions
 *****************************************************************************/
static void fttsc010_enable(struct fttsc010 *fttsc010)
{
	struct device *dev = &fttsc010->input->dev;
	int tmp;
	int reg;

	/*
	 * ADC clock and sampling rate configuration register
	 *
	 * FTTSC010_ADC_CLK_ADC determines the main clock.
	 * The main clock is used as input clock of ADC which must smaller
	 * than 2.8MHz.
	 * FTTSC010_ADC_CLK_SAMPLE determines the sample time slot size.
	 * The ADC needs more than 14 clocks per time slot.
	 * That means FTTSC010_ADC_CLK_SAMPLE must be greater than 14.
	 */
	tmp = INPUT_CLK / 2 / ADC_MAIN_CLK;

	reg = FTTSC010_ADC_CLK_ADC(tmp)
	    | FTTSC010_ADC_CLK_SAMPLE(ADC_SLOT_SIZE - 1);

	dev_info(dev, "[ADC CLK]   = %08x\n", reg);
	iowrite32(reg, fttsc010->base + FTTSC010_OFFSET_ADC_CLK);

	/*
	 * ADC sampling delay control register
	 *
	 * This determines how many frames we do a sample.
	 * Each frame consists of 16 time slots.
	 * Every FTTSC010_ADC_DELAY frames, three frames are used for
	 * sampling X, Y and Z1/Z2.
	 */
	tmp = INPUT_CLK / tmp / 2;	/* main clock */
	tmp /= ADC_SLOT_SIZE;	/* slots per sec */
	tmp /= 16;		/* frames per sec */
	tmp /= SAMPLES_PER_SEC;	/* frames per sample */

	reg = FTTSC010_ADC_DELAY(tmp);

	dev_info(dev, "[ADC DELAY] = %08x\n", reg);
	iowrite32(reg, fttsc010->base + FTTSC010_OFFSET_ADC_DELAY);

	/*
	 * Enable interrupt
	 *
	 * Note: 
	 *	Sytlus interrupt of FTTSC010 on A369 is broken.
	 *	We cannot use stylus mode.
	 */
	reg = FTTSC010_INT_AUTO_PST
	    | FTTSC010_INT_UNDERRUN
	    | FTTSC010_INT_OVERRUN;

	dev_info(dev, "[MASK]      = %08x\n", reg);
	iowrite32(reg, fttsc010->base + FTTSC010_OFFSET_INT_ENABLE);

	/*
	 * Panel function control register
	 */
	reg = FTTSC010_PANEL_AUTO_PST
	    | FTTSC010_PANEL_AUTO_ZPST
	    | FTTSC010_PANEL_DEBOUNCE_12MS;

	dev_info(dev, "[PANEL]     = %08x\n", reg);
	iowrite32(reg, fttsc010->base + FTTSC010_OFFSET_PANEL);

	/*
	 * ADC power on, Enable autoscan
	 */
	reg = FTTSC010_ADC_CTRL_XPWDB
	    | FTTSC010_ADC_CTRL_AUTOSCAN;

	dev_info(dev, "[ADC CTRL]  = %08x\n", reg);
	iowrite32(reg, fttsc010->base + FTTSC010_OFFSET_ADC_CTRL);
}

static void fttsc010_disable(struct fttsc010 *fttsc010)
{
	iowrite32(0, fttsc010->base + FTTSC010_OFFSET_ADC_CTRL);
}

static unsigned int fttsc010_eval_pressure(int x, int z1, int z2)
{
	unsigned int pr;

	if (z2 < z1 || z1 == 0)
		return 0;

	pr = ((z2 * x / z1) - x) / 16;

	if (pr > PRESSURE_MAX)
		pr = 0;

	return pr;
}

/******************************************************************************
 * interrupt handler
 *****************************************************************************/
static irqreturn_t fttsc010_interrupt(int irq, void *dev_id)
{
	struct fttsc010 *fttsc010 = dev_id;
	struct input_dev *input = fttsc010->input;
	unsigned int status;

	status = ioread32(fttsc010->base + FTTSC010_OFFSET_INT_STATUS);

	if (status & FTTSC010_INT_AUTO_PST) {
		unsigned int data;
		unsigned int x, y;
		unsigned int z1, z2;
		unsigned int pressure;

		data = ioread32(fttsc010->base + FTTSC010_OFFSET_TP_POS);
		x = FTTSC010_TP_POS_X(data);
		y = 0xfff - FTTSC010_TP_POS_Y(data);

		data = ioread32(fttsc010->base + FTTSC010_OFFSET_TP_Z);
		z1 = FTTSC010_TP_Z_Z1(data);
		z2 = FTTSC010_TP_Z_Z2(data);

		pressure = fttsc010_eval_pressure(x, z1, z2);

		if (pressure) {
			/* pen down */

			input_report_abs(input, ABS_X, x);
			input_report_abs(input, ABS_Y, y);
			input_report_abs(input, ABS_PRESSURE, pressure);

			input_report_key(input, BTN_TOUCH, 1);
			input_sync(input);

			dev_dbg(&input->dev, "(%4d, %4d), %d\n", x, y, pressure);

		} else {
			/* pen up */
			dev_dbg(&input->dev, "pen up\n");

			input_report_key(input, BTN_TOUCH, 0);
			input_report_abs(input, ABS_PRESSURE, 0);

			input_sync(input);
		}
	}

	if (status & FTTSC010_INT_UNDERRUN) {
		dev_notice(&input->dev, "fifo under-run\n");
	}

	if (status & FTTSC010_INT_OVERRUN) {
		dev_notice(&input->dev, "fifo over-run\n");
	}

	iowrite32(status, fttsc010->base + FTTSC010_OFFSET_INT_STATUS);

	return IRQ_HANDLED;
}

/******************************************************************************
 * struct platform_driver functions
 *****************************************************************************/
static int __devinit fttsc010_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct fttsc010 *fttsc010;
	struct input_dev *input;
	struct resource *res;
	int irq;
	int ret;

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res) {
		dev_err(dev, "no mmio resource defined.\n");
		return -ENXIO;
	}

	/*
	 * Note:
	 *	Sytlus interrupt of FTTSC010 on A369 is broken.
	 *	The only thing we can use is ADC interrupt.
	 */
	if ((irq = platform_get_irq(pdev, 0)) < 0) {
		ret = irq;
		goto err_get_irq;
	}

	/*
	 * Allocate driver private data
	 */
	fttsc010 = kzalloc(sizeof(struct fttsc010), GFP_KERNEL);
	if (!fttsc010) {
		dev_err(dev, "failed to allocate memory.\n");
		ret = -ENOMEM;
		goto err_alloc_priv;
	}
	platform_set_drvdata(pdev, fttsc010);

	/*
	 * Allocate input_dev
	 */
	input = input_allocate_device();
	if (!input) {
		dev_err(dev, "Failed to allocate input device.\n");
		ret = -EBUSY;
		goto err_alloc_input_dev;
	}
	fttsc010->input = input;

	/*
	 * Map io memory
	 */
	fttsc010->res = request_mem_region(res->start, res->end - res->start, dev_name(dev));
	if (!fttsc010->res) {
		dev_err(dev, "Resources is unavailable.\n");
		ret = -EBUSY;
		goto err_req_mem_region;
	}

	fttsc010->base = ioremap(res->start, res->end - res->start);
	if (!fttsc010->base) {
		dev_err(dev, "Failed to map registers.\n");
		ret = -ENOMEM;
		goto err_ioremap;
	}

	ret = request_irq(irq, fttsc010_interrupt, 0, pdev->name, fttsc010);
	if (ret < 0) {
		dev_err(dev, "Failed to request irq %d\n", irq);
		goto err_req_irq;
	}
	fttsc010->irq = irq;

	input->name = "Faraday touch screen";

	/*
	 * Declare what events and event codes can be generated
	 */
	input->evbit[0] = BIT_MASK(EV_KEY) | BIT_MASK(EV_ABS);
	input->keybit[BIT_WORD(BTN_TOUCH)] = BIT_MASK(BTN_TOUCH);

#if 0
	input_set_abs_params(input, ABS_X, 0, 0xfff, 0, 0);
	input_set_abs_params(input, ABS_Y, 0, 0xfff, 0, 0);
#else
	input_set_abs_params(input, ABS_X, 100, 3850, 0, 0);
	input_set_abs_params(input, ABS_Y, 260, 3860, 0, 0);
	input_set_abs_params(input, ABS_PRESSURE, 0, PRESSURE_MAX, 0, 0);
#endif

	fttsc010_enable(fttsc010);

	/*
	 * All went ok, so register to the input system
	 */
	ret = input_register_device(input);
	if (ret)
		goto err_register_input;

	dev_info(dev, "irq %d, mapped at %p\n", irq, fttsc010->base);
	return 0;

err_register_input:
	free_irq(irq, fttsc010);
err_req_irq:
	iounmap(fttsc010->base);
err_ioremap:
//	release_mem_region(res->start, res->end - res->start);
err_req_mem_region:
	input_free_device(fttsc010->input);
err_alloc_input_dev:
	platform_set_drvdata(pdev, fttsc010);
	kfree(fttsc010);
err_alloc_priv:
err_get_irq:
	release_resource(res);
	return ret;
}

static int __devexit fttsc010_remove(struct platform_device *pdev)
{
	struct fttsc010 *fttsc010 = platform_get_drvdata(pdev);
	struct resource *res = fttsc010->res;

	fttsc010_disable(fttsc010);

	input_unregister_device(fttsc010->input);
	free_irq(fttsc010->irq, fttsc010);
	iounmap(fttsc010->base);
//	release_mem_region(res->start, res->end - res->start);
	input_free_device(fttsc010->input);
	platform_set_drvdata(pdev, fttsc010);
	kfree(fttsc010);
	release_resource(res);

	return 0;
}

static struct platform_driver fttsc010_driver = {
	.probe		= fttsc010_probe,
	.remove		= __devexit_p(fttsc010_remove),
	.driver		= {
		.name	= "fttsc010",
	},
};

/******************************************************************************
 * initialization / finalization
 *****************************************************************************/
static int __init fttsc010_init(void)
{
	return platform_driver_register(&fttsc010_driver);
}

static void __exit fttsc010_exit(void)
{
	platform_driver_unregister(&fttsc010_driver);
}

module_init(fttsc010_init);
module_exit(fttsc010_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Faraday TouchScreen Driver");
MODULE_AUTHOR("Faraday Corp.");

