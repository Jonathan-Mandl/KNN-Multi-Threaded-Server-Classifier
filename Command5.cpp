
#include "Command5.h"
#include <iostream>
#include <unistd.h>


Command5::Command5(DefaultIO *dio, std::vector<std::vector<double>> &Xexamples,
                   std::vector<std::string> &Yexamples, std::vector<std::vector<double>> &XtoClassify,
                   std::vector<std::string> &Yresults)
    : Command("download results", dio), Yresults(Yresults), Xexamples(Xexamples),
      Yexamples(Yexamples), XtoClassify(XtoClassify)
{
}
/*
this method asks client to enter file path and sends to client the labels of test file.
client should add the text to the specified file path.
*/
void Command5::execute()
{
    dio->write("***download");
    dio->read();
//if data is not uploaded or classified, it sends message to client.
    if (Xexamples.size() == 0 || Yexamples.size() == 0 || XtoClassify.size() == 0)
    {
        dio->write("please upload data");
        dio->read();
    }
    else if (Yresults.size() == 0)
    {
        dio->write("please classify the data");
        dio->read();
    }
    else
    {
        dio->write("Please enter file path:");
        string valid=dio->read();
        if(valid=="***invalid_path")
        {
            dio->write("Invalid file path");
            dio->read();
            return;
        }

        dio->write("downloading");
        dio->read();

        for (int i = 0; i < Yresults.size(); i++)
        {
            sleep(0.01);
            dio->write(to_string(i + 1) + "\t" + Yresults[i]);
            dio->read();
        }

        dio->write("***done");

    
    }

}