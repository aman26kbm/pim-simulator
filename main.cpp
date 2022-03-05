#include <iostream>
#include <string>

#include "backend/Config.h"
#include "backend/System.h"
#include "System.cpp"

#include <chrono>

#include "look_up_table.h"

using namespace std;
using namespace pimsim;


float RandomFloat(float a, float b) {
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}


int main(int argc, char *argv[]) {

    string config_file = "";
    config_file = argv[1];

    Config* config = new Config(config_file);
    System<float>* system = new System<float>(config);

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    //GEMV application
    system->gemv();

    system->finish();

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() << "[s]" << std::endl;
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << std::endl;



    return 0;
}

