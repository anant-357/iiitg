import boto3

client = boto3.client('apigateway')


def create_api(name,description,version,types):
    response = client.create_rest_api(
        name=name,
        description=description,
        version=version,
        endpointConfiguration={'types': [types]}
    )
    return response['id']

def create_resource(apiId,parentId,pathPart):
    response=client.create_resource(
        restApiId=apiId,
        parentId=parentId,
        pathPart=pathPart
    )
    return response['id']


def get_parent_id(apiId,parent_resource_path):
    response = client.get_resources(
        restApiId=apiId,
        limit=500 
    )

    parent_resource_id=''
    # Iterate through the resources to find the parent resource
    for resource in response['items']:
        if resource['path'] == parent_resource_path:
            parent_resource_id = resource['id']
            break

    return parent_resource_id

def putMethod(apiId,authorizationType,resourceId,httpMethod): 
    client.put_method(
        restApiId=apiId,
        authorizationType=authorizationType,
        resourceId=resourceId,  
        httpMethod=httpMethod,
    )

def putIntegration(apiId,httpMethod,resourceId,type,integrationHttpMethod,url):    
    client.put_integration(
        restApiId=apiId,
        resourceId=resourceId,  
        httpMethod=httpMethod,
        type=type,
        integrationHttpMethod=integrationHttpMethod,
        uri=url
    )

def put_method_response(apiId,resourceId,httpMethod,statusCode,contentType,Model):
    client.put_method_response(
        restApiId=apiId,
        resourceId=resourceId,
        httpMethod=httpMethod,
        statusCode=statusCode,
        responseModels={
            contentType: Model,
        }
    ) 

def put_integration_response(apiId,resourceId,httpMethod,statusCode):
    client.put_integration_response(
        restApiId=apiId,
        resourceId=resourceId,
        httpMethod=httpMethod,
        statusCode=statusCode,
    )


def create_deployement(apiId,stageName):
    client.create_deployment(
        restApiId=apiId,
        stageName=stageName,
    )
    print("Stage Deployed successfully...")



name='_api_'
description='Gateway for microservies in portfolio'
version='v-1'
endpointConfigurationType='EDGE'
authorizationType='NONE'
httpMethod='GET'
integrationHttpMethod="GET"
type='HTTP'
statusCode='200'
contentType = 'text/html; charset=UTF-8'
Model='Empty'
stageName='dev'

cn='http://ec2-13-235-73-72.ap-south-1.compute.amazonaws.com/'
feedback='http://ec2-13-127-211-59.ap-south-1.compute.amazonaws.com/'
like='http://ec2-3-111-57-186.ap-south-1.compute.amazonaws.com/'


print("started creating Api Gateway...")
apiId=create_api(name,description,version,endpointConfigurationType)

resourceId=get_parent_id(apiId,'/')
root_resourceId=resourceId

putMethod(apiId,authorizationType,resourceId,httpMethod)
putIntegration(apiId,httpMethod,resourceId,type,integrationHttpMethod,cn)
put_method_response(apiId,resourceId,httpMethod,statusCode,contentType,Model)
put_integration_response(apiId,resourceId,httpMethod,statusCode)
print("successfully created / GET method...")


resourceId=create_resource(apiId,root_resourceId,"changeName")
httpMethod='GET'
integrationHttpMethod="GET"
type='HTTP'
putMethod(apiId,authorizationType,resourceId,httpMethod)
putIntegration(apiId,httpMethod,resourceId,type,integrationHttpMethod,cn+'changeName')
put_method_response(apiId,resourceId,httpMethod,statusCode,contentType,Model)
put_integration_response(apiId,resourceId,httpMethod,statusCode)
print("successfully created /changeName GET method...")

httpMethod='POST'
integrationHttpMethod="POST"
type='HTTP_PROXY'

putMethod(apiId,authorizationType,resourceId,httpMethod)
putIntegration(apiId,httpMethod,resourceId,type,integrationHttpMethod,cn+'changeName')
put_method_response(apiId,resourceId,httpMethod,statusCode,contentType,Model)
put_integration_response(apiId,resourceId,httpMethod,statusCode)
print("successfully created /changeName POST method...")

resourceId=create_resource(apiId,root_resourceId,"set-name")
httpMethod='GET'
integrationHttpMethod="GET"
type='HTTP'
putMethod(apiId,authorizationType,resourceId,httpMethod)
putIntegration(apiId,httpMethod,resourceId,type,integrationHttpMethod,cn+'set-name')
put_method_response(apiId,resourceId,httpMethod,statusCode,contentType,Model)
put_integration_response(apiId,resourceId,httpMethod,statusCode)
print("successfully created /set-name GET method...")

resourceId=create_resource(apiId,root_resourceId,"feedback")
httpMethod='GET'
integrationHttpMethod="GET"
type='HTTP'
putMethod(apiId,authorizationType,resourceId,httpMethod)
putIntegration(apiId,httpMethod,resourceId,type,integrationHttpMethod,feedback+'feedback')
put_method_response(apiId,resourceId,httpMethod,statusCode,contentType,Model)
put_integration_response(apiId,resourceId,httpMethod,statusCode)
print("successfully created /feedback GET method...")

httpMethod='POST'
integrationHttpMethod="POST"
type='HTTP_PROXY'
putMethod(apiId,authorizationType,resourceId,httpMethod)
putIntegration(apiId,httpMethod,resourceId,type,integrationHttpMethod,feedback+'feedback')
put_method_response(apiId,resourceId,httpMethod,statusCode,contentType,Model)
put_integration_response(apiId,resourceId,httpMethod,statusCode)
print("successfully created /feedback POST method...")

resourceId=create_resource(apiId,root_resourceId,"comments")
httpMethod='GET'
integrationHttpMethod="GET"
type='HTTP'
putMethod(apiId,authorizationType,resourceId,httpMethod)
putIntegration(apiId,httpMethod,resourceId,type,integrationHttpMethod,feedback+'comments')
put_method_response(apiId,resourceId,httpMethod,statusCode,contentType,Model)
put_integration_response(apiId,resourceId,httpMethod,statusCode)
print("successfully created /comments GET method...")

resourceId=create_resource(apiId,root_resourceId,"like")
httpMethod='GET'
integrationHttpMethod="GET"
type='HTTP'
putMethod(apiId,authorizationType,resourceId,httpMethod)
putIntegration(apiId,httpMethod,resourceId,type,integrationHttpMethod,like+'like')
put_method_response(apiId,resourceId,httpMethod,statusCode,contentType,Model)
put_integration_response(apiId,resourceId,httpMethod,statusCode)
print("successfully created /like GET method...")

resourceId=create_resource(apiId,root_resourceId,"get-like-count")
httpMethod='GET'
integrationHttpMethod="GET"
type='HTTP'
putMethod(apiId,authorizationType,resourceId,httpMethod)
putIntegration(apiId,httpMethod,resourceId,type,integrationHttpMethod,like+'get-like-count')
put_method_response(apiId,resourceId,httpMethod,statusCode,contentType,Model)
put_integration_response(apiId,resourceId,httpMethod,statusCode)
print("successfully created /get-like-count GET method...")

resourceId=create_resource(apiId,root_resourceId,"increment-like-count")
httpMethod='POST'
integrationHttpMethod="POST"
type='HTTP_PROXY'
putMethod(apiId,authorizationType,resourceId,httpMethod)
putIntegration(apiId,httpMethod,resourceId,type,integrationHttpMethod,like+'increment-like-count')
put_method_response(apiId,resourceId,httpMethod,statusCode,contentType,Model)
put_integration_response(apiId,resourceId,httpMethod,statusCode)
print("successfully created /increment-like-count POST method...")


create_deployement(apiId,stageName)
print("successfully Deployed API...")
url="https://"+apiId+".execute-api.ap-south-1.amazonaws.com/"+stageName+'/'
print(f"Deployment endpoint for {stageName} : {url}")

