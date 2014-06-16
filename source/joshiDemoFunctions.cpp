#include<joshiDemoFunctions.h>
#pragma warning (disable : 4996)

//using namespace xlw;

short // echoes a short
EchoShort(short x // number to be echoed
           )
{
    return x;
}

MyMatrix // adds two matrices together after checking they're the same size.
AddMatrix(const MyMatrix& matrixA, const MyMatrix& matrixB) {

	if (matrixA.rows() != matrixB.rows() || matrixA.columns() != matrixB.columns()){
	throw("matrices must be of the same size");}

	MyMatrix result(matrixA.rows(), matrixB.columns());

	for (size_t r=0; r<matrixA.rows();r++) {
		for (size_t c= 0 ; c < matrixB.columns(); c++) {
		result(r,c) = matrixA(r,c)+ matrixB(r,c);
		}
	}
	return result;

}

double // Return the MC price of vanilla call given spot, strike, r, d, vol, expiry, numberOfPaths
MCVanillaCall(const MyMatrix& parametersMatrix) {
	if (parametersMatrix.columns() != 7 && parametersMatrix.rows() != 1 )
	{throw("Input matrix should be 1 x 7");}

	double Spot =  parametersMatrix(0,0);
	double Strike =  parametersMatrix(0,1);
	double r =  parametersMatrix(0,2);
	double d =  parametersMatrix(0,3);
	double vol =  parametersMatrix(0,4);
	double expiry = parametersMatrix(0,5); 
	unsigned long NumberOfPaths = parametersMatrix(0,6);

    PayOffCall thePayOff(Strike);

    VanillaOption theOption(thePayOff, expiry);

    ParametersConstant VolParam(vol);
    ParametersConstant rParam(r);

    StatisticsMean gatherer;
    ConvergenceTable gathererTwo(gatherer);

    RandomParkMiller RNgenerator(1);
	//RandomMersTwister generator(1);
    AntiThetic GenTwo(RNgenerator);

	SimpleMonteCarlo6(theOption,
                                      Spot, 
                                      VolParam,
                                      rParam,
                                      NumberOfPaths,
                                      gathererTwo,
									  RNgenerator); // no Antithetic
                                      //GenTwo); // Antithetic

	return gathererTwo.GetResultsSoFar()[gathererTwo.GetResultsSoFar().size()-1][0];
}



CellMatrix MCVanillaChoice3(const CellMatrix& parametersMatrix, unsigned long seed) {
	// check selected area has the correct size
	if ( parametersMatrix.ColumnsInStructure() < 8  || parametersMatrix.ColumnsInStructure() > 9 ||
		parametersMatrix.RowsInStructure() !=1 )
	{
		throw("Input matrix should be 1 x 8 or 1x9");
	}
	double Spot =  parametersMatrix(0,0).NumericValue();
	double r =  parametersMatrix(0,1).NumericValue();
	double d =  parametersMatrix(0,2).NumericValue();
	double vol =  parametersMatrix(0,3).NumericValue();
	double expiry = parametersMatrix(0,4).NumericValue(); 
	std::string name = parametersMatrix(0,5).StringValue();
	unsigned long NumberOfPaths = parametersMatrix(0,6).NumericValue();
	double Strike[2];
	Strike[0] = parametersMatrix(0,7).NumericValue();
	// extra value for double digital - can't use an if statement as Strike[1] goes out of scope
	Strike[1] = (parametersMatrix.ColumnsInStructure()==9) ? 
		parametersMatrix(0,8).NumericValue() : 110;
    PayOff* PayOffPtr = PayOffFactory::Instance().CreatePayOff(name,Strike); 
	VanillaOption theOption(*PayOffPtr, expiry);

    if (PayOffPtr != NULL)
    {
		ParametersConstant VolParam(vol);
		ParametersConstant rParam(r);
		StatisticsBasic gathererBasic;
		StatisticsMean gatherer;
		ConvergenceTable gathererTwo(gatherer);

		RandomParkMiller generator(1);
		generator.SetSeed(seed);
		AntiThetic GenTwo(generator);

		SimpleMonteCarlo6(theOption,
                                      Spot, 
                                      VolParam,
                                      rParam,
                                      NumberOfPaths,
                                      gathererBasic,
									  generator);
        delete PayOffPtr;
		CellMatrix resultMatrix(1,3); 
		resultMatrix(0,0) = (gathererBasic.GetResultsSoFar()[0][0]); //mean
		resultMatrix(0,1) =  (gathererBasic.GetResultsSoFar()[0][1]); //var
		resultMatrix(0,2) =  (gathererBasic.GetResultsSoFar()[0][2]); //std error
		return resultMatrix;
		}
		else {
			MyMatrix resultMatrix(1,3);
			resultMatrix(0,0)=0.0; resultMatrix(0,1)=0.0; resultMatrix(0,2)=0.0; 
			return resultMatrix;
		}
}




CellMatrix MCVanillaChoiceConvergenceTable(const CellMatrix& parametersMatrix) {
	// check selected area has the correct size
	if ( parametersMatrix.ColumnsInStructure() < 8  || parametersMatrix.ColumnsInStructure() > 9 ||
		parametersMatrix.RowsInStructure() !=1 )
	{
		throw("Input matrix should be 1 x 8 or 1x9");
	}
	double Spot =  parametersMatrix(0,0).NumericValue();
	double r =  parametersMatrix(0,1).NumericValue();
	double d =  parametersMatrix(0,2).NumericValue();
	double vol =  parametersMatrix(0,3).NumericValue();
	double expiry = parametersMatrix(0,4).NumericValue(); 
	std::string name = parametersMatrix(0,5).StringValue();
	unsigned long NumberOfPaths = parametersMatrix(0,6).NumericValue();
	double Strike[2];
	Strike[0] = parametersMatrix(0,7).NumericValue();
	// extra value for double digital - can't use an if statement as Strike[1] goes out of scope
	Strike[1] = (parametersMatrix.ColumnsInStructure()==9) ? 
		parametersMatrix(0,8).NumericValue() : 110;
    PayOff* PayOffPtr = PayOffFactory::Instance().CreatePayOff(name,Strike); 
	VanillaOption theOption(*PayOffPtr, expiry);

    if (PayOffPtr != NULL)
    {
		ParametersConstant VolParam(vol);
		ParametersConstant rParam(r);
		//StatisticsBasic gathererBasic;
		StatisticsMean gatherer;
		ConvergenceTable gathererTwo(gatherer);

		RandomParkMiller generator(1);
		//AntiThetic GenTwo(generator);

		SimpleMonteCarlo6(theOption,
                                      Spot, 
                                      VolParam,
                                      rParam,
                                      NumberOfPaths,
                                      gathererTwo,
									  generator);
        delete PayOffPtr;
		std::vector< std::vector<double>> resultsVector = gathererTwo.GetResultsSoFar();
		CellMatrix resultMatrix(resultsVector.size(),2); 
		for (int i=0; i<resultsVector.size(); i++ ) {
		resultMatrix(i,0) = resultsVector[i][1];
		resultMatrix(i,1) = resultsVector[i][0]; 
		}
		return resultMatrix;
		}
		else {
			CellMatrix resultMatrix(1,3);
			resultMatrix(0,0)=0.0; resultMatrix(0,1)=0.0; resultMatrix(0,2)=0.0; 
			return resultMatrix;
		}
}

//using Euler stepping
CellMatrix MCVanillaChoiceEulerStepping(const CellMatrix& parametersMatrix) {
	// check selected area has the correct size
	if ( parametersMatrix.ColumnsInStructure() < 9  || parametersMatrix.ColumnsInStructure() > 10 ||
		parametersMatrix.RowsInStructure() !=1 )
	{
		throw("Input matrix should be 1 x 8 or 1x9");
	}
	double Spot =  parametersMatrix(0,0).NumericValue();
	double r =  parametersMatrix(0,1).NumericValue();
	double d =  parametersMatrix(0,2).NumericValue();
	double vol =  parametersMatrix(0,3).NumericValue();
	double expiry = parametersMatrix(0,4).NumericValue(); 
	std::string name = parametersMatrix(0,5).StringValue();
	unsigned long NumberOfPaths = parametersMatrix(0,6).NumericValue();
	unsigned long NumberOfSteps = parametersMatrix(0,7).NumericValue();
	double Strike[2];
	Strike[0] = parametersMatrix(0,8).NumericValue();
	// extra value for double digital - can't use an if statement as Strike[1] goes out of scope
	Strike[1] = (parametersMatrix.ColumnsInStructure()==10) ? 
		parametersMatrix(0,9).NumericValue() : 110;
    PayOff* PayOffPtr = PayOffFactory::Instance().CreatePayOff(name,Strike); 
	VanillaOption theOption(*PayOffPtr, expiry);

    if (PayOffPtr != NULL)
    {
		ParametersConstant VolParam(vol);
		ParametersConstant rParam(r);
		StatisticsBasic gathererBasic;
		//StatisticsMean gatherer;
	//	ConvergenceTable gathererTwo(gatherer);

		RandomParkMiller generator(1); // this initializes a Random Number Generator of dimensionality given in brackets.
		// it's dimensionality is then reset in SimpleMonteCarlo7
//		AntiThetic GenTwo(generator);

		SimpleMonteCarlo7(theOption,
                                      Spot, 
                                      VolParam,
                                      rParam,
                                      NumberOfPaths, 
									  NumberOfSteps,
                                      gathererBasic,
									  generator);
        delete PayOffPtr;
		CellMatrix resultMatrix(1,3); 
		resultMatrix(0,0) = (gathererBasic.GetResultsSoFar()[0][0]); //mean
		resultMatrix(0,1) =  (gathererBasic.GetResultsSoFar()[0][1]); //var
		resultMatrix(0,2) =  (gathererBasic.GetResultsSoFar()[0][2]); //std error
		return resultMatrix;
		}
		else {
			CellMatrix resultMatrix(1,3);
			resultMatrix(0,0)=13.0; resultMatrix(0,1)=0.0; resultMatrix(0,2)=0.0; 
			return resultMatrix;
		}
}



CellMatrix // Return the price, variance and std error of vanilla option given spot, r, d, vol, expiry, name, numPaths, Strike
//<xlw:time
MCVanillaChoice1(const CellMatrix& parametersMatrix) {
return MCVanillaChoice3(parametersMatrix, 1UL);
}

CellMatrix // Return the delta of a plain vanilla call calculated by Monte Carlo pathwise method (see p.195 of Joshi Concepts)
//<xlw:time
MCVanillaDeltaCall(const CellMatrix& parametersMatrix) {
	double Spot =  parametersMatrix(0,0).NumericValue();
	CellMatrix returnMatrix = MCVanillaChoice3(parametersMatrix, 1UL);
	returnMatrix(0,0) =returnMatrix(0,0).NumericValue()/Spot; 
	return returnMatrix;
}


CellMatrix MCVanillaDeltaLR(const CellMatrix& parametersMatrix, unsigned long seed) {
	// check selected area has the correct size
	if ( parametersMatrix.ColumnsInStructure() < 8  || parametersMatrix.ColumnsInStructure() > 9 ||
		parametersMatrix.RowsInStructure() !=1 )
	{
		throw("Input matrix should be 1 x 8 or 1x9");
	}
	double Spot =  parametersMatrix(0,0).NumericValue();
	double r =  parametersMatrix(0,1).NumericValue();
	double d =  parametersMatrix(0,2).NumericValue();
	double vol =  parametersMatrix(0,3).NumericValue();
	double expiry = parametersMatrix(0,4).NumericValue(); 
	std::string name = parametersMatrix(0,5).StringValue();
	unsigned long NumberOfPaths = parametersMatrix(0,6).NumericValue();
	double Strike[2];
	Strike[0] = parametersMatrix(0,7).NumericValue();
	// extra value for double digital - can't use an if statement as Strike[1] goes out of scope
	Strike[1] = (parametersMatrix.ColumnsInStructure()==9) ? 
		parametersMatrix(0,8).NumericValue() : 110;
    PayOff* PayOffPtr = PayOffFactory::Instance().CreatePayOff(name,Strike); 
	VanillaOption theOption(*PayOffPtr, expiry);

    if (PayOffPtr != NULL)
    {
		ParametersConstant VolParam(vol);
		ParametersConstant rParam(r);
		StatisticsBasic gathererBasic;
		StatisticsMean gatherer;
		ConvergenceTable gathererTwo(gatherer);

		RandomParkMiller generator(1);
		generator.SetSeed(seed);
		AntiThetic GenTwo(generator);

		SimpleMonteCarloForLRDelta(theOption,
                                      Spot, 
                                      VolParam,
                                      rParam,
                                      NumberOfPaths,
                                      gathererBasic,
									  generator);
        delete PayOffPtr;
		CellMatrix resultMatrix(1,3); 
		resultMatrix(0,0) = (gathererBasic.GetResultsSoFar()[0][0]); //mean
		resultMatrix(0,1) =  (gathererBasic.GetResultsSoFar()[0][1]); //var
		resultMatrix(0,2) =  (gathererBasic.GetResultsSoFar()[0][2]); //std error
		return resultMatrix;
		}
		else {
			MyMatrix resultMatrix(1,3);
			resultMatrix(0,0)=0.0; resultMatrix(0,1)=0.0; resultMatrix(0,2)=0.0; 
			return resultMatrix;
		}
}


