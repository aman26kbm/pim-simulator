#include <iostream>
#include <string>
#include <algorithm>
#include "tclap/CmdLine.h"

#include "backend/Config.h"
#include "backend/System.h"
#include <chrono>

using namespace std;
using namespace pimsim;



int main(int argc, char *argv[]) {

    string config_file;
    string param_file;
    string log_file;
    string workload;

    Config* config = nullptr;
    System* system = nullptr;
    //Parse command line options
    // Wrap everything in a try block.  Do this every time,
    // because exceptions will be thrown for problems.
    try {
        // Define the command line object.
        TCLAP::CmdLine cmd("PIMRA simulator", ' ', "0.9");

        // Define a value argument and add it to the command line.
        TCLAP::ValueArg<string> config_file_arg("c", "cfg", "name of config file", true, "mycfg", "string");
        cmd.add(config_file_arg);

        TCLAP::ValueArg<string> simulate_arg("m", "sim", "workload to simulate", false, "simulated", "string");
        cmd.add(simulate_arg);

        TCLAP::ValueArg<string> param_file_arg("p", "param", "parameters of the workload", false, "", "string");
        cmd.add(param_file_arg);

        TCLAP::ValueArg<string> log_file_arg("l", "log", "logfile to generate", false, "pimsim.log", "string");
        cmd.add(log_file_arg);

        // Parse the args.
        cmd.parse(argc, argv);

        // Get the value parsed by each arg.
        config_file = config_file_arg.getValue();
        param_file = param_file_arg.getValue();
        log_file = log_file_arg.getValue();

        config = new Config(config_file, log_file);
        system = new System(config);

        try {
          workload = simulate_arg.getValue();
          if (!Registry::registeredSimulation().count(workload)) {
            std::cerr << "cannot simulate " << workload << std::endl;
            return 1;
          } else {
            Registry::registeredSimulation()[workload].f(system, param_file);
          }
        } catch (TCLAP::ArgException &e) {
          std::cout << "It is ok to not have " << e.argId() << std::endl;
        }

    } catch (TCLAP::ArgException &e) {
        cerr << "Error: " << e.error() << " for arg " << e.argId() << endl;
    }

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    std::cout<<"Starting executing user program: "<<workload<<std::endl;
    //Execute the requests queued by the workload above
    system->run(workload);

    //Collect and print stats
    system->finish();

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    //std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() << "[s]" << std::endl;
    //std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;
    //std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << std::endl;

    std::cout<<"Done"<<endl;

    return 0;
}

