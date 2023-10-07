import boto3
from time import sleep

appName = 'node-app'
envName = 'node-app-environment'
platform = '64bit Amazon Linux 2023 v6.0.0 running Node.js 18'
tierName = 'WebServer'
tierType = 'Standard'
bucketName = 'anant-cclab-website'
bucketKey = 'website.zip'

eb = boto3.client('elasticbeanstalk', region_name='ap-south-1')
cf = boto3.client('cloudfront', region_name='ap-south-1')
def create_cf_distribution():
    cf.create_distribution(
            DistributionConfig={
        "CallerReference": "CDNprofile",
        "DefaultRootObject": "",
        "Origins": {
            "Quantity": 1,
            "Items": [
                {
                    "Id": "anant-cclab-bucket",
                    "DomainName": "anant-cclab-bucket.s3.ap-south-1.amazonaws.com",
                    "S3OriginConfig": {"OriginAccessIdentity": ""},
                },
            ],
        },
        
        'DefaultCacheBehavior': dict(
                TargetOriginId = 'anant-cclab-bucket',
                ViewerProtocolPolicy= 'redirect-to-https',
                TrustedSigners = dict(Quantity=0, Enabled=False),
                ForwardedValues=dict(
                    Cookies = {'Forward':'all'},
                    Headers = dict(Quantity=0),
                    QueryString=False,
                    QueryStringCacheKeys= dict(Quantity=0),
                    ),
                MinTTL=1000
        ),
        "Comment": "distribution to host portfolio",
        "Enabled": True,
    }
            )

def create_app_ver():
    app_ver = eb.create_application_version(
                ApplicationName = appName,
                VersionLabel = "v1",
                Process = True,
                AutoCreateApplication=True,
                SourceBundle= {
                    "S3Bucket": "anant-cclab-website",
                    "S3Key": "website.zip"
                    }
        )
    print(app_ver)

def create_env():
    environment = eb.create_environment(
            ApplicationName = appName,
            EnvironmentName = envName,
            Tier = {
                'Name': tierName,
                'Type': tierType
                },
            VersionLabel = "v1",
            SolutionStackName = platform,
            OptionSettings = [
                {
                    'Namespace': 'aws:autoscaling:launchconfiguration',
                    'OptionName': 'IamInstanceProfile',
                    'Value': 't2-micro-role'
                },{
                    'Namespace':'aws:autoscaling:launchconfiguration',
                    'OptionName': 'EC2KeyName',
                    'Value':'cc_kp'
                },{
                    'Namespace': 'aws:autoscaling:asg',
                    'OptionName': 'MinSize',
                    'Value': '1'
                },{
                    'Namespace': 'aws:autoscaling:asg',
                    'OptionName': 'MaxSize',
                    'Value': '3'
                },{
                    "Namespace": "aws:autoscaling:trigger",
                    "OptionName": "MeasureName",
                    "Value": "CPUUtilization",
                },{
                    "Namespace": "aws:autoscaling:trigger",
                    "OptionName": "BreachDuration",
                    "Value": "5",
                },{
                    "Namespace": "aws:autoscaling:trigger",
                    "OptionName": "LowerThreshold",
                    "Value": "30",
                },{
                    "Namespace": "aws:autoscaling:trigger",
                    "OptionName": "LowerBreachScaleIncrement",
                    "Value": "-1",
                },{
                    "Namespace": "aws:autoscaling:trigger",
                    "OptionName": "Period",
                    "Value": "5",
                },{
                    "Namespace": "aws:autoscaling:trigger",
                    "OptionName": "EvaluationPeriods",
                    "Value": "2",
                },{
                    "Namespace": "aws:autoscaling:trigger",
                    "OptionName": "Statistic",
                    "Value": "Average",
                },{
                    "Namespace": "aws:autoscaling:trigger",
                    "OptionName": "Unit",
                    "Value": "Percent",
                },{
                    "Namespace": "aws:autoscaling:trigger",
                    "OptionName": "UpperBreachScaleIncrement",
                    "Value": "1",
                },{
                    "Namespace": "aws:autoscaling:trigger",
                    "OptionName": "UpperThreshold",
                    "Value": "80",
                }
                
                ]
            )

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
            DBInstanceClass="db.t2.micro",
            Engine="mariadb",
            DBInstanceIdentifier = "node-cc-app-id",
            MasterUsername="user",
            MasterUserPassword="user1234",
            AllocatedStorage=20,
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
        print(res['DBInstances'][0]['Endpoint'])

#create_rds_instance()
#create_app_ver()
#create_env()
create_cf_distribution()
