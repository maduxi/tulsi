resource "aws_iot_policy" "iot_device_policy" {
  name = "iot_device_policy"

  # Terraform's "jsonencode" function converts a
  # Terraform expression result to valid JSON syntax.
  policy = jsonencode({
    Version = "2012-10-17"
    Statement = [
      {
        Action = [
          "iot:Connect",
          "iot:Receive",
          "iot:Publish",
          "iot:Subscribe",
          "iot:DescribeEndpoint"
        ]
        Effect   = "Allow"
        Resource = "*"
      },
    ]
  })
}

resource "aws_iot_certificate" "iot_certificate" {
  active = true
}

resource "local_file" "cert_pem" {
  sensitive_content = aws_iot_certificate.iot_certificate.certificate_pem
  filename = "${path.module}/certs/device.pem.crt"
}

resource "local_file" "cert_public_key" {
  sensitive_content = aws_iot_certificate.iot_certificate.public_key
  filename = "${path.module}/certs/public.pem.key"
}

resource "local_file" "cert_private_key" {
  sensitive_content = aws_iot_certificate.iot_certificate.private_key
  filename = "${path.module}/certs/private.pem.key"
}

resource "aws_iot_policy_attachment" "att" {
  policy = aws_iot_policy.iot_device_policy.name
  target = aws_iot_certificate.iot_certificate.arn
}

resource "aws_iot_thing_principal_attachment" "att" {
  principal = aws_iot_certificate.iot_certificate.arn
  thing     = aws_iot_thing.tulsi-rpi.name
}