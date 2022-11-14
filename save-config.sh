#!/bin/bash

MODIFIED_BEAGLEBONE_DEFCONFIG=base_external/configs/aesd_beaglebone_defconfig
MODIFIED_DEFCONFIG=${MODIFIED_BEAGLEBONE_DEFCONFIG}
MODIFIED_DEFCONFIG_REL_BUILDROOT=../${MODIFIED_DEFCONFIG}

cd `dirname $0`
mkdir -p base_external/configs/
make -C buildroot savedefconfig BR2_DEFCONFIG=${MODIFIED_DEFCONFIG_REL_BUILDROOT}

if [ -e buildroot/.config ]; then
	grep "BR2_LINUX_KERNEL_CUSTOM_CONFIG_FILE" buildroot/.config > /dev/null
	if [ $? -eq 0 ]; then
		echo "Saving linux defconfig"
		make -C buildroot linux-update-defconfig
	fi
fi
