import boto3
from time import sleep
import base64

startup_script = """#!/bin/bash
    yum update -y
    yum install httpd -y
    systemctl start httpd.service
    systemctl enable httpd
    usermod -a -G apache ec2-user
    chown -R ec2-user:apache /var/www
    chmod 2775 /var/www
    aws s3 sync s3://anant-cclab-bucket/website /var/www/html/
    systemctl restart httpd""".encode('ascii')

startup_script=base64.b64encode(startup_script)

def create_autoscaling_group():
    client = boto3.client("autoscaling")
    response = client.create_auto_scaling_group(
        AutoScalingGroupName="autoscaling-group-1",
        MinSize=0,
        DesiredCapacity=2,
        MaxSize=2,
        LaunchTemplate={"LaunchTemplateName": "launch-template-1"},
        AvailabilityZones=["ap-south-1a", "ap-south-1b", "ap-south-1c"],
    )
    return response

def create_launch_template():
    client = boto3.client("ec2")
    response = client.create_launch_template(
        LaunchTemplateName="launch-template-1",
        LaunchTemplateData={
            "ImageId": "ami-0da59f1af71ea4ad2",
            "InstanceType": "t2.micro",
            "KeyName": "cc_kp",
            "SecurityGroups": ["launch-wizard-1"],
            "IamInstanceProfile": {
                "Arn": "arn:aws:iam::669000163710:instance-profile/t2-micro-role"
            },
            "UserData":startup_script.decode('ascii'),
        },
    )
    return response

def create_up_scaling_policies():
    client = boto3.client("autoscaling")
    response = client.put_scaling_policy(
        AutoScalingGroupName="autoscaling-group-1",
        PolicyName='scale-up-policy',
        PolicyType='StepScaling',
        AdjustmentType="ChangeInCapacity",
        StepAdjustments=[
            {
                "MetricIntervalLowerBound": 0,
                "ScalingAdjustment": 1,
            },
        ],
        Cooldown=300,
        MetricAggregationType='Average',
    )
    return response

def create_down_scaling_policies():
    client = boto3.client("autoscaling")
    response = client.put_scaling_policy(
        AutoScalingGroupName="autoscaling-group-1",
        PolicyName='scale-down-policy',
        PolicyType='StepScaling',
        AdjustmentType="ChangeInCapacity",
        StepAdjustments=[
            {
                "MetricIntervalUpperBound":0,
                "ScalingAdjustment": -1,
            }
        ],
        Cooldown=300,
        MetricAggregationType='Average',
    )
    return response

def create_cloudwatch_alarms(scale_up_policy_arn, scale_down_policy_arn):
    client = boto3.client("cloudwatch")
    response_up_alarm = client.put_metric_alarm(
        AlarmName="scale-up-alarm",
        ComparisonOperator="GreaterThanThreshold",
        EvaluationPeriods=1,
        MetricName="CPUUtilization",
        Namespace='AWS/EC2',
        Period=60,
        Statistic="Average",
        Threshold=70,
        AlarmDescription="Scale up when CPU utilization is high",
        AlarmActions=[scale_up_policy_arn],
        Dimensions=[
            {
                "Name": "AutoScalingGroupName",
                "Value": "autoscaling-group-1",
            }
        ],
    )

    response_down_alarm = client.put_metric_alarm(
        AlarmName="scale-down-alarm",
        ComparisonOperator="LessThanThreshold",
        EvaluationPeriods=1,
        MetricName="CPUUtilization",
        Namespace='AWS/EC2',
        Period=60,
        Statistic="Average",
        Threshold=30,
        AlarmDescription="Scale down when CPU utilization is low",
        AlarmActions=[scale_down_policy_arn],
        Dimensions=[
            {
                "Name": "AutoScalingGroupName",
                "Value": "autoscaling-group-1",
            },
        ],
    )

def run():
    ec2 = boto3.client("ec2")
    response = ec2.run_instances(
        LaunchTemplate={"LaunchTemplateName": "launch-template-1"},
        MaxCount=1,
        MinCount=1,
    )

    client = boto3.client("autoscaling")
    sleep(30)
    client.attach_instances(
        AutoScalingGroupName="autoscaling-group-1",
        InstanceIds=[
            response['Instances'][0]['InstanceId'],
        ],
    )

launch_template_response = create_launch_template()
#create_autoscaling_group()
#up_scaling_policy_response = create_up_scaling_policies()
#down_scaling_policy_response = create_down_scaling_policies()
#create_cloudwatch_alarms(up_scaling_policy_response["PolicyARN"],down_scaling_policy_response["PolicyARN"])
#run()
# up_scaling_policy_response, down_scaling_policy_response

