//resource "aws_iot_topic_rule" "ts-rule" {
//  name        = "TSRule"
//  enabled     = true
//  sql         = "SELECT temperature, status FROM 'topic/test'"
//  sql_version = "2016-03-23"
//
// Waiting for feature https://github.com/hashicorp/terraform-provider-aws/issues/19904
//}

data "aws_iam_policy_document" "iot_assume_role_policy" {
  statement {
    actions = ["sts:AssumeRole"]

    principals {
      type        = "Service"
      identifiers = ["iot.amazonaws.com"]
    }
  }
}


resource "aws_iam_role" "ts-role-for-iot" {
  name               = "iot_ts_role"
  assume_role_policy = data.aws_iam_policy_document.iot_assume_role_policy.json # (not shown)

  inline_policy {
    name = "my_inline_policy"

    policy = jsonencode({
      Version = "2012-10-17"
      Statement = [
        {
          Action   = ["timestream:WriteRecords","timestream:DescribeEndpoints"]
          Effect   = "Allow"
          Resource = "*"
        }
      ]
    })
  }
}


//{
//  "temperature": 18.125,
//  "status": 0,
//  "client": "rpib",
//  "sensor": "DS18B20"
//}