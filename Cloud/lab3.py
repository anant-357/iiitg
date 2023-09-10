import boto3
from time import sleep

def create_instances(ec2, startup_script):
    ec2.run_instances(
        ImageId="ami-0da59f1af71ea4ad2",
        MaxCount=3,
        MinCount=3,
        InstanceType="t2.micro",
        KeyName="cc_kp",
        SecurityGroups=["launch-wizard-1"],
        IamInstanceProfile={
            "Arn": "arn:aws:iam::669000163710:instance-profile/t2-micro-role"
        },
    )

def list_running_instances(ec2):
    reservation = ec2.describe_instances()
    instances = reservation["Reservations"][0]["Instances"]
    running = []
    for instance in instances:
        if instance["State"]["Name"] == "running":
            running.append(instance["InstanceId"])
    print("running:", running)
    return running

def check_health_status(ec2, running):
    instance_statuses = ec2.describe_instance_status(InstanceIds=running)[
        "InstanceStatuses"
    ]
    for instance_status in instance_statuses:
        print(
            instance_status["InstanceId"],
            instance_status["InstanceStatus"],
            instance_status["SystemStatus"],
        )

def stop_running_instances(ec2, running):
    ec2.stop_instances(InstanceIds=running)

def terminate_running_instances(ec2, running):
    ec2.terminate_instances(InstanceIds=running)

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
    ec2 = boto3.client("ec2", region_name="ap-south-1")
    create_instances(ec2, startup_script)
    sleep(40)
    running = list_running_instances(ec2)
    check_health_status(ec2, running)
    sleep(30)
    stop_running_instances(ec2, running)
    terminate_running_instances(ec2, running)
