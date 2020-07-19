/*
 * Copyright (c) 2020 Friedt Professional Engineering Services, Inc
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZEPHYR_INCLUDE_GREYBUS_PLATFORM_H_
#define ZEPHYR_INCLUDE_GREYBUS_PLATFORM_H_

#ifdef __cplusplus
extern "C" {
#endif

struct device;

/**
 * @brief Map a Greybus @p cport to / from a Zephyr @ref device
 *
 * This function creates a bidirectional mapping between @p cport
 * and @p dev. And is intended to be used by Zephyr's platform-
 * independent device backend.
 *
 * @param cport the cport to be used in the mapping
 * @param dev   the device to be used in the mapping
 * @return 0 on success
 * @return -EINVAL if an argument is invalid
 * @return -ENOMEM if memory could not be allocated
 * @return -EALREADY if at least one of @p cport or @dev has already been mapped
 */
int gb_add_cport_device_mapping(unsigned int cport, struct device *dev);

/**
 * @brief Query the Greybus cport / device mapping for @p dev
 *
 * This function retrieves the cport associated with a particular
 * Zephyr @ref device.
 *
 * @param dev a pointer to the Zephyr @ref device to use for the query
 * @return the associated cport on success (which is >= 0)
 * @return -ENOENT on failure
 */
int gb_device_to_cport(struct device *dev);

/**
 * @brief Query the Greybus cport / device mapping for @p cport
 *
 * This function retrieves the @ref device associated with a
 * particular Greybus @p cport.
 *
 * @param cport the Greybus cport use for the query
 * @return a pointer to the associated Zephyr @ref device on success
 * @return NULL on failure
 */
struct device *gb_cport_to_device(unsigned int cport);

struct greybus_platform_api {
	int (*add_interface)(struct device *bus, uint16_t vendor_string_id,
            uint16_t product_string_id);
	int (*add_string)(struct device *bus, uint8_t id, const char *string_);
	int (*add_bundle)(struct device *bus, uint8_t id, BundleClass class_);
	int (*add_cport)(struct device *bus, uint8_t id, BundleClass class_, CPortProtocol protocol);
	int (*num_cports)(struct device *bus);
	int (*gen_mnfb)(struct device *bus, uint8_t **mnfb, size_t *mnfb_size);
	void (*fini)(struct device *bus);
};

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_GREYBUS_PLATFORM_H_ */
