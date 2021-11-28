resource "aws_iot_thing" "tulsi-rpi" {
  name = "tulsi-rpi"

  attributes = {
    First = "examplevalue"
  }
}