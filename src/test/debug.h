
//#define DEBUG haha

#ifdef  DEBUG
#define PRINTFRandomDemand		printRandomDemand();
#define PRINTFRandomPoints		printRandomPoints();
//#define PRINTFCost				printCost();
#define PRINTFCost				;
#define PRINTFTSPtour			printTSPtour();
#define PRINTFSuperNode			printSuperNodeInformation();
#define PRINTFFinalPath			printFinalPath();
#define PRINTFReuslt			;
#else
#define PRINTFRandomDemand		;
#define PRINTFRandomPoints		;
#define PRINTFCost				;
#define PRINTFTSPtour			;
#define PRINTFSuperNode			;
#define PRINTFFinalPath			;
#define PRINTFReuslt			;
#endif