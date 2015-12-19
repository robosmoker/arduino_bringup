A small program for the bringup of the AVR on the robosmoker board

# Getting The Software Environment Setup
I'm using Raspian, which is a quasi-debian.  In order to program the AVR, you need a patched version of avrdude.  To do this, we just rebuild the avrdude dude package.

1. Enable source repositories
`sudo sed -i -E 's/#\s?deb-src/deb-src/' /etc/apt/sources.list`

2. Update package lists
`sudo apt-get update`

3. Install needed tools/etc
`sudo apt-get install build-essential fakeroot git arduino screen`

4. Get the avrdude build dependencies
`sudo apt get build-dep avrdude`

5. Get the avrdude source
`apt-get build-dep avrdude`

6. Modify the configure scipt
`sed -i -E 's/--enable-doc/--enable-doc --enable-linuxgpio/' debian/rules`

7. Fix debian bug #745034
`sed -i -E 's/HAVE_LINUX_GPIO/HAVE_LINUXGPIO/' pindefs.h`

8. Build it
`fakeroot debian/rules binary`

9. Install it:
`sudo dpkg -i ../*.deb`

10. Configure for i2c dev
Run raspi-config, and under advanced, enable the hw i2c support:
`sudo raspi-config`

11. Reboot

10. Build/Flash AVR code
`make && sudo make ispload`

12. Interract with AVR from RPI
get with `sudo i2cget 1 0x45 <register> c`
set with `sudo i2cset 1 0x45 <rigister> <value>`

