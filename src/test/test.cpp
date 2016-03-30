
#include <node.h>
#include <nan.h>
#include <iostream>
// #include "uncapacitatedBSP.h"
// #include "capacitatedBSP.h"
#include "mergeBSP.h"


// using namespace   std;
// int main() {
// 	int STATION_NUM = 1;

// 	cout << "Input station num:" << endl;
// 	cin >> STATION_NUM;

// 	UncapacitatedBSP ubsp(STATION_NUM);
// 	CapacitatedBSP cbsp(STATION_NUM);
// 	MergeBSP mbsp(STATION_NUM);

// //	ubsp.runRandom();
// //	cbsp.runRandom();
// 	mbsp.runRandom();

// 	system("pause");

// 	return 0;
// }

int runMergeScript(int num,v8::Local<v8::Array> &retArr)
{
    MergeBSP mbsp(num); 
    mbsp.runRandom();
    retArr = Nan::New<v8::Array>(mbsp._finalPaht.size());
    for (int i = 0; i < mbsp._finalPaht.size(); i++) {  
  		Nan::Set(retArr, i, Nan::New(mbsp._finalPaht[i]));
	}
}

void runMerge(const Nan::FunctionCallbackInfo<v8::Value>& info) {
    
//    if (info.Length() != 1) {
//        Nan::ThrowTypeError("Wrong number of arguments");
//        return;
//    }
//    
//    if (!info[0]->IsNumber()) {
//        Nan::ThrowTypeError("Both arguments should be numbers");
//        return;
//    }
//    
    int arg0 = info[0]->NumberValue();
    
    double arg1 = info[1]->NumberValue();
    v8::Local<v8::Array> retArr;  
	Nan::New(runMergeScript(arg0,retArr));
	
	info.GetReturnValue().Set(retArr);  
}

void Init(v8::Local<v8::Object> exports) {
    exports->Set(Nan::New("runMerge").ToLocalChecked(),
                 Nan::New<v8::FunctionTemplate>(runMerge)->GetFunction());
}
NODE_MODULE(addon, Init)