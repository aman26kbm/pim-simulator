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

//    int index_A = 252;
//    int index_B = 1;
//    int size = 256;
//
//    int height = h_tree_height(size);
////    std::vector<int> index = get_index_list(index_B, height);
//    std::vector<int> index = h_tree_switches(index_A, index_B, size);
//
//    for (int i = 0; i < index.size(); i++)
//        cout << index[i] << ", ";
//    cout << endl;
//
//    cout << "height: " << height << endl;




//    float a[NUM_ELEMENT_DATABASE];
//    float b[NUM_ELEMENT_DATABASE];
//    float c[NUM_ELEMENT_DATABASE * 6];
//
//    for (int i = 0; i < NUM_ELEMENT_DATABASE; i++) {
//        a[i] = RandomFloat(1.0, 1000.0);
//        b[i] = RandomFloat(1.0, 1000.0);
//    }
//
//    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
//
//    for (int i = 0; i < NUM_ELEMENT_DATABASE; i++) {
//        c[i] = sqrt(a[i]*b[i]);
//    }
//
//    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
//    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() << "[s]" << std::endl;
//    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;
//    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << std::endl;
//
//
//    begin = std::chrono::steady_clock::now();
//
//    for (int i = 0; i < NUM_ELEMENT_DATABASE * 6; i++) {
//        c[i] = 1.0 / (a[i%NUM_ELEMENT_DATABASE]+b[i%NUM_ELEMENT_DATABASE]);
//    }
//
//    end = std::chrono::steady_clock::now();
//    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() << "[s]" << std::endl;
//    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;
//    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << std::endl;


    string config_file = "";
//    string config_file = "../configs/elastic_riemann_flux_5.cfg";

    config_file = argv[1];

    Config* config = new Config(config_file);
    System<float>* system = new System<float>(config);

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    int level = config_file.at(22) - '0';
    std::cout << "level: " << level << std::endl;


//    if (atoi(argv[3]) == 0) { // acoustic
//        NUM_VARS = 4;
//        NUMBER_OF_MATERIALS = 2;
//	    if (level == 5) {
//            REFINEMENT_LEVEL = 5;
//            MAX_AXIS_VALUE = (1 << REFINEMENT_LEVEL);
//            if (atoi(argv[2]) == 512) {
//                NUM_ELEMENT_DATABASE = (16*256);
//            } else if (atoi(argv[2]) == 2048) {
//                NUM_ELEMENT_DATABASE = (64*256);
//            } else if (atoi(argv[2]) == 8192) {
//                NUM_ELEMENT_DATABASE = (128*256);
//            } else if (atoi(argv[2]) == 16384) {
//                NUM_ELEMENT_DATABASE = (128*256);
//            }
//	    } else if (level == 4) {
//            REFINEMENT_LEVEL = 4;
//            MAX_AXIS_VALUE = (1 << REFINEMENT_LEVEL);
//            NUM_ELEMENT_DATABASE = (16*256);
//	    }
//
//	    if (atoi(argv[5]) == 0) { // no expansion
//	        if (atoi(argv[4]) == 0) // volume
//	            system->multi_element_compute_volume_acoustic_3d();
//	        else // integrate
//	            system->IntegrationLSRK4(0);
//	    } else { // expansion
//	        if (atoi(argv[4]) == 0) // volume
//	            system->multi_element_compute_volume_acoustic_3d_4();
//	        else // integrate
//	            system->IntegrationLSRK4_4(0);
//	    }
//    } else if (atoi(argv[3]) == 1) { // elastic
//        NUM_VARS = 9;
//        NUMBER_OF_MATERIALS = 3;
//        if (level == 5) {
//            REFINEMENT_LEVEL = 5;
//            MAX_AXIS_VALUE = (1 << REFINEMENT_LEVEL);
//            if (atoi(argv[2]) == 512) {
//                NUM_ELEMENT_DATABASE = (16*256/4);
//            } else if (atoi(argv[2]) == 2048) {
//                NUM_ELEMENT_DATABASE = (64*256/4);
//            } else if (atoi(argv[2]) == 8192) {
//                NUM_ELEMENT_DATABASE = (256*256/4);
//            } else if (atoi(argv[2]) == 16384) {
//                NUM_ELEMENT_DATABASE = (512*256/4);
//            }
//        } else if (level == 4) {
//            REFINEMENT_LEVEL = 4;
//            MAX_AXIS_VALUE = (1 << REFINEMENT_LEVEL);
//		    if (atoi(argv[2]) == 512) {
//                NUM_ELEMENT_DATABASE = (16*256/4);
//            } else {
//                NUM_ELEMENT_DATABASE = (64*256/4);
//            }
//        }
//
//        if (atoi(argv[5]) == 0) { // no expansion
//            if (atoi(argv[4]) == 0) // volume
//                system->multi_element_compute_volume_elastic_3d();
//            else // integrate
//                system->IntegrationLSRK4_elastic(0);
//        } else { // expansion
//            if (atoi(argv[4]) == 0) // volume
//                system->multi_element_compute_volume_elastic_3d_4();
//            else // integrate
//                system->IntegrationLSRK4_elastic_4(0);
//        }
//    }

//    if (atoi(argv[3]) == 0) { // acoustic
//        NUM_VARS = 4;
//        NUMBER_OF_MATERIALS = 2;
//        if (level == 5) {
//            REFINEMENT_LEVEL = 5;
//            MAX_AXIS_VALUE = (1 << REFINEMENT_LEVEL);
//            if (atoi(argv[2]) == 512) {
//                NUM_ELEMENT_DATABASE = (16*256);
//            } else if (atoi(argv[2]) == 2048) {
//                NUM_ELEMENT_DATABASE = (64*256);
//            } else if (atoi(argv[2]) == 8192) {
//                NUM_ELEMENT_DATABASE = (128*256);
//            } else if (atoi(argv[2]) == 16384) {
//                NUM_ELEMENT_DATABASE = (128*256);
//            }
//        } else if (level == 4) {
//            REFINEMENT_LEVEL = 4;
//            MAX_AXIS_VALUE = (1 << REFINEMENT_LEVEL);
//            NUM_ELEMENT_DATABASE = (16*256);
//        }
//
//        if (atoi(argv[5]) == 0) { // no expansion
//            if (atoi(argv[4]) == 0) // compute
//                system->multi_element_compute_flux_acoustic_3d();
//            else // transfer
//                system->multi_element_compute_flux_acoustic_3d_inter();
//        } else { // expansion
//            if (atoi(argv[4]) == 0) // compute
//                system->multi_element_compute_flux_acoustic_3d_4();
//            else // transfer
//                system->multi_element_compute_flux_acoustic_3d_4_inter();
//        }
//    } else if (atoi(argv[3]) == 1) { // elastic
//        NUM_VARS = 9;
//        NUMBER_OF_MATERIALS = 3;
//        if (level == 5) {
//            REFINEMENT_LEVEL = 5;
//            MAX_AXIS_VALUE = (1 << REFINEMENT_LEVEL);
//            if (atoi(argv[2]) == 512) {
//                NUM_ELEMENT_DATABASE = (16*256/4);
//            } else if (atoi(argv[2]) == 2048) {
//                NUM_ELEMENT_DATABASE = (64*256/4);
//            } else if (atoi(argv[2]) == 8192) {
//                NUM_ELEMENT_DATABASE = (256*256/4);
//            } else if (atoi(argv[2]) == 16384) {
//                NUM_ELEMENT_DATABASE = (512*256/4);
//            }
//        } else if (level == 4) {
//            REFINEMENT_LEVEL = 4;
//            MAX_AXIS_VALUE = (1 << REFINEMENT_LEVEL);
//            if (atoi(argv[2]) == 512) {
//                NUM_ELEMENT_DATABASE = (16*256/4);
//            } else {
//                NUM_ELEMENT_DATABASE = (64*256/4);
//            }
//        }
//
//        if (atoi(argv[5]) == 0) { // no expansion
//            if (atoi(argv[4]) == 0) // compute
//                system->multi_element_compute_flux_elastic_3d();
//            else // transfer
//                system->multi_element_compute_flux_elastic_3d_inter();
//        } else { // expansion
//            if (atoi(argv[4]) == 0) // compute
//                system->multi_element_compute_flux_elastic_3d_4();
//            else // transfer
//                system->multi_element_compute_flux_elastic_3d_4_inter();
//        }
//    }


    if (atoi(argv[3]) == 1) { // elastic
        NUM_VARS = 9;
        NUMBER_OF_MATERIALS = 3;
        if (level == 5) {
            REFINEMENT_LEVEL = 5;
            MAX_AXIS_VALUE = (1 << REFINEMENT_LEVEL);
            if (atoi(argv[2]) == 512) {
                NUM_ELEMENT_DATABASE = (16*256/4);
            } else if (atoi(argv[2]) == 2048) {
                NUM_ELEMENT_DATABASE = (64*256/4);
            } else if (atoi(argv[2]) == 8192) {
                NUM_ELEMENT_DATABASE = (256*256/4);
            } else if (atoi(argv[2]) == 16384) {
                NUM_ELEMENT_DATABASE = (512*256/4);
            }
        } else if (level == 4) {
            REFINEMENT_LEVEL = 4;
            MAX_AXIS_VALUE = (1 << REFINEMENT_LEVEL);
            if (atoi(argv[2]) == 512) {
                NUM_ELEMENT_DATABASE = (16*256/4);
            } else {
                NUM_ELEMENT_DATABASE = (64*256/4);
            }
        }

        if (atoi(argv[5]) == 0) { // no expansion
            system->multi_element_compute_flux_elastic_riemann_3d();
        } else { // expansion
            system->multi_element_compute_flux_elastic_riemann_3d_4();
        }
    }



//    system->multi_element_compute_volume_acoustic_3d();
//    for (int i = 0; i < 5; i++) {
//        cout << "i: " << i << endl;
//        system->IntegrationLSRK4(i);
//    }
//    system->load_data_volume();
//    system->store_data_flux();



//    system->multi_element_compute_volume_elastic_3d();
//    system->multi_element_compute_flux_elastic_3d();
//    system->multi_element_compute_flux_elastic_riemann_3d();;

//    for (int i = 0; i < 5; i++) {
//        cout << "i: " << i << endl;
//        system->IntegrationLSRK4_elastic(0);
//    }

//    system->multi_element_compute_volume_elastic_3d_4();
//    system->multi_element_compute_flux_elastic_3d_4();

//    for (int i = 0; i < 5; i++) {
//        cout << "i: " << i << endl;
//        system->IntegrationLSRK4_elastic_4(i);
//    }

    system->finish();

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() << "[s]" << std::endl;
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << std::endl;



//    config_file = "../configs/flux_float_bus.cfg";
//    config = new Config(config_file);
//    system = new System<float>(config);
//
//    begin = std::chrono::steady_clock::now();
//
//    system->multi_element_compute_flux_acoustic_3d();
//    system->finish();
//
//   end = std::chrono::steady_clock::now();
//    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() << "[s]" << std::endl;
//    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;
//    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << std::endl;



    return 0;
}

