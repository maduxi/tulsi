# tulsi

## Headless RPI

1. Download, install and run [Raspberry Pi Imager](https://www.raspberrypi.org/downloads/).
2. Install Raspbian without desktop
3. Add `wpa_supplicant.conf` on the root of the SD with this syntax: 
```shell
  ctrl_interface=DIR=/var/run/wpa_supplicant GROUP=netdev
  country=ES
  update_config=1

  network={
      ssid="MiFibra-030F-24G"
      psk="wifi password"
      key_mgmt=WPA-PSK
  }
  ```
4. Add empty `ssh` file on the root of the SD.
5. Run `sudo raspi-config`
   1. Performance Options -> Set GPU Memory to 16Mb
   2. Advance options -> Expand Filesystem
   3. Change TimeZone
   4. Update

## RPI Update
Update installed software
```shell
sudo apt-get update -y
sudo apt-get upgrade -y
```

## Docker Install
Install docker with:
```shell
curl -fsSL https://get.docker.com -o get-docker.sh
sudo sh get-docker.sh
```

Add `pi` user to the Docker group to give the rights to execute containers
```shell
sudo usermod -aG docker pi
```
Log out and in again. Check that your Docker install works with:
```shell
docker run hello-world
```

## Install docker-compose

To install docker compose run:
```shell
sudo apt-get -y install libffi-dev libssl-dev python3-dev python3 python3-pip
sudo pip3 install docker-compose # needs sudo to put it into path correctly
```

Reference: [link](https://blog.anoff.io/2020-12-install-docker-raspi/).

## Install influxDB with Chronograph

Reference [link](https://blog.anoff.io/2020-12-run-influx-on-raspi-docker-compose/)

## Git
Install git:
`sudo apt-get install git`

Clone this repo:
```shell
git clone --depth=1 --branch=main git://github.com/maduxi/tulsi.git tulsi
rm -rf ./tulsi/.git
mkdir ~/tulsi/docker/influxdb/data
```

Run the docker compose:
```shell
cd ~/tulsi/docker/compose-files/influxdb
docker-compose up -d
```

