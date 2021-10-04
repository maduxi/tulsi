# tulsi

## Headless RPI

1. Download, install and run [Raspberry Pi Imager](https://www.raspberrypi.org/downloads/). 
2. Install Raspbian without desktop
3. Add `wpa_supplicant.conf` on the root of the SD with this syntax:
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
4. Add empty `ssh` file on the root of the SD.


## RPI setup

`sudo apt-get update -y`

`sudo apt-get upgrade -y`

