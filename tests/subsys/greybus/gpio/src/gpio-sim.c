/*
 * Copyright (c) 2020 Friedt Professional Engineering Services, Inc
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <drivers/gpio/gpio_sim.h>
#include <sys/util.h>
#include <zephyr.h>

#include "test-greybus-gpio.h"

/*
 * When GPIO are simulated, this callback can be used to implement the
 * "wiring". E.g. in this test application, PIN_OUT is connected to
 * PIN_IN. When PIN_OUT is set high or low, PIN_IN must be set
 * correspondingly, as if a wire were connecting the two.
 */

static void gpio_sim_callback_handler(struct device *port,
				      struct gpio_callback *cb,
				      gpio_port_pins_t pins);

static struct gpio_callback gpio_sim_callback = {
	.handler = gpio_sim_callback_handler,
	.pin_mask = BIT(GPIO_PIN_IN) | BIT(GPIO_PIN_OUT),
};

static void gpio_sim_callback_handler(struct device *port,
				      struct gpio_callback *cb,
				      gpio_port_pins_t pins)
{
	int r;
	int val;
	uint32_t output_flags;
	uint32_t input_flags;

	__ASSERT(pins & gpio_sim_callback.pin_mask, "invalid mask: %x", pins);

	r = gpio_sim_flags_get(port, GPIO_PIN_OUT, &output_flags);
	__ASSERT(r == 0, "gpio_sim_flags_get() failed: %d", r);
	r = gpio_sim_flags_get(port, GPIO_PIN_IN, &input_flags);
	__ASSERT(r == 0, "gpio_sim_flags_get() failed: %d", r);

	if ((output_flags & GPIO_OUTPUT) && (input_flags & GPIO_INPUT)) {
		r = gpio_sim_output_get(port, GPIO_PIN_OUT);
		__ASSERT(r == 0 || r == 1, "gpio_sim_output_get() failed: %d", r);
		val = r;
		r = gpio_sim_input_set(port, GPIO_PIN_IN, val);
		__ASSERT(r == 0, "gpio_sim_input_set() failed: %d", r);

		return;
	}

	if ((output_flags == GPIO_DISCONNECTED) && (input_flags & GPIO_INPUT)) {
		if (input_flags & GPIO_PULL_UP) {
			val = 1;
		} else {
			/* either GPIO_PULL_DOWN or no input */
			val = 0;
		}

		r = gpio_sim_input_set(port, GPIO_PIN_IN, val);
		__ASSERT(r == 0, "gpio_sim_input_set() failed: %d", r);

		return;
	}
}

void gpio_sim_setup(void)
{
	struct device *dev = device_get_binding(GPIO_DEV_NAME);
	__ASSERT(dev != NULL, "Device not found");
	int rc = gpio_add_callback(dev, &gpio_sim_callback);
	__ASSERT(rc == 0, "gpio_add_callback() failed: %d", rc);
}
