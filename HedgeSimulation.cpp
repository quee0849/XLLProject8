#include "HedgeSimulation.h"

HedgeSimulation::HedgeSimulation(Wrapper<PathGeneratorClass>, Wrapper<HedgeStrategy>)
{
}

HedgeSimulation::~HedgeSimulation(void)
{
}

double HedgeSimulation::runSimulation(unsigned long seed){
return 0.0;}



PathGeneratorBS::PathGeneratorBS(MJArray& observationTimes, unsigned long numObservations, const Parameters& R_,
                                    const Parameters& D_,
                                    const Parameters& Vol_,
                                    const Wrapper<RandomBase>& TheGenerator_,
									double Spot_) : 
									dummyPayOff(100),
									dummyPayOffBridge(dummyPayOff),
									dummyProduct(observationTimes, observationTimes[numObservations-1], dummyPayOffBridge),
									joshisBSEngine(dummyProduct, R_, D_, Vol_, TheGenerator_,Spot_)
{
 // nothing to do now as we've just constructed the inner ExoticBSEngine
}

void PathGeneratorBS::getAnotherPath(MJArray& SpotValues)
{
		joshisBSEngine.GetOnePath(SpotValues);
}




DeltaHedgeStrategy::DeltaHedgeStrategy(MJArray& observationTimes, unsigned long numObservations, const Parameters& R_,
                                    const Parameters& D_,
									const Parameters& Vol_, double Strike_,double Expiry_):
strike(Strike_), expiry(Expiry_), r(R_.Mean(0,expiry)), d(D_.Mean(0,expiry)), vol(Vol_.Mean(0,expiry))
{
}

MJArray DeltaHedgeStrategy::costToHedgeAtTimeIndex(MJArray &SpotValues)
{
	int n= SpotValues.size();
	MJArray costArray(n);
	
	costArray[0]=BlackScholesDelta(SpotValues[0],strike,r,d,vol,expiry)*SpotValues[0];  
	if (n==1) { return costArray; }
	for (unsigned long j=1; j< SpotValues.size();j++)
	{
		costArray[j] = SpotValues[j]*BlackScholesDelta(SpotValues[j],strike,r,d,vol,expiry) - costArray[j-1];
	}
	return costArray;
}
