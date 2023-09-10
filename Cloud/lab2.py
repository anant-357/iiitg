import boto3
from time import sleep


def create_instance(startup_script):
    try:
        ec2 = boto3.client("ec2", region_name="ap-south-1")

        instance = ec2.run_instances(
            ImageId="ami-0da59f1af71ea4ad2",
            MaxCount=1,
            MinCount=1,
            InstanceType="t2.micro",
            KeyName="cc_kp",
            SecurityGroups=["launch-wizard-1"],
            UserData=startup_script,
            IamInstanceProfile={
                "Arn": "arn:aws:iam::669000163710:instance-profile/t2-micro-role"
            },
        )
        state = instance["Instances"][0]["State"]["Name"]
        while state != "running":
            sleep(3)
            data = ec2.describe_instances(
                InstanceIds=[
                    str(instance["Instances"][0]["InstanceId"]),
                ],
            )
            state = data["Reservations"][0]["Instances"][0]["State"]["Name"]

        instance = ec2.describe_instances(
            InstanceIds=[
                str(instance["Instances"][0]["InstanceId"]),
            ],
        )

        return instance["Reservations"][0]["Instances"][0]["PublicDnsName"]

    except Exception as e:
        print(e)


if __name__ == "__main__":
    startup_script = """#!/bin/bash
    yum update -y
    yum install httpd -y
    systemctl start httpd.service
    systemctl enable httpd
    usermod -a -G apache ec2-user
    chown -R ec2-user:apache /var/www
    chmod 2775 /var/www
    aws s3 sync s3://anant-cclab-bucket/website /var/www/html/
    systemctl restart httpd"""
    public_dns = create_instance(startup_script)
    print(f"Public DNS: {public_dns}")
