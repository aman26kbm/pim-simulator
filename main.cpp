#include <iostream>
#include <string>
#include <algorithm>
#include "tclap/CmdLine.h"

#include "backend/Config.h"
#include "backend/System.h"
#include "System.cpp"

#include <chrono>

using namespace std;
using namespace pimsim;


int main(int argc, char *argv[]) {

    string config_file = "";
    bool gemv_program = false;
    bool fir_program = false;
    bool test_program = false;

    //Parse command line options
    // Wrap everything in a try block.  Do this every time,
    // because exceptions will be thrown for problems.
    try {
        // Define the command line object.
        TCLAP::CmdLine cmd("PIMRA simulator", ' ', "0.9");

        // Define a value argument and add it to the command line.
        TCLAP::ValueArg<string> config_file_arg("c", "cfg", "name of config file", true, "mycfg", "string");
        cmd.add(config_file_arg);

        // Define a switch and add it to the command line.
        TCLAP::SwitchArg gemv_program_arg("g", "gemv", "Run the GEMV program", false);
        cmd.add(gemv_program_arg);

        TCLAP::SwitchArg fir_program_arg("f", "fir", "Run the FIR program", false);
        cmd.add(fir_program_arg);

        TCLAP::SwitchArg test_program_arg("t", "test", "Run the test program", false);
        cmd.add(test_program_arg);

        // Parse the args.
        cmd.parse(argc, argv);

        // Get the value parsed by each arg.
        config_file = config_file_arg.getValue();
        gemv_program = gemv_program_arg.getValue();
        fir_program = fir_program_arg.getValue();
        test_program = test_program_arg.getValue();

    } catch (TCLAP::ArgException &e)  // catch any exceptions
    {
        cerr << "Error: " << e.error() << " for arg " << e.argId() << endl;
    }

    Config* config = new Config(config_file);
    System<float>* system = new System<float>(config);

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

    system->run();
    system->finish();

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() << "[s]" << std::endl;
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << std::endl;

    return 0;
}

