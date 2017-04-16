#include <stdio.h>

//IMA-ADPCM coding

void Encoder(int input[], int codeword[], int step, int index, int arr3[], int arr4[], int arr5[]);
int Predictor(int pred_sample);
int Quantizer(int diff, int step);
int Dequantizer(int code, int step);
void print_f(int arr[]);

int indexTable[16] = { -1,-1,-1,-1,2,4,6,8,
-1,-1,-1,-1,2,4,6,8 };

int stepTable[89] = { 7,8,9,10,11,12,13,14,16,17,
19,21,23,25,28,31,34,37,41,45,
50,55,60,66,73,80,88,97,107,118,
130,143,157,173,190,209,230,253,279,
307,337,371,408,449,494,544,598,658,
724,796,876,963,1060,1166,1282,1411,1552,
1707,1878,2066,2272,2499,2749,3024,3327,
3660,4026,4428,4871,5358,5894,6484,7132,
7845,8630,9493,10442,11487,12635,13899,
15289,16818,18500,20350,22385,
24623,27086,29794,32767 };

int mask = 0; //Q1. mask 가 무엇일까...

int  main(void) {
	int i = 0;
	int step = 50;
	int index = 20;
	int arr[5] = { 81,97,107,92,64 };
	int arr2[5] = { 0 }; //codeword
	int arr3[5] = { 0 }; //diff
	int arr4[5] = { 0 }; //pred_diff
	int arr5[5] = { 0 }; //pred_sample

	Encoder(arr, arr2, step, index,arr3,arr4,arr5);

	for (i = 0; i <5; i++) {
		printf("%d \t", i + 1);
	}
	printf("\n");
	print_f(arr2);
	print_f(arr3);
	print_f(arr4);
	print_f(arr5);



}

void Encoder(int input[], int codeword[],int step, int index, int arr3[], int arr4[], int arr5[]) { //(input = input[]-sample , output = codeword[]-4bit)
	int i = 0, diff = 0;
	int  pred_sample = 0, pred_diff = 0;

	while (i<5)  {

		//if (i == 0)	step = 7, index = 0, pred_sample = 0; // first time set
		
		
		diff = input[i] - Predictor(pred_sample); // difference - encoding
		arr3[i] = diff;
		codeword[i] = Quantizer(diff, step);  // count codeword

		pred_diff = Dequantizer(codeword[i], step); // do Dequantizer to recover pred_diff
		arr4[i] = pred_diff;
		pred_sample = pred_sample + pred_diff; // add pred_diff to recover standard pred_sample (손실발생-loss occurrence)
		arr5[i] = pred_sample;
		index += indexTable[(unsigned)codeword[i]]; //change index
		step = stepTable[index]; //change step
		
		i++; //count up
	}

}

int Predictor(int pred_sample) {
	int predictor = 0;

	predictor = pred_sample;
	return predictor;
}

int Quantizer(int diff, int step) {
	int i = 1; 
	int code = 0;

	if (diff < 0) {
		diff = -diff;
		code = 8;
	}

	mask = 4;

	while (1 <= i&&i <= 3) {
		if (diff >= step) {
			code |= mask;
			diff = diff - step;
		}
		step = step >> 1;
		mask = mask >> 1;
		i++;
		}
	
	return code;
}

int Dequantizer(int code, int step) {
	int recon = 0;
	mask = 4;
	int i = 1;

	while (1 <= i&&i <= 3) {
		if ((masking(code, mask)) == 1) recon = recon + step;
		step = step >> 1;
		mask = mask >> 1;
		i++;
	}

	recon = recon + step;

	if (masking(code, 8) == 1) recon = -recon;

	return recon;
}


int masking(int code, int mask) {
	int codemask = 0;

	codemask = code&mask;
	
	switch (codemask)
	{
	case 8 : case 4 : case 2 : case 1 :
		return 1;
		
	default: return 0;
		break;
	}
	return 0;
}

void print_f(int arr[]) {
	int i = 0;

	for (i = 0; i <5; i++) {
		printf("%d \t", arr[i]);
	}
	printf("\n");

}



