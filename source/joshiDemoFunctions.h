//
//
//                                                                    Test.h
//

#ifndef JOSHIDEMO_H
#define JOSHIDEMO_H

#include "xlw/MyContainers.h"
#include <xlw/CellMatrix.h>
#include <xlw/DoubleOrNothing.h>
#include <xlw/ArgList.h>
#include <xlw/MJCellMatrix.h> // added for having a matrix of parameters of mixed type
#include <MCStatistics.h>
#include <Antithetic.h>
#include <DoubleDigital2.h>
#include <Arrays.h>
#include <ConvergenceTable.h>
#include <SimpleMC8.h>
#include <Vanilla3.h>
#include <ParkMiller.h>
#include <vector>
#include <PayOffFactory.h>
#include <PathDependentAsian.h>
#include <ExoticBSEngine.h>
//#include <PayOffRegistration.h>


using namespace xlw;

//<xlw:libraryname=MonteCarloRoutines


short // echoes a short
EchoShort(short x // number to be echoed
       );

MyMatrix // adds two matrices together after checking they're the same size.
AddMatrix(const MyMatrix& matrixA, const MyMatrix& matrixB);

double // Return the price of vanilla call given spot, strike, r, d, vol, expiry, numPaths
MCVanillaCall(const MyMatrix& parametersMatrix);

CellMatrix // Return the price, variance and std error of vanilla option given spot, r, d, vol, expiry, name, numPaths, Strike
//<xlw:time
MCVanillaChoice3(const CellMatrix& parametersMatrix, unsigned long seed);

CellMatrix // Return the price, variance and std error of vanilla option given spot, r, d, vol, expiry, name, numPaths, Strike
//<xlw:time
MCVanillaChoice1(const CellMatrix& parametersMatrix); 


CellMatrix  // return convergence table for vanilla option given spot,  r, d, vol, expiry, name, numPaths, Strike
MCVanillaChoiceConvergenceTable(const CellMatrix& parametersMatrix);

CellMatrix // Return the price, variance and std error of vanilla option using Euler stepping engine given spot, r, d, vol, expiry, name, numPaths, numSteps, Strike
//<xlw:time
MCVanillaChoiceEulerStepping(const CellMatrix& parametersMatrix);

CellMatrix // Return the Delta of a vanilla call using pathwise method - make sure the string in the excel array is deltaCallPathwise
//<xlw:time
MCVanillaDeltaCall(const CellMatrix& parametersMatrix);

CellMatrix // Return the Delta of a vanilla call using likelihood ratio method 
//<xlw:time
MCVanillaDeltaLR(const CellMatrix& parametersMatrix, unsigned long seed);

CellMatrix // Return the price of an asian call option given spot, r, d, vol, expiry, numDates, numPaths, Strike
//<xlw:time
MCAsianCall(const CellMatrix& parametersMatrix);

CellMatrix // Return the price of an asian call option given spot, r, d, vol, expiry, numDatesToHedge, numPaths, Strike
//<xlw:time
MCDeltaHedge(const CellMatrix& parametersMatrix);


#endif
