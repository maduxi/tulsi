resource "aws_iot_thing" "tulsi-rpi" {
  name = "tulsi-rpi"
}

resource "aws_iot_thing" "studio-esp32" {
  name = "studio-esp32"
}