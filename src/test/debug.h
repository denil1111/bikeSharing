
//#define DEBUG haha

#ifdef  DEBUG
#define PRINTFDemand			printDemand();
#define PRINTFPoints			printPoints();
//#define PRINTFCost				printCost();
#define PRINTFTSPtour			printTSPtour();
#define PRINTFSuperNode			printSuperNodeInformation();
#define PRINTFFinalPath			printFinalPath();
#define PRINTFReuslt			;
#else
#define PRINTFDemand			;
#define PRINTFPoints			;
#define PRINTFCost				;
#define PRINTFTSPtour			;
#define PRINTFSuperNode			;
#define PRINTFReuslt			;
#define PRINTFFinalPath			;
#endif