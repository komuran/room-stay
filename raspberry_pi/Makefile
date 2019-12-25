nexmon/install: ## install nexmon
	apt-get update && apt-get upgrade
	sudo apt install raspberrypi-kernel-headers git libgmp3-dev gawk qpdf bison flex make
	cd && git clone https://github.com/seemoo-lab/nexmon.git

support/1: ## bug support
	apt install libtool-bin automake texinfo
	cd ~/nexmon/buildtools/mpfr-3.1.4 &&\
	autoreconf -f -i

support/2: ## get build directory
	apt install dtrx
	cd && wget http://tardis.tiny-vps.com/aarm/packages/l/linux-raspberrypi-headers/linux-raspberrypi-headers-4.14.98-1-armv7h.pkg.tar.xz &&\
	dtrx linux-raspberrypi-headers-4.14.98-1-armv7h.pkg.tar.xz &&\
	cp -r linux-raspberrypi-headers-4.14.98-1-armv7h.pkg/usr/lib/modules/4.14.98-1-ARCH/build/ /usr/src/linux-headers-4.14.98-v7+/ &&\
	ln -s /usr/src/linux-headers-4.14.98-v7+/ /lib/modules/4.14.98-v7+/build

support/3: ## bug support
	apt-get update
	apt-get install ncurses-dev libncurses5-dev flex bison bc libgmp3-dev gawk qpdf
	cd && wget https://raw.githubusercontent.com/notro/rpi-source/master/rpi-source -O /usr/bin/rpi-source && sudo chmod +x /usr/bin/rpi-source && /usr/bin/rpi-source -q --tag-update
	cd /usr/bin && rpi-source

nexmon/setup1: ## setup nexmon
	cd ~/nexmon/buildtools/isl-0.10 &&\
	./configure &&\
	make && make install &&\
	ln -fs /usr/local/lib/libisl.so /usr/lib/arm-linux-gnueabihf/libisl.so.10
	cd ~/nexmon/buildtools/mpfr-3.1.4 &&\
	./configure &&\
	make && make install &&\
	ln -fs /usr/local/lib/libmpfr.so /usr/lib/arm-linux-gnueabihf/libmpfr.so.4

nexmon/setup2: ## setup nexmon
	cd ~/nexmon/patches/bcm43455c0/7_45_189/nexmon/ &&\
	make && make backup-firmware && make install-firmware
	cd ~/nexmon/utilities/nexutil/ &&\
	make && make install
	apt-get remove wpasupplicant

help: ## Display this help screen
	@grep -E '^[a-zA-Z/_-]+:.*?## .*$$' $(MAKEFILE_LIST) | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-20s\033[0m %s\n", $$1, $$2}'