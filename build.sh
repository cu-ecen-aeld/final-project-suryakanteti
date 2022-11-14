#!/bin/sh

# The defconfig from the buildroot directory we use for qemu builds
BEAGLEBONE_DEFCONFIG=configs/beaglebone_defconfig
# The place we store customizations to the qemu configuration
MODIFIED_BEAGLEBONE_DEFCONFIG=base_external/configs/finalproject_config
# The defconfig from the buildroot directory we use for the project
DEFAULT_DEFCONFIG=${BEAGLEBONE_DEFCONFIG}
MODIFIED_DEFCONFIG=${MODIFIED_BEAGLEBONE_DEFCONFIG}
MODIFIED_DEFCONFIG_REL_BUILDROOT=../${MODIFIED_DEFCONFIG}

EXTERNAL_REL_BUILDROOT=../base_external
git submodule init
git submodule sync
git submodule update

set -e 
cd `dirname $0`

if [ ! -e buildroot/.config ]
then
	echo "MISSING BUILDROOT CONFIGURATION FILE"

	if [ -e ${MODIFIED_DEFCONFIG} ]
	then
		echo "USING ${AESD_MODIFIED_DEFCONFIG}"
		make -C buildroot defconfig BR2_EXTERNAL=${EXTERNAL_REL_BUILDROOT} BR2_DEFCONFIG=${MODIFIED_DEFCONFIG_REL_BUILDROOT}
	else
		echo "Run ./save_config.sh to save this as the default configuration in ${AESD_MODIFIED_DEFCONFIG}"
		echo "Then add packages as needed to complete the installation, re-running ./save_config.sh as needed"
		make -C buildroot defconfig BR2_EXTERNAL=${EXTERNAL_REL_BUILDROOT} BR2_DEFCONFIG=${DEFAULT_DEFCONFIG}
	fi
else
	echo "USING EXISTING BUILDROOT CONFIG"
	echo "To force update, delete .config or make changes using make menuconfig and build again."
	make -C buildroot BR2_EXTERNAL=${EXTERNAL_REL_BUILDROOT}
	echo "Move generated sdcard.img to root of repository"
	cp ./buildroot/output/images/sdcard.img ./sdcard.img

fi
