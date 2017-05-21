#!/bin/sh
set -e
. ./build.sh

mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub

cp sysroot/boot/emilos.kernel isodir/boot/emilos.kernel
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "emilos" {
	multiboot /boot/emilos.kernel
}
EOF
grub-mkrescue -o emilos.iso isodir
