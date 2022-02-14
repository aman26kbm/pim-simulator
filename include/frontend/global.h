//#ifndef GLOBAL_H
//#define GLOBAL_H
//
//// ******************************************************************************* //
//// Global Identifier                                                               //
//// ******************************************************************************* //
//// It is recommended to not change this global identifier.
//
////  Physics identifiers used in the code:
//#define PROBLEM_ACOUSTIC                                                      1
//#define PROBLEM_ELASTIC                                                       2
//
////  Integrator identifiers used in the code:
//#define INTEGRATOR_EULER                                                      1
//#define INTEGRATOR_LSRK2                                                      2
//#define INTEGRATOR_LSRK3                                                      3
//#define INTEGRATOR_LSRK4                                                      4
//
//// ******************************************************************************* //
//
//// ******************************************************************************* //
//// Simulator Runtime Configuration                                                 //
//// ******************************************************************************* //
////  Control the Start Time
//#define SIM_START_TIME                                  0.00000000000000000000
////  Control the End Time
////#define SIM_END_TIME                                       0.00561312761712136
//#define SIM_END_TIME                                         0.000601406530405860
////#define SIM_END_TIME                                          0.0769048600756494
////#define SIM_END_TIME                                         0.820318507473593
////  Control the Time Step
//#define SIM_TIME_STEP                                      0.00080187537387448
////#define SIM_TIME_STEP                                       0.000075175816300733
//// ******************************************************************************* //
//
//// ******************************************************************************* //
//// Configurable Global Switch                                                      //
//// ******************************************************************************* //
//// Adjust the Global Switch as needed.                                             //
//
////  Control the number of 1D Node
//#define NNODE_1D                                                              8
//
////  Control the refinement level of the mesh
//#define REFINEMENT_LEVEL                                                      5
//
////  Number of material nodes in each direction (1D)
//#define NNODE_MATERIALS                                                       1
//
////  Precision Control
//#define PRECISION_DOUBLE
//
////  Define the Problem Type
////#define PROBLEM_TYPE                                           PROBLEM_ELASTIC
//
////  Define the Integrator Type
//#define INTEGRATOR_TYPE                                        INTEGRATOR_LSRK4
//
////  Define the p4est Mode (i.e., control the dimensionality)
//#define P4_TO_P8
//
//// ******************************************************************************* //
//
//
//// ******************************************************************************* //
//// Non-Configurable Global Switch                                                  //
//// ******************************************************************************* //
//// Do not change the switch here unless you know what you are doing.               //
//
//// Define the Dimensionality and Total Node
//#ifndef P4_TO_P8
//#define DIMENSION                                                        2
//	    #define NNODE_FACE                                                 NODE_1D
//        #define NNODE                                          (NNODE_1D*NNODE_1D)
//#else
//#define DIMENSION                                                        3
//#define NNODE_FACE                                     (NNODE_1D*NNODE_1D)
//#define NNODE                                 (NNODE_1D*NNODE_1D*NNODE_1D)
//#endif
//
//// Define the Maximum Axis Value for each dimensionality.
////#define MAX_AXIS_VALUE                                    (1<<REFINEMENT_LEVEL)
//
//// Define the Children for each refinement level.
//#define NUM_REFINEMENT_CHILDREN                                  (1<<DIMENSION)
//
//// Define the Number of Element Database because of refinement level
////#define NUM_ELEMENT_DATABASE                  (1<<(DIMENSION*REFINEMENT_LEVEL))
//
//// Define the Number of Materials and Number of Variables
//#if (PROBLEM_TYPE == PROBLEM_ACOUSTIC)
//#define NUMBER_OF_MATERIALS 2
//// MATERIALS := rho, kappa
//  #if (DIMENSION == 2)
//    #define NUM_VARS 3
//  // VARS := p, vx, vy
//  #else
//    #define NUM_VARS 4
//  // VARS := p, vx, vy, vz
//  #endif
//#elif (PROBLEM_TYPE == PROBLEM_ELASTIC)
//#define NUMBER_OF_MATERIALS 3
//// MATERIALS := rho, lambda, mu
//#if (DIMENSION == 2)
//#define NUM_VARS 5
//  // VARS := s11, s22, s12, v1, v2
//#else
//#define NUM_VARS 9
//// VARS := s11, s22, s33, s12, s13, s23, v1, v2, v3
//#endif
//#endif
//
//// Define the Number of Aux
//#if (INTEGRATOR_TYPE == INTEGRATOR_LSRK4)
//#define NUM_AUX 		                                                  1
//#endif
//
//// Define the Floating Point Type
//#ifdef PRECISION_DOUBLE
//typedef                                                      double real_t;
//#elif  PRECISION_SINGLE
//typedef                                                       float real_t;
//    #else
//        //  reserve for 1/2 float precision
//#endif
//
//// ******************************************************************************* //
//
//
//#endif //PIM_SIM_GLOBAL_H



#ifndef GLOBAL_H
#define GLOBAL_H


static int NNODE_1D = 8;
static int REFINEMENT_LEVEL = 5;
static int NNODE_MATERIALS = 1;
static int DIMENSION = 3;
static int NNODE_FACE = (NNODE_1D*NNODE_1D);
static int NNODE = (NNODE_1D*NNODE_1D*NNODE_1D);
static int MAX_AXIS_VALUE = (1<<REFINEMENT_LEVEL);
static int NUM_REFINEMENT_CHILDREN = (1<<DIMENSION);
static int NUM_ELEMENT_DATABASE = (1<<(DIMENSION*REFINEMENT_LEVEL));
static int NUMBER_OF_MATERIALS = 3;
static int NUM_VARS  = 9;

static int NUM_AUX = 1;
typedef double real_t;



#endif //PIM_SIM_GLOBAL_H
