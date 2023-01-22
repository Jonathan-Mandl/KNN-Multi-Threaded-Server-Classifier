
#include "Command2.h"
#include <sstream>
#include <string.h>
#include <cmath>
#include <iostream>
#include <unistd.h>

using namespace std;

Command2::Command2(DefaultIO *dio, int &k, string &metric)
    : Command("algorithm settings", dio),
      metric(metric), k(k)
{
}

void Command2::execute()
{
    dio->write("The current KNN parameters are: k= " + to_string(this->k) + ", distance metric = " + this->metric);
    // user should enter k, space and then distance metric name.
    string parameters = dio->read();

    if (parameters == "***end")
    {
        return;
    }
    else
    {
        stringstream s_stream(parameters);
        string sub_String1;
        getline(s_stream, sub_String1, ' ');
        double k_value;
        try
        {
            k_value = stod(sub_String1);
        }
        catch (std::invalid_argument& e)
        {
            dio->write("***invalid");
            dio->read();
            dio->write("invalid value for K");
            return;
        }

        string metric_value;
        getline(s_stream, metric_value, ' ');
        if (ceil(k_value) == floor(k_value) && k_value > 0 &&
            (metric_value == "AUC" || metric_value == "MAN" || metric_value == "CHB" || metric_value == "CAN" || metric_value == "MIN"))
        {
            this->k = k_value;
            this->metric = metric_value;
            dio->write("***valid");
            dio->read();
            return;
        }
        else
        {
            if (ceil(k_value) != floor(k_value) || k_value <= 0)
            {
                dio->write("***invalid");
                dio->read();
                dio->write("invalid value for K");
            }
            else
            {
                dio->write("***invalid");
                dio->read();
                dio->write("invalid value for metric");
            }
        }
    }
}