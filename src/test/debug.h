
//#define DEBUG haha

#ifdef  DEBUG
#define PRINTFRandomDemand		printRandomDemand();
#define PRINTFRandomPoints		printRandomPoints();
#define PRINTFCost				printCost();
#define PRINTFTSPtour			printTSPtour();
#define PRINTFSuperNode			printSuperNodeInformation();
#define PRINTFFinalPath			printFinalPaht();
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