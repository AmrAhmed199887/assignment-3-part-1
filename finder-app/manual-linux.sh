#!/bin/bash
# Script outline to install and build kernel.
# Author: Siddhant Jajoo.

set -e
set -u

OUTDIR=/tmp/aeld
KERNEL_REPO=git://git.kernel.org/pub/scm/linux/kernel/git/stable/linux-stable.git
KERNEL_VERSION=v6.8
BUSYBOX_VERSION=1_36_1
FINDER_APP_DIR=$(realpath $(dirname $0))
ARCH=arm64
CROSS_COMPILE=aarch64-none-linux-gnu-
SYSROOT=`aarch64-none-linux-gnu-gcc -print-sysroot`

if [ $# -lt 1 ]
then
	echo "Using default directory ${OUTDIR} for output"
else
	OUTDIR=$1
	echo "Using passed directory ${OUTDIR} for output"
fi

if [ ! -d ${OUTDIR} ]
then
echo "OUTDIR does not exist creating one named ${OUTDIR}" 
mkdir -p ${OUTDIR}
fi

if [ ! -d ${OUTDIR} ]
then
echo "cant create the directory"
(exit 1)
fi

cd "$OUTDIR"
if [ ! -d "${OUTDIR}/linux-stable" ]; then
    #Clone only if the repository does not exist.
	echo "CLONING GIT LINUX STABLE VERSION ${KERNEL_VERSION} IN ${OUTDIR}"
	git clone ${KERNEL_REPO} --depth 1 --single-branch --branch ${KERNEL_VERSION}
fi
if [ ! -e ${OUTDIR}/linux-stable/arch/${ARCH}/boot/Image ]; then
    cd linux-stable
    echo "Checking out version ${KERNEL_VERSION}"
    git checkout ${KERNEL_VERSION}

    # TODO: Add your kernel build steps here
    make ARCH=${ARCH} CROSS_COMPILE=${CROSS_COMPILE} mrproper
    make ARCH=${ARCH} CROSS_COMPILE=${CROSS_COMPILE} defconfig
    make -j4 ARCH=${ARCH} CROSS_COMPILE=${CROSS_COMPILE} all
    make ARCH=${ARCH} CROSS_COMPILE=${CROSS_COMPILE} modules
    make ARCH=${ARCH} CROSS_COMPILE=${CROSS_COMPILE} dtbs

fi

echo "Adding the Image in outdir"
cp ${OUTDIR}/linux-stable/arch/${ARCH}/boot/Image ${OUTDIR}

if [ ! -e ${OUTDIR}/Image ]
then 
echo "coudnt copy the image to OUTDIR"
(exit 1)
fi

echo "Creating the staging directory for the root filesystem"
cd "$OUTDIR"
if [ -d "${OUTDIR}/rootfs" ]
then
	echo "Deleting rootfs directory at ${OUTDIR}/rootfs and starting over"
    sudo rm  -rf ${OUTDIR}/rootfs ${OUTDIR}/initramfs.cpio.gz
fi

# TODO: Create necessary base directories
mkdir -p rootfs
cd rootfs
mkdir bin dev etc home lib lib64 proc sbin sys tmp usr var
mkdir usr/bin usr/lib usr/sbin
mkdir var/log

cd "$OUTDIR"
if [ ! -d "${OUTDIR}/busybox" ]
then
git clone git://busybox.net/busybox.git
    cd busybox
    git checkout ${BUSYBOX_VERSION}
    # TODO:  Configure busybox
else
    cd busybox
fi

# TODO: Make and install busybox
make distclean
make defconfig
make -j4 ARCH=${ARCH} CROSS_COMPILE=${CROSS_COMPILE} 
make CONFIG_PREFIX=${OUTDIR}/rootfs ARCH=${ARCH} CROSS_COMPILE=${CROSS_COMPILE} install

cd ${OUTDIR}/rootfs

echo "Library dependencies"
${CROSS_COMPILE}readelf -a bin/busybox | grep "program interpreter"
${CROSS_COMPILE}readelf -a bin/busybox | grep "Shared library"

# TODO: Add library dependencies to rootfs
cp -a ${SYSROOT}/lib/ld-linux-aarch64.so.1 lib
cp -a ${SYSROOT}/lib64/ld-2.33.so lib64
cp -a ${SYSROOT}/lib64/libm.so.6 lib64
cp -a ${SYSROOT}/lib64/libc.so.6 lib64
cp -a ${SYSROOT}/lib64/libresolv.so.2 lib64
cp -a ${SYSROOT}/lib64/libresolv-2.33.so lib64
cp -a ${SYSROOT}/lib64/libc-2.33.so lib64
cp -a ${SYSROOT}/lib64/libm-2.33.so lib64
# TODO: Make device nodes
sudo mknod -m 666 dev/null c 1 3
sudo mknod -m 600 dev/console c 5 1





# TODO: Clean and build the writer utility
cd ${FINDER_APP_DIR}
make clean
make
cp writer ${OUTDIR}/rootfs/home 
# TODO: Copy the finder related scripts and executables to the /home directory
# on the target rootfs
cp finder.sh ${OUTDIR}/rootfs/home
cp finder-test.sh ${OUTDIR}/rootfs/home
cp conf/username.txt ${OUTDIR}/rootfs/home
cp conf/assignment.txt ${OUTDIR}/rootfs/home
cp autorun-qemu.sh ${OUTDIR}/rootfs/home
# TODO: Chown the root directory
cd ${OUTDIR}/rootfs
sudo chown -R root:root *
# TODO: Create initramfs.cpio.gz
find . | cpio -H newc -ov --owner root:root > ${OUTDIR}/initramfs.cpio
cd ${OUTDIR}
gzip initramfs.cpio
