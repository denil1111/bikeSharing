
#include <node.h>
#include <nan.h>
#include <iostream>
#include "uncapacitatedBSP.h"
#include "capacitated.h"
#include "ktimescapacitatedBSP.h"
#include "noZeroCapacitatedBSP.h"
#include "bspbase.h"


TspBase *MainTSP = nullptr;
// int randomDataScript(int num,v8::Local<v8::Array> &retArr)
// {

// 	MainTSP = new MergeBSP(num);
// 	MainTSP->randomData();
// 	retArr = Nan::New<v8::Array>(num);
// 	for (int i = 0; i < num;i++) {  
// 		v8::Local<v8::Object> pointI = Nan::New<v8::Object>();
// 		std::cout<<MainTSP->_point[i].a<<std::endl;
// 		Nan::Set(pointI,Nan::New("x").ToLocalChecked(),
//                 Nan::New(MainTSP->_point[i].a));
// 		Nan::Set(pointI,Nan::New("y").ToLocalChecked(),
//                 Nan::New(MainTSP->_point[i].b));
// 		Nan::Set(pointI,Nan::New("d").ToLocalChecked(),
//                 Nan::New(MainTSP->_stationDemand[i]));
//   		Nan::Set(retArr, i, pointI);
// 	}

// }
// void randomData(const Nan::FunctionCallbackInfo<v8::Value>& info) {
    
// //    if (info.Length() != 1) {
// //        Nan::ThrowTypeError("Wrong number of arguments");
// //        return;
// //    }
// //    
// //    if (!info[0]->IsNumber()) {
// //        Nan::ThrowTypeError("Both arguments should be numbers");
// //        return;
// //    }
// //  
// 	std::cout<<"go!"<<std::endl;
//     int arg0 = info[0]->NumberValue();
//     v8::Local<v8::Array> retArr;  
// 	Nan::New(randomDataScript(arg0,retArr));
	
// 	info.GetReturnValue().Set(retArr);  
// }

void runAlgorithm(const Nan::FunctionCallbackInfo<v8::Value>& info)
{  
    v8::Local<v8::Object> ret = Nan::New<v8::Object>();  
    int arg0 = info[0]->NumberValue();
    // if (arg0 == 0)
    // {
    //     UncapacitatedBSP ubsp(*MainTSP);
    //     ubsp.run();
    // }
    std::vector<StationidAndDemand> resultPath;
    BspBase *bsp;
    switch(arg0)
    {
        case 1: bsp = new KTimesCapacitatedBSP(*MainTSP);break;
        case 2: bsp = new CapacitatedBSP(*MainTSP);break;
        case 3: bsp = new NoZeroCapacitatedBSP(*MainTSP);break;
    }
    bsp->run();
    resultPath = bsp->_minCostPath;
    v8::Local<v8::Array> retResultPath = Nan::New<v8::Array>(resultPath.size());
    for (int i = 0; i < resultPath.size(); i++) {
        v8::Local<v8::Object> pointI = Nan::New<v8::Object>();
        Nan::Set(pointI,Nan::New("id").ToLocalChecked(),
                 Nan::New(resultPath[i].stationId));
        Nan::Set(pointI,Nan::New("d").ToLocalChecked(),
                 Nan::New(resultPath[i].stationDemand));
  		Nan::Set(retResultPath, i, pointI);
	}
    std::cout<<"result record success"<<std::endl;
	info.GetReturnValue().Set(retResultPath);  
}

void input(const Nan::FunctionCallbackInfo<v8::Value>& info) {
    int stationNum = info[0]->NumberValue();
    int capacity = info[1]->NumberValue();
    std::cout<<"capacity:"<<capacity<<std::endl;
    MainTSP = new TspBase(stationNum,100,capacity);
    for (int i=0;i<stationNum;i++)
    {
        point x;
        x.a = info[2]->ToObject()->Get(i)->ToObject()->Get(Nan::New("x").ToLocalChecked())->NumberValue();
        x.b = info[2]->ToObject()->Get(i)->ToObject()->Get(Nan::New("y").ToLocalChecked())->NumberValue();
        int demand = info[2]->ToObject()->Get(i)->ToObject()->Get(Nan::New("d").ToLocalChecked())->NumberValue(); 
        MainTSP->_point.push_back(x);
        MainTSP->_allStationDemand.push_back(demand);
        std::cout<<"x:"<<x.a<<","<<x.b<<"d:"<<demand<<std::endl;
    }
    for (int i=0;i<stationNum+1;i++) {
        std::vector<double> v;
        for (int j=0;j<stationNum+1;j++) { 
            v.push_back(info[3]->ToObject()->Get(i)->ToObject()->Get(j)->NumberValue());
            std::cout<<" "<<v[j];
        }
        MainTSP->_cost.push_back(v);
        std::cout<<std::endl;
    }
    std::vector<int> tspPath;
    MainTSP->data();
    tspPath = MainTSP->_path;
    v8::Local<v8::Array> retTspPath = Nan::New<v8::Array>(tspPath.size());
    for (int i = 0;i<tspPath.size();i++) {
         v8::Local<v8::Object> pointI = Nan::New<v8::Object>();
        Nan::Set(pointI,Nan::New("id").ToLocalChecked(),
                 Nan::New(tspPath[i]));
        Nan::Set(pointI,Nan::New("d").ToLocalChecked(),
                 Nan::New(0));
  		Nan::Set(retTspPath, i, pointI);
    }
    info.GetReturnValue().Set(retTspPath);  
    std::cout<<"tsp record success"<<std::endl;
    
}
void Init(v8::Local<v8::Object> exports) {
    exports->Set(Nan::New("runAlgorithm").ToLocalChecked(),
                 Nan::New<v8::FunctionTemplate>(runAlgorithm)->GetFunction());
    // exports->Set(Nan::New("randomData").ToLocalChecked(),
                //  Nan::New<v8::FunctionTemplate>(randomData)->GetFunction());
    exports->Set(Nan::New("input").ToLocalChecked(),
                 Nan::New<v8::FunctionTemplate>(input)->GetFunction());

}
NODE_MODULE(addon, Init)
