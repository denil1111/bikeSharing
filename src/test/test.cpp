
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
MergeBSP *MainBSP = nullptr;
int randomDataScript(int num,v8::Local<v8::Array> &retArr)
{

	MainBSP = new MergeBSP(num);
	MainBSP->randomData();
	retArr = Nan::New<v8::Array>(num);
	for (int i = 0; i < num;i++) {  
		v8::Local<v8::Object> pointI = Nan::New<v8::Object>();
		std::cout<<MainBSP->_point[i].a<<std::endl;
		Nan::Set(pointI,Nan::New("x").ToLocalChecked(),
                Nan::New(MainBSP->_point[i].a));
		Nan::Set(pointI,Nan::New("y").ToLocalChecked(),
                Nan::New(MainBSP->_point[i].b));
		Nan::Set(pointI,Nan::New("d").ToLocalChecked(),
                Nan::New(MainBSP->_stationDemand[i]));
  		Nan::Set(retArr, i, pointI);
	}

}
void randomData(const Nan::FunctionCallbackInfo<v8::Value>& info) {
    
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
	std::cout<<"go!"<<std::endl;
    int arg0 = info[0]->NumberValue();
    v8::Local<v8::Array> retArr;  
	Nan::New(randomDataScript(arg0,retArr));
	
	info.GetReturnValue().Set(retArr);  
}
int runMergeScript(v8::Local<v8::Array> &retArr)
{
    MainBSP->run();
    retArr = Nan::New<v8::Array>(MainBSP->_minCostPath.size());
    for (int i = 0; i < MainBSP->_minCostPath.size(); i++) {
        v8::Local<v8::Object> pointI = Nan::New<v8::Object>();
        Nan::Set(pointI,Nan::New("id").ToLocalChecked(),
                 Nan::New(MainBSP->_minCostPath[i].stationId));
        Nan::Set(pointI,Nan::New("d").ToLocalChecked(),
                 Nan::New(MainBSP->_minCostPath[i].stationDemand));
  		Nan::Set(retArr, i, pointI);
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
    // int arg0 = info[0]->NumberValue();
    
    // double arg1 = info[1]->NumberValue();
    v8::Local<v8::Array> retArr;  
	Nan::New(runMergeScript(retArr));
	
	info.GetReturnValue().Set(retArr);  
}

void input(const Nan::FunctionCallbackInfo<v8::Value>& info) {
    int stationNum = info[0]->NumberValue();
    MainBSP = new MergeBSP(stationNum);
    for (int i=0;i<stationNum;i++)
    {
        point x;
        x.a = info[1]->ToObject()->Get(i)->ToObject()->Get(Nan::New("x").ToLocalChecked())->NumberValue();
        x.b = info[1]->ToObject()->Get(i)->ToObject()->Get(Nan::New("y").ToLocalChecked())->NumberValue();
        int demand = info[1]->ToObject()->Get(i)->ToObject()->Get(Nan::New("d").ToLocalChecked())->NumberValue(); 
        MainBSP->_point.push_back(x);
        MainBSP->_stationDemand.push_back(demand);
        std::cout<<"x:"<<x.a<<","<<x.b<<"d:"<<demand<<std::endl;
    }
    for (int i=0;i<stationNum+1;i++) {
        std::vector<double> v;
        for (int j=0;j<stationNum+1;j++) { 
            v.push_back(info[2]->ToObject()->Get(i)->ToObject()->Get(j)->NumberValue());
            std::cout<<" "<<v[j];
        }
        MainBSP->_cost.push_back(v);
        std::cout<<std::endl;
    }
}
void Init(v8::Local<v8::Object> exports) {
    exports->Set(Nan::New("runMerge").ToLocalChecked(),
                 Nan::New<v8::FunctionTemplate>(runMerge)->GetFunction());
    exports->Set(Nan::New("randomData").ToLocalChecked(),
                 Nan::New<v8::FunctionTemplate>(randomData)->GetFunction());
    exports->Set(Nan::New("input").ToLocalChecked(),
                 Nan::New<v8::FunctionTemplate>(input)->GetFunction());

}
NODE_MODULE(addon, Init)
