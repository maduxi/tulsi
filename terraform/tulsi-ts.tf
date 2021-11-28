resource "aws_timestreamwrite_database" "tulsi-ts" {
  database_name = "tulsi"

  tags = {
    Name = "tulsi-ts"
  }
}


resource "aws_timestreamwrite_table" "status" {
  database_name = aws_timestreamwrite_database.tulsi-ts.database_name
  table_name    = "status"

  retention_properties {
    magnetic_store_retention_period_in_days = 180
    memory_store_retention_period_in_hours  = 8
  }

  tags = {
    Name = "tulsi-status-table"
  }
}