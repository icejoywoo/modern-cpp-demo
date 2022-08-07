#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HttpServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Util/ServerApplication.h>
#include <Poco/URI.h>

#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace Poco::Net;
using namespace Poco::Util;

class MyRequestHandler : public HTTPRequestHandler
{
public:
    virtual void handleRequest(HTTPServerRequest &req, HTTPServerResponse &resp)
    {
        resp.setStatus(HTTPResponse::HTTP_OK);
        resp.setContentType("text/html");

        ostream & out = resp.send();

        out << "<h1>Hello World!</h1>";
    }
};

class HelloRequestHandler : public HTTPRequestHandler
{
public:
    HelloRequestHandler(const string& name) : name_(name) {}

    virtual void handleRequest(HTTPServerRequest &req, HTTPServerResponse &resp)
    {
        resp.setStatus(HTTPResponse::HTTP_OK);
        resp.setContentType("text/html");

        ostream & out = resp.send();

        out << "<h1>Hello, " << name_ << "!</h1>";
    }
private:
    string name_;
};

class MyRequestHandlerFactory : public HTTPRequestHandlerFactory
{
public:
    virtual HTTPRequestHandler* createRequestHandler(const HTTPServerRequest &request)
    {
        const Poco::URI uri(request.getURI());
        // http://localhost:8088/hello?name=John
        // return: Hello, John!
        if (uri.getPath() == "/hello") {
            const Poco::URI::QueryParameters queryParms = uri.getQueryParameters();
            const char* paramName = "name";
            string name;
            for (const auto &item: queryParms) {
                if (item.first == paramName) {
                    name = item.second;
                }
            }
            if (name.empty()) {
                name = "Unknown";
            }
            return new HelloRequestHandler(name);
        } else {
            return new MyRequestHandler;
        }
    }
};

class MyServerApp :public ServerApplication
{
protected:
    int main(const vector<string> &)
    {
        HTTPServer s(new MyRequestHandlerFactory, ServerSocket(8088), new HTTPServerParams);
        s.start();
        cout << endl << "Server started" << endl;
        waitForTerminationRequest();  // wait for CTRL-C or kill
        cout << endl << "Shutting down..." << endl;
        s.stop();
        return Application::EXIT_OK;
    }
};

int main(int argc, char **argv)
{
    MyServerApp app;
    return app.run(argc, argv);
}