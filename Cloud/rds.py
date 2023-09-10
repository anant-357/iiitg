import boto3
from time import sleep

Endpoint = None

def create_instance(startup_script):
    try:
        ec2 = boto3.client('ec2', region_name='ap-south-1')
        reservation = ec2.run_instances(
            ImageId='ami-0da59f1af71ea4ad2',
            MaxCount=1,
            MinCount=1,
            InstanceType='t2.micro',
            KeyName='cc_kp',
            SecurityGroups=['launch-wizard-1'],
            UserData=startup_script,
            IamInstanceProfile={
                'Arn': 'arn:aws:iam::669000163710:instance-profile/t2-micro-role'
            }
        )
        return reservation
        
        
    except Exception as e:
        print(e)

def create_rds_instance():
    rds = boto3.client('rds')
    res=None
    try:
        res = rds.describe_db_instances(
            DBInstanceIdentifier='node-cc-app-id'
        )
    finally:

        if res == None:
            rds.create_db_instance(
            DBName= "nodeAppDB",
            DBInstanceClass="db.t3.micro",
            Engine="mariadb",
            DBInstanceIdentifier = "node-cc-app-id",
            MasterUsername="user",
            MasterUserPassword="user1234",
            AllocatedStorage=10,
            StorageType="Standard",
            VpcSecurityGroupIds = [
                'sg-0880b1c0215f773fd'
            ],
            AvailabilityZone="ap-south-1a"
            )
        res = rds.describe_db_instances(
        DBInstanceIdentifier='node-cc-app-id'
    )
        print(res['DBInstances'][0]['DBInstanceStatus'])
        while res['DBInstances'][0]['DBInstanceStatus'] == 'creating':
            sleep(15)
            res = rds.describe_db_instances(
        DBInstanceIdentifier='node-cc-app-id'
            )
        return  res['DBInstances'][0]['Endpoint']


if __name__ == "__main__":
    
    Endpoint=create_rds_instance()
    startup_script = """#!/bin/bash
yum update -y
yum install mariadb105 -y

curl -fsSL https://rpm.nodesource.com/setup_16.x | bash -
yum update -y
yum install -y nodejs
npm install -g npm@latest

systemctl start mariadb
systemctl enable mariadb

mkdir /website
aws s3 sync s3://anant-cclab-bucket/website /website
cd /website/
echo 'const endpoint = "{}";module.exports=endpoint;' >> endpoint.js
npm install
node app.js
""".format(Endpoint['Address'])
    instance_response = create_instance(startup_script)
    
