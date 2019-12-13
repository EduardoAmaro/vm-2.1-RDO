#include "Block4D.h"
#include <iostream>
#include <fstream>

using namespace std;

class Prediction 
{
public:
	ifstream DCPredictorFile;
	int predictionType, counter;

	Prediction(int prediction);
	int simplePredictor(Block4D *origBlock);
	int DCPredictorRefPlaneRaster(Block4D *origBlock, int verticalView);
	void saveSamplesMule(Block4D *residueBlock, Block4D *origBlock, int spectralComponent);
	void reconstruct4DBlock(Block4D *residueBlock, int DCPredictor);
	void fourRefsPredictor(Block4D *residueBlock, Block4D *origBlock, Block4D *ref0, Block4D *ref1, Block4D *ref2, Block4D *ref3);
	void calculateResidue(Block4D *residueBlock, Block4D *origBlock, int DCPredictor);
	void differentialPredictionRaster(Block4D *residueBlock, Block4D *origBlock, int spectralComponent);
	void recDifferentialPredictionRaster(Block4D *residueBlock, Block4D *reconstructedBlock);
	void differentialPredictionRasterDCRefPlane(Block4D *residueBlock, Block4D *origBlock, int spectralComponent);
	void recDifferentialPredictionRasterDCRefPlane(Block4D *residueBlock, Block4D *reconstructedBlock);
	void differentialPredictionCentral(Block4D *residueBlock, Block4D *origBlock, int spectralComponent);
	void recDifferentialPredictionCentral(Block4D *recBlock, Block4D *origBlock);
	void differentialPredictionCentralDCRefPlane(Block4D *residueBlock, Block4D *origBlock, int spectralComponent);
	void recDifferentialPredictionCentralDCRefPlane(Block4D *recBlock, Block4D *origBlock);
	void differentialPredictionRasterHalf(Block4D *residueBlock, Block4D *origBlock);
	void recDifferentialPredictionRasterHalf(Block4D *recBlock, Block4D *origBlock);
	void hierarchicalDifferentialPrediction(Block4D *residueBlock, Block4D *origBlock);
	void recHierarchicalDifferentialPrediction(Block4D *residueBlock, Block4D *reconstructedBlock);
	void hierarchicalDifferentialPrediction1Level(Block4D *residueBlock, Block4D *origBlock);
	void recHierarchicalDifferentialPrediction1Level(Block4D *recBlock, Block4D *origBlock);
};