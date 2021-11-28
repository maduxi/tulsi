//resource "aws_iot_topic_rule" "ts-rule" {
//  name        = "TSRule"
//  enabled     = true
//  sql         = "SELECT temperature, status FROM 'topic/test'"
//  sql_version = "2016-03-23"
//
//  sns {
//    message_format = "RAW"
//    role_arn       = aws_iam_role.role.arn
//    target_arn     = aws_sns_topic.mytopic.arn
//  }
//
//  error_action {
//    sns {
//      message_format = "RAW"
//      role_arn       = aws_iam_role.role.arn
//      target_arn     = aws_sns_topic.myerrortopic.arn
//    }
//  }
//}
//{
//  "temperature": 18.125,
//  "status": 0,
//  "client": "rpib",
//  "sensor": "DS18B20"
//}