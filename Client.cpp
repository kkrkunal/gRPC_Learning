#include <iostream>
#include <grpcpp/grpcpp.h>
#include <sstream>
#include <string>
#include <cctype>
#include "gRPC_stuff/calculator.grpc.pb.h"

using namespace std;

void startClient(const string& server_address)
{
    cout << "Connecting to address : " << server_address << endl;

    auto channel = grpc::CreateChannel(server_address, grpc::InsecureChannelCredentials());
    auto stub = ProtocolServiceTest::NewStub(channel);

    operands2 request;
    result1 result;
    std::string line;
    std::stringstream ss;
    char op = 0;
    while(op >= 0)
    {
        grpc::ClientContext context;

        std::cout << "Enter your simple expression : ";
        std::getline(std::cin , line);
        ss.clear();
        ss.str(line);

        int x;
        ss >> line;
        if(isdigit(line[0]))
        {
            x = stoi(line);
            request.set_a(x);
        }
        else exit(0);

        ss >> op;

        ss >> line;
        if(isdigit(line[0]))
        {
            x = stoi(line);
            request.set_b(x);
        }
        else exit(0);

        switch(op)
        {
            case '+' :
            {
                auto status = stub->Add(&context , request , &result);
                if(status.ok())
                {
                    cout << request.a() << " + " << request.b() << " = " << result.res1() << endl;
                }
                else
                {
                    cout << "Error : " << status.error_message() << endl;
                }
                break;
            }

            case '-' : 
            {
                auto status = stub->Sub(&context , request , &result);
                if(status.ok())
                {
                    cout << request.a() << " - " << request.b() << " = " << result.res1() << endl;
                }
                else
                {
                    cout << "Error : " << status.error_message() << endl;
                }
                break;
            }

            case '*' : 
            {
                auto status = stub->Mul(&context , request , &result);
                if(status.ok())
                {
                    cout << request.a() << " * " << request.b() << " = " << result.res1() << endl;
                }
                else
                {
                    cout << "Error : " << status.error_message() << endl;
                }
                break;
            }

            case '/' :
            {
                auto status = stub->Div(&context , request , &result);
                if(status.ok())
                {
                    cout << request.a() << " / " << request.b() << " = " << result.res1() << endl;
                }
                else
                {
                    cout << "Error : " << status.error_message() << endl;
                }
                break;
            }

            default :
            {
                cout << "Invalide Operation" << endl;
            }
        }
    }
}

int main()
{
    cout << "Client this side" << endl;
    string ip , port , server_address;
    
    cout << "Enter IP address to connect : " ;
    cin >> ip;
    cout << endl;

    cout << "Enter port : ";
    cin >> port;
    cout << endl;

    server_address = ip + ":" + port;
    cin.ignore();
    
    startClient(server_address);
}