resource "aws_timestreamwrite_database" "tulsi-ts" {
  database_name = "tulsi"

  tags = {
    Name = "tulsi-ts"
  }
}
