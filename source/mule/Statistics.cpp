#include "Statistics.h"
#include "Block4D.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include <string.h>
#include <climits>

using namespace std;

Statistics :: Statistics(int prediction) {
	predictionType = prediction;

	yRefPlaneSamples = new int[refPlaneTotalSamples];
	cbRefPlaneSamples = new int[refPlaneTotalSamples];
	crRefPlaneSamples = new int[refPlaneTotalSamples];
	yOtherPlanesSamples = new int[otherPlanesTotalSamples];
	cbOtherPlanesSamples = new int[otherPlanesTotalSamples];
	crOtherPlanesSamples = new int[otherPlanesTotalSamples];

	counterBcDecrease = 0;
	counterSplitHexaDecaTree = 0;
	partitioningCounter = 0;
	zeroCoefficientsCounter = 0;
	totalCoefficientsCounter = 0;
	yCounterRP = 0;
	cbCounterRP = 0;
	crCounterRP = 0;
	yCounterOP = 0;
	cbCounterOP = 0;
	crCounterOP = 0;
	coeffEnergy = 0;
	sumCoeff = 0;
	y_totalSignalEnergyRefPlane = 0;
	cb_totalSignalEnergyRefPlane = 0;
	cr_totalSignalEnergyRefPlane = 0;
	y_totalSignalEnergyOtherPlanes = 0;
	cb_totalSignalEnergyOtherPlanes = 0;
	cr_totalSignalEnergyOtherPlanes = 0;
	y_refPlaneSamplesSum = 0;
	cb_refPlaneSamplesSum = 0;
	cr_refPlaneSamplesSum = 0;
	y_otherPlanesSamplesSum = 0;
	cb_otherPlanesSamplesSum = 0;
	cr_otherPlanesSamplesSum = 0;

	maxRefPlane = INT_MIN;
	minRefPlane = INT_MAX;
	maxOtherPlanes = INT_MIN;
	minOtherPlanes = INT_MAX;
	maxCoeff = INT_MIN;
	minCoeff = INT_MAX;
}

int Statistics :: getPartitioningCounter() {
	return partitioningCounter;
}

int Statistics :: getMaxCoeff() {
	return maxCoeff;
}

int Statistics :: getMinCoeff() {
	return minCoeff;
}

int Statistics :: getMaxRefPlane() {
	return maxRefPlane;
}

int Statistics :: getMinRefPlane() {
	return minRefPlane;
}

int Statistics :: getMaxOtherPlanes() {
	return maxOtherPlanes;
}

int Statistics :: getMinOtherPlanes() {
	return minOtherPlanes;
}

int Statistics :: getBcDecreaseCounter() {
	return counterBcDecrease;
}

int Statistics :: getSplitHexaDecaTreeCounter() {
	return counterSplitHexaDecaTree;
}

double Statistics :: getZeroCoefficients() {
	return zeroCoefficientsCounter;
}

double Statistics :: getTotalCoefficients() {
	return totalCoefficientsCounter;
}

double Statistics :: getYRefPlaneSamplesAverage() {
	return y_refPlaneSamplesSum/refPlaneTotalSamples;
}

double Statistics :: getCbRefPlaneSamplesAverage() {
	return cb_refPlaneSamplesSum/refPlaneTotalSamples;
}

double Statistics :: getCrRefPlaneSamplesAverage() {
	return cr_refPlaneSamplesSum/refPlaneTotalSamples;
}

double Statistics :: getYOtherPlanesSamplesAverage() {
	return y_otherPlanesSamplesSum/otherPlanesTotalSamples;
}

double Statistics :: getCbOtherPlanesSamplesAverage() {
	return cb_otherPlanesSamplesSum/otherPlanesTotalSamples;
}

double Statistics :: getCrOtherPlanesSamplesAverage() {
	return cr_otherPlanesSamplesSum/otherPlanesTotalSamples;
}

double Statistics :: getAverageRefPlaneCoeff() {
	return sumRefPlaneCoeff/refPlaneTotalSamples;
}

double Statistics :: getAverageOtherPlanesCoeff() {
	return sumOtherPlanesCoeff/otherPlanesTotalSamples;
}

double Statistics :: getYRefPlaneEnergy() {
	return y_totalSignalEnergyRefPlane;
}

double Statistics :: getCbRefPlaneEnergy() {
	return cb_totalSignalEnergyRefPlane;
}

double Statistics :: getCrRefPlaneEnergy() {
	return cr_totalSignalEnergyRefPlane;
}

double Statistics :: getYOtherPlanesEnergy() {
	return y_totalSignalEnergyOtherPlanes;
}

double Statistics :: getCbOtherPlanesEnergy() {
	return cb_totalSignalEnergyOtherPlanes;
}

double Statistics :: getCrOtherPlanesEnergy() {
	return cr_totalSignalEnergyOtherPlanes;
}

double Statistics :: getEnergyCoeff() {
	return coeffEnergy;
}

void Statistics :: incSplitHexaDecaTree() {
	counterSplitHexaDecaTree++;
}

void Statistics :: incBcDecrease() {
	counterBcDecrease++;
}

void Statistics :: printOneBlock(Block4D *lfBlock) {
	for(int viewLine = 0; viewLine < 15; viewLine += 1) {
		for(int viewColumn = 0; viewColumn < 15; viewColumn += 1) {
			printf("[coeff]: %d\n", lfBlock->mPixel[3][3][viewLine][viewColumn]);
		}
	}
}

void Statistics :: calcCoeffsPerBitPlane(Block4D *lfBlock) {
	int quantizedCoeff;
	int bitplane = 0;

	for(int verticalView = 0; verticalView < 13; verticalView += 1) {
		for(int horizontalView = 0; horizontalView < 13; horizontalView += 1) {
			for(int viewLine = 0; viewLine < 15; viewLine += 1) {
				for(int viewColumn = 0; viewColumn < 15; viewColumn += 1) {
					bitplane = 0;
					quantizedCoeff = 1;
					int coeff = lfBlock->mPixel[horizontalView][verticalView][viewColumn][viewLine];
					
					if(coeff < 0) {
						coeff = -coeff;
					}


					if(coeff == 0) {
						coeffsPerBitPlanes[0]++;
					}
					else {
						while(quantizedCoeff > 0) {
							bitplane++;
							quantizedCoeff = coeff >> bitplane;
							//printf("qMag: %d\n", quantizedCoeff);
						}

						coeffsPerBitPlanes[bitplane]++;
					}
				}
			}
		}
	}
}

void Statistics :: getCoeffsPerBitPlane() {
	for(int i = 0; i < 31; i++) {
		printf("\t\t\t\tBitplane %d: %d\n", i, coeffsPerBitPlanes[i]);
	}
}

void Statistics :: calcReferencePlaneEnergy(Block4D *lfBlock, int spectralComponent) {
	int samples;

	for(int verticalView = 0; verticalView < 13; verticalView += 1) {
		for(int viewLine = 0; viewLine < 15; viewLine += 1) {
			for(int viewColumn = 0; viewColumn < 15; viewColumn += 1) {
				if(predictionType == 1 || predictionType == 4) {
					samples = lfBlock->mPixel[6][verticalView][viewColumn][viewLine];
				}
				else {
					samples = lfBlock->mPixel[0][verticalView][viewColumn][viewLine];
				}

				if(spectralComponent == 0)
					y_totalSignalEnergyRefPlane += samples*samples;
				if(spectralComponent == 1)
					cb_totalSignalEnergyRefPlane += samples*samples;
				if(spectralComponent == 2)
					cr_totalSignalEnergyRefPlane += samples*samples;

				if(samples > maxRefPlane) {
					maxRefPlane = samples;
				}
				if(samples < minRefPlane) {
					minRefPlane = samples;
				}
			}
		}
	}
}

void Statistics :: calcOtherPlanesEnergy(Block4D *lfBlock, int spectralComponent) {
	for(int verticalView = 0; verticalView < 13; verticalView += 1) {
		for(int horizontalView = 0; horizontalView < 13; horizontalView += 1) {
			for(int viewLine = 0; viewLine < 15; viewLine += 1) {
				for(int viewColumn = 0; viewColumn < 15; viewColumn += 1) {
					if(predictionType == 1 || predictionType == 4) {
						//printf("\tdiffC\n");
						if(horizontalView != 6) {
							int samples = lfBlock->mPixel[horizontalView][verticalView][viewColumn][viewLine];
							
							if(spectralComponent == 0)
								y_totalSignalEnergyOtherPlanes += samples*samples;
							if(spectralComponent == 1)
								cb_totalSignalEnergyOtherPlanes += samples*samples;
							if(spectralComponent == 2)
								cr_totalSignalEnergyOtherPlanes += samples*samples;

							if(samples > maxOtherPlanes) {
								maxOtherPlanes = samples;
							}
							if(samples < minOtherPlanes) {
								minOtherPlanes = samples;
							}
						}
					}
					else {
						if(horizontalView != 0) {
							int samples = lfBlock->mPixel[horizontalView][verticalView][viewColumn][viewLine];
							
							if(spectralComponent == 0)
								y_totalSignalEnergyOtherPlanes += samples*samples;
							if(spectralComponent == 1)
								cb_totalSignalEnergyOtherPlanes += samples*samples;
							if(spectralComponent == 2)
								cr_totalSignalEnergyOtherPlanes += samples*samples;

							if(samples > maxOtherPlanes) {
								maxOtherPlanes = samples;
							}
							if(samples < minOtherPlanes) {
								minOtherPlanes = samples;
							}
						}
					}
				}
			}
		}
	}
}

void Statistics :: calcSumYRefPlaneSamples(Block4D *lfBlock) {
	int sample;

	for(int verticalView = 0; verticalView < 13; verticalView += 1) {
		for(int viewLine = 0; viewLine < 15; viewLine += 1) {
			for(int viewColumn = 0; viewColumn < 15; viewColumn += 1) {
				if(predictionType == 1 || predictionType == 4) {
					sample = lfBlock->mPixel[6][verticalView][viewColumn][viewLine];
				}
				else {
					sample = lfBlock->mPixel[0][verticalView][viewColumn][viewLine];
				}

				y_refPlaneSamplesSum += sample;

				yRefPlaneSamples[yCounterRP] = sample;
				yCounterRP++;
			}
		}
	}
}

void Statistics :: calcSumCbRefPlaneSamples(Block4D *lfBlock) {
	int sample;

	for(int verticalView = 0; verticalView < 13; verticalView += 1) {
		for(int viewLine = 0; viewLine < 15; viewLine += 1) {
			for(int viewColumn = 0; viewColumn < 15; viewColumn += 1) {
				if(predictionType == 1 || predictionType == 4) {
					sample = lfBlock->mPixel[6][verticalView][viewColumn][viewLine];
				}
				else {
					sample = lfBlock->mPixel[0][verticalView][viewColumn][viewLine];
				}

				cb_refPlaneSamplesSum += sample;

				cbRefPlaneSamples[cbCounterRP] = sample;
				cbCounterRP++;
			}
		}
	}
}

void Statistics :: calcSumCrRefPlaneSamples(Block4D *lfBlock) {
	int sample;

	for(int verticalView = 0; verticalView < 13; verticalView += 1) {
		for(int viewLine = 0; viewLine < 15; viewLine += 1) {
			for(int viewColumn = 0; viewColumn < 15; viewColumn += 1) {
				if(predictionType == 1 || predictionType == 4) {
					sample = lfBlock->mPixel[6][verticalView][viewColumn][viewLine];
				}
				else {
					sample = lfBlock->mPixel[0][verticalView][viewColumn][viewLine];
				}

				cr_refPlaneSamplesSum += sample;
				
				crRefPlaneSamples[crCounterRP] = sample;
				crCounterRP++;
			}
		}
	}
}

void Statistics :: calcSumYOtherPlanesSamples(Block4D *lfBlock) {
	int sample;

	for(int verticalView = 0; verticalView < 13; verticalView += 1) {
		for(int horizontalView = 0; horizontalView < 13; horizontalView += 1) {
			for(int viewLine = 0; viewLine < 15; viewLine += 1) {
				for(int viewColumn = 0; viewColumn < 15; viewColumn += 1) {
					if(predictionType == 1 || predictionType == 4) {
						if(horizontalView != 6)
							sample = lfBlock->mPixel[horizontalView][verticalView][viewColumn][viewLine];
					}
					else {
						if(horizontalView != 0)
							sample = lfBlock->mPixel[horizontalView][verticalView][viewColumn][viewLine];
					}

					y_otherPlanesSamplesSum += sample;
					
					yOtherPlanesSamples[yCounterOP] = sample;
					yCounterOP++;
				}
			}
		}
	}
}

void Statistics :: calcSumCbOtherPlanesSamples(Block4D *lfBlock) {
	int sample;
	
	for(int verticalView = 0; verticalView < 13; verticalView += 1) {
		for(int horizontalView = 1; horizontalView < 13; horizontalView += 1) {
			for(int viewLine = 0; viewLine < 15; viewLine += 1) {
				for(int viewColumn = 0; viewColumn < 15; viewColumn += 1) {
					if(predictionType == 1 || predictionType == 4) {
						if(horizontalView != 6)
							sample = lfBlock->mPixel[horizontalView][verticalView][viewColumn][viewLine];
					}
					else {
						if(horizontalView != 0)
							sample = lfBlock->mPixel[horizontalView][verticalView][viewColumn][viewLine];
					}

					cb_otherPlanesSamplesSum += sample;

					cbOtherPlanesSamples[cbCounterOP] = sample;
					cbCounterOP++;
				}
			}
		}
	}
}

void Statistics :: calcSumCrOtherPlanesSamples(Block4D *lfBlock) {
	int sample;

	for(int verticalView = 0; verticalView < 13; verticalView += 1) {
		for(int horizontalView = 1; horizontalView < 13; horizontalView += 1) {
			for(int viewLine = 0; viewLine < 15; viewLine += 1) {
				for(int viewColumn = 0; viewColumn < 15; viewColumn += 1) {
					if(predictionType == 1 || predictionType == 4) {
						if(horizontalView != 6)
							sample = lfBlock->mPixel[horizontalView][verticalView][viewColumn][viewLine];
					}
					else {
						if(horizontalView != 0)
							sample = lfBlock->mPixel[horizontalView][verticalView][viewColumn][viewLine];
					}

					cr_otherPlanesSamplesSum += sample;

					crOtherPlanesSamples[crCounterOP] = sample;
					crCounterOP++;
				}
			}
		}
	}
}

double Statistics :: calcStdYRefPlane() {
	double stdDev = 0;
	double average = (double)y_refPlaneSamplesSum/refPlaneTotalSamples;

	for(int i = 0; i < refPlaneTotalSamples; i++) {
		stdDev += pow(yRefPlaneSamples[i] - average, 2);
	}

	return sqrt(stdDev/refPlaneTotalSamples);
}

double Statistics :: calcStdCbRefPlane() {
	double stdDev = 0;
	double average = (double)cb_refPlaneSamplesSum/refPlaneTotalSamples;

	for(int i = 0; i < refPlaneTotalSamples; i++) {
		stdDev += pow(cbRefPlaneSamples[i] - average, 2);
	}
	
	return sqrt(stdDev/refPlaneTotalSamples);
}

double Statistics :: calcStdCrRefPlane() {
	double stdDev = 0;
	double average = (double)cr_refPlaneSamplesSum/refPlaneTotalSamples;

	for(int i = 0; i < refPlaneTotalSamples; i++) {
		stdDev += pow(crRefPlaneSamples[i] - average, 2);
	}
	
	return sqrt(stdDev/refPlaneTotalSamples);
}

double Statistics :: calcStdYOtherPlanes() {
	double stdDev = 0;
	double average = (double)y_otherPlanesSamplesSum/otherPlanesTotalSamples;

	for(int i = 0; i < otherPlanesTotalSamples; i++) {
		stdDev += pow(yOtherPlanesSamples[i] - average, 2);
	}
	
	return sqrt(stdDev/otherPlanesTotalSamples);
}

double Statistics :: calcStdCbOtherPlanes() {
	double stdDev = 0;
	double average = (double)cb_otherPlanesSamplesSum/otherPlanesTotalSamples;

	for(int i = 0; i < otherPlanesTotalSamples; i++) {
		stdDev += pow(cbOtherPlanesSamples[i] - average, 2);
	}

	return sqrt(stdDev/otherPlanesTotalSamples);
}

double Statistics :: calcStdCrOtherPlanes() {
	double stdDev = 0;
	double average = (double)cr_otherPlanesSamplesSum/otherPlanesTotalSamples;

	for(int i = 0; i < otherPlanesTotalSamples; i++) {
		stdDev += pow(crOtherPlanesSamples[i] - average, 2);
	}
	
	return sqrt(stdDev/otherPlanesTotalSamples);
}

void Statistics :: calcSumCoeff(Block4D *transformedBlock) {
for(int verticalView = 0; verticalView < 13; verticalView += 1) {
		for(int horizontalView = 0; horizontalView < 13; horizontalView += 1) {
			for(int viewLine = 0; viewLine < 15; viewLine += 1) {
				for(int viewColumn = 0; viewColumn < 15; viewColumn += 1) {
					int coeff = transformedBlock->mPixel[horizontalView][verticalView][viewColumn][viewLine];
					sumCoeff += coeff;

					if(coeff > maxCoeff)
						maxCoeff = coeff;
					else if(coeff < minCoeff)
						minCoeff = coeff;
				}
			}
		}
	}
}

void Statistics :: calcCoeffEnergy(Block4D *transformedBlock) {
	for(int verticalView = 0; verticalView < 13; verticalView += 1) {
		for(int horizontalView = 0; horizontalView < 13; horizontalView += 1) {
			for(int viewLine = 0; viewLine < 15; viewLine += 1) {
				for(int viewColumn = 0; viewColumn < 15; viewColumn += 1) {
					int coeff = transformedBlock->mPixel[horizontalView][verticalView][viewColumn][viewLine];
					
					coeffEnergy += coeff*coeff;
				}
			}
		}
	}
}

void Statistics :: countCoefficients(Block4D *transformedBlock) {
	for(int verticalView = 0; verticalView < 13; verticalView += 1) {
		for(int horizontalView = 0; horizontalView < 13; horizontalView += 1) {
			for(int viewLine = 0; viewLine < 15; viewLine += 1) {
				for(int viewColumn = 0; viewColumn < 15; viewColumn += 1) {
					int coeff = transformedBlock->mPixel[horizontalView][verticalView][viewColumn][viewLine];

					if(coeff == 0) {
						zeroCoefficientsCounter++;
					}

					totalCoefficientsCounter++;
				}
			}
		}
	}
}

void Statistics :: countPartitioning(char *partitionCode) {
	if(strcmp(partitionCode, "T") != 0) {
		partitioningCounter++;
	}
}