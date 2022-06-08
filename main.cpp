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

    printf("simulator start:\n");

    string config_file;
    bool gemv_program = false;
    bool fir_program = false;
    bool test_program = false;
    bool sync_program = false;
    string to_simulate;

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

        TCLAP::ValueArg<string> simulate_arg("m", "sim", "simulate the target", false, "simulated", "string");
        cmd.add(simulate_arg);

        // Define a switch and add it to the command line.
        TCLAP::SwitchArg gemv_program_arg("g", "gemv", "Run the GEMV program", false);
        cmd.add(gemv_program_arg);

        TCLAP::SwitchArg fir_program_arg("f", "fir", "Run the FIR program", false);
        cmd.add(fir_program_arg);

        TCLAP::SwitchArg test_program_arg("t", "test", "Run the test program", false);
        cmd.add(test_program_arg);

        TCLAP::SwitchArg sync_program_arg("s", "sync", "Run the sync program", false);
        cmd.add(sync_program_arg);

        // Parse the args.
        cmd.parse(argc, argv);

        // Get the value parsed by each arg.
        config_file = config_file_arg.getValue();
        gemv_program = gemv_program_arg.getValue();
        fir_program = fir_program_arg.getValue();
        test_program = test_program_arg.getValue();
        sync_program = sync_program_arg.getValue();

        config = new Config(config_file);
        system = new System(config);

        try {
          to_simulate = simulate_arg.getValue();
          if (!Registry::registeredSimulation().count(to_simulate)) {
            std::cerr << "cannot simulate " << to_simulate << std::endl;
            return 1;
          } else {
            Registry::registeredSimulation()[to_simulate].f(system);
          }
        } catch (TCLAP::ArgException &e) {
          std::cout << "It is ok to not have " << e.argId() << std::endl;
        }

    } catch (TCLAP::ArgException &e) {
        cerr << "Error: " << e.error() << " for arg " << e.argId() << endl;
    }

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    //GEMV application
    if (gemv_program) {
        std::cout<<"Running GEMV program"<<std::endl;
        system->gemv();
    }

    //FIR application
    if (fir_program) {
        std::cout<<"Running fir program"<<std::endl;
        system->fir();
    }

    //Test application
    if (test_program) {
        std::cout<<"Running test program"<<std::endl;
        system->test();
    }

    if(sync_program) {
        std::cout<<"Running sync program"<<std::endl;
        system->sync();
    }

    printf("starting executing user program:\n");
    //Execute the requests queued by the workload above
    system->run();

    //Collect and print stats
    system->finish();

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    //std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() << "[s]" << std::endl;
    //std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;
    //std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << std::endl;

    std::cout<<"Done"<<endl;

    return 0;
}

