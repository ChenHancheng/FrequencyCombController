#include "Kalman.h"

int Q = 20;
int R = 35;
int EstiP = 80;
int PreP;

int PreValue;
int EstiValue;

float K;

int First=1;

int KalmanFilter(int measurement){
	
	if(First == 1){
		First = 0;
		if( measurement<4000 && measurement >1600) EstiValue = measurement;
		else EstiValue = ADVALUE_MID;
	}
	PreValue = EstiValue;
	PreP = EstiP+Q;
	K = PreP/(PreP+R);
	
	EstiValue = measurement+K*(PreValue-measurement);
	EstiP = (1-K)*PreP;
	
	return(EstiValue);
}
