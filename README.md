# tulsi

## Headless RPI

1. Download, install and run [Raspberry Pi Imager](https://www.raspberrypi.org/downloads/). 
1. Install Raspbian without desktop
1. Add `wpa_supplicant.conf` on the root of the SD with this syntax:
```
ctrl_interface=DIR=/var/run/wpa_supplicant GROUP=netdev
country=ES
update_config=1

network={
    ssid="MiFibra-030F-24G"
    psk="XXX"
    key_mgmt=WPA-PSK
}
```
1. Add empty `ssh` file on the root of the SD.
1. Run `sudo raspi-config`
   1. Performance Options -> Set GPU Memory to 16Mb
   1. Advance options -> Expand Filesystem
   1. Update 

## RPI setup

`sudo apt-get update -y`

`sudo apt-get upgrade -y`

