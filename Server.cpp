#include <iostream>
#include <grpcpp/grpcpp.h>
#include "gRPC_stuff/calculator.grpc.pb.h"
#include "Utilities/Network_Utils.h"

using namespace std;

class mygRPC final : public ProtocolServiceTest::Service
{
    grpc::Status Add(grpc::ServerContext* context , const operands2* request , result1* result) override
    {
        if(!request || !result)
        {
            return grpc::Status(grpc::StatusCode::INTERNAL , "Null pointers have been passed");
        }
        result->set_res1(request->a() + request->b());

        cout << request->a() << " + " << request->b() << " = " << result->res1() << endl;

        return grpc::Status::OK;
    }

    grpc::Status Sub(grpc::ServerContext* context , const operands2* request , result1* result) override
    {
        if(!request || !result)
        {
            return grpc::Status(grpc::StatusCode::INTERNAL , "Null pointers have been passed");
        }
        result->set_res1(request->a() - request->b());

        cout << request->a() << " - " << request->b() << " = " << result->res1() << endl;

        return grpc::Status::OK;
    }

    grpc::Status Mul(grpc::ServerContext* context , const operands2* request , result1* result) override
    {
        if(!request || !result)
        {
            return grpc::Status(grpc::StatusCode::INTERNAL , "Null pointers have been passed");
        }
        result->set_res1(request->a() * request->b());

        cout << request->a() << " * " << request->b() << " = " << result->res1() << endl;

        return grpc::Status::OK;
    }

    grpc::Status Div(grpc::ServerContext* context , const operands2* request , result1* result) override
    {
        if(!request || !result)
        {
            return grpc::Status(grpc::StatusCode::INTERNAL , "Null pointers have been passed");
        }
        if (request->b() == 0) 
        {
            return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, "Division by zero not allowed");
        }
        result->set_res1(request->a() / request->b());

        cout << request->a() << " / " << request->b() << " = " << result->res1() << endl;

        return grpc::Status::OK;
    }
};

void startserver()
{
    mygRPC service;
    grpc::ServerBuilder builder;
    int selected_port = 0;

    builder.AddListeningPort("[::]:0", grpc::InsecureServerCredentials() , &selected_port);
    builder.RegisterService(&service);

    auto server = builder.BuildAndStart();
    cout << "Server listening on port : " << selected_port << endl;
    cout << "Device IP address is : " << NetworkUtils::getPrimaryIP() << endl;
    server->Wait();
}

int main()
{
    cout << "Server this side" << endl;
    startserver();
}