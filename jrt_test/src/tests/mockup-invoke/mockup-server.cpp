// Copyright 2016 Yahoo Inc. Licensed under the terms of the Apache 2.0 license. See LICENSE in the project root.

#include <vespa/fnet/frt/frt.h>
#include <vespa/fastos/app.h>

class MockupServer : public FRT_Invokable
{
private:
    MockupServer(const MockupServer &);
    MockupServer &operator=(const MockupServer &);

public:
    MockupServer(FRT_Supervisor *s)
    {
        FRT_ReflectionBuilder rb(s);
        //-------------------------------------------------------------------
        rb.DefineMethod("concat", "ss", "s", true,
                        FRT_METHOD(MockupServer::RPC_concat), this);
        rb.MethodDesc("Concatenate two strings");
        rb.ParamDesc("string1", "a string");
        rb.ParamDesc("string2", "another string");
        rb.ReturnDesc("ret", "the concatenation of string1 and string2");
        //-------------------------------------------------------------------
    }

    void RPC_concat(FRT_RPCRequest *req)
    {
        FRT_Values &params = *req->GetParams();
        FRT_Values &ret    = *req->GetReturn();

        uint32_t len = (params[0]._string._len +
                        params[1]._string._len);
        char *tmp = ret.AddString(len);
        strcpy(tmp, params[0]._string._str);
        strcat(tmp, params[1]._string._str);
    }
};


class App : public FastOS_Application
{
public:
    int Main() override;
};


int
App::Main()
{
    if (_argc < 2) {
        printf("usage: %s <listenspec>\n", _argv[0]);
        return 1;
    }
    FRT_Supervisor orb;
    MockupServer server(&orb);
    orb.Listen(_argv[1]);
    orb.Main();
    return 0;
}


int
main(int argc, char **argv)
{
  App myapp;
  return myapp.Entry(argc, argv);
}
