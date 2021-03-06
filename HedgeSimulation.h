#pragma once
#include <Arrays.h>
#include <ExoticBSEngine.h>
#include <PathDependentAsian.h>
#include <BlackScholesFormulas.h>
#include <Wrapper.h>
#include <vector>


// abstract base pathGenetatng class to define the interface.
class PathGeneratorClass 
{
public: 
	//PathGeneratorClass();
	//~PathGeneratorClass();

	// idea is to fill in the SpotValues array with the values at the given time intervals.
	virtual void getAnotherPath(MJArray& SpotValues) = 0;
//private:
	// record number of Observation points for a given class
	//unsigned long numObservations;
};


class PathGeneratorBS : public PathGeneratorClass 
{
	PathGeneratorBS(MJArray& observationTimes, unsigned long numObservations, const Parameters& R_,
                                    const Parameters& D_,
                                    const Parameters& Vol_,
                                    const Wrapper<RandomBase>& TheGenerator_,
                                    double Spot_);
	~PathGeneratorBS();
	virtual void getAnotherPath(MJArray& SpotValues) ;
	

private:

	PayOffCall dummyPayOff; 
	PayOffBridge dummyPayOffBridge;
	PathDependentAsian dummyProduct; // dummies used to construct joshisBSEngine below;
	ExoticBSEngine joshisBSEngine;

};
// abstract base class defining interface for Hedging Strategy.
class HedgeStrategy 
{
public:
	MJArray getPossibleTimes();
	unsigned long getNumPossibleTimes();
	// this is the main thing that the class will do - in the inherited class we can
	// define as much addtional data as we want - i.e how much cash to hold, stock to hold, options to hold etc, and calculate the cost of doing so now. 
	// i.e given an array of SpotValues generated by a  PathGenerator, calculate the cost of performing this hedging.

	// do we acually want to return a vector of CashFlows here else we lose the timing information?
	virtual MJArray costToHedgeAtTimeIndex(MJArray& SpotValues)=0;
private:
	// this might belong in the inherited classes - how can we initialize this number otherwise?
	unsigned long numPossibleHedgingTimes;
};

// given a 

class DeltaHedgeStrategy : public HedgeStrategy 
{ public:
	DeltaHedgeStrategy(MJArray& observationTimes, unsigned long numObservations, const Parameters& R_,
                                    const Parameters& D_,
                                    const Parameters& Vol_, double Strike_, double Expiry_);
	virtual MJArray costToHedgeAtTimeIndex(MJArray& SpotValues);
private: 
	double strike;
	double r; // should replace this with an array of average rs
	double d; // should replace this with an array of average ds
	double vol; // should replace this with an array of average vols
	double expiry;
	// an array of all the delta values calculated using the BSFormulas
	//MJArray deltas;
};


class HedgeSimulation
{
public:
	HedgeSimulation(Wrapper<PathGeneratorClass>, Wrapper<HedgeStrategy>);
	~HedgeSimulation(void);
	double runSimulation(unsigned long seed=1);
	std::vector<double> getResultsSoFar(); 
private:
	std::vector<double>  resultsVector; 
};
