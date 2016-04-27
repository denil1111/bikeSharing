
#include <node.h>
#include <nan.h>
#include <iostream>
#include "uncapacitatedBSP.h"
#include "capacitated.h"
#include "ktimescapacitatedBSP.h"
#include "NoZeroCapacitatedBSP.h"
#include "bspbase.h"


TspBase *MainBSP = nullptr;
// int randomDataScript(int num,v8::Local<v8::Array> &retArr)
// {

// 	MainBSP = new MergeBSP(num);
// 	MainBSP->randomData();
// 	retArr = Nan::New<v8::Array>(num);
// 	for (int i = 0; i < num;i++) {  
// 		v8::Local<v8::Object> pointI = Nan::New<v8::Object>();
// 		std::cout<<MainBSP->_point[i].a<<std::endl;
// 		Nan::Set(pointI,Nan::New("x").ToLocalChecked(),
//                 Nan::New(MainBSP->_point[i].a));
// 		Nan::Set(pointI,Nan::New("y").ToLocalChecked(),
//                 Nan::New(MainBSP->_point[i].b));
// 		Nan::Set(pointI,Nan::New("d").ToLocalChecked(),
//                 Nan::New(MainBSP->_stationDemand[i]));
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
    //     UncapacitatedBSP ubsp(*MainBSP);
    //     ubsp.run();
    // }
    std::vector<StationidAndDemand> resultPath;
    std::vector<int> tspPath;
    BspBase *bsp;
    switch(arg0)
    {
        case 1: bsp = new KTimesCapacitatedBSP(*MainBSP);break;
        case 2: bsp = new CapacitatedBSP(*MainBSP);break;
        case 3: bsp = new NoZeroCapacitatedBSP(*MainBSP);break;
    }
    bsp->run();
    resultPath = bsp->_minCostPath;
    tspPath = bsp->_tspBase._path;
    v8::Local<v8::Array> retResultPath = Nan::New<v8::Array>(resultPath.size());
    v8::Local<v8::Array> retTspPath = Nan::New<v8::Array>(tspPath.size());
    for (int i = 0; i < resultPath.size(); i++) {
        v8::Local<v8::Object> pointI = Nan::New<v8::Object>();
        Nan::Set(pointI,Nan::New("id").ToLocalChecked(),
                 Nan::New(resultPath[i].stationId));
        Nan::Set(pointI,Nan::New("d").ToLocalChecked(),
                 Nan::New(resultPath[i].stationDemand));
  		Nan::Set(retResultPath, i, pointI);
	}
    std::cout<<"result record success"<<std::endl;
    for (int i = 0;i<tspPath.size();i++) {
        Nan::Set(retTspPath, i, Nan::New(tspPath[i]));
    }
    std::cout<<"tsp record success"<<std::endl;
    Nan::Set(ret,Nan::New("result").ToLocalChecked(),retResultPath);
    Nan::Set(ret,Nan::New("tsp").ToLocalChecked(),retTspPath);             
    std::cout<<"json create success"<<std::endl;
	info.GetReturnValue().Set(ret);  
}

void input(const Nan::FunctionCallbackInfo<v8::Value>& info) {
    int stationNum = info[0]->NumberValue();
    int capacity = info[1]->NumberValue();
    std::cout<<"capacity:"<<capacity<<std::endl;
    MainBSP = new TspBase(stationNum,100,capacity);
    for (int i=0;i<stationNum;i++)
    {
        point x;
        x.a = info[2]->ToObject()->Get(i)->ToObject()->Get(Nan::New("x").ToLocalChecked())->NumberValue();
        x.b = info[2]->ToObject()->Get(i)->ToObject()->Get(Nan::New("y").ToLocalChecked())->NumberValue();
        int demand = info[2]->ToObject()->Get(i)->ToObject()->Get(Nan::New("d").ToLocalChecked())->NumberValue(); 
        MainBSP->_point.push_back(x);
        MainBSP->_allStationDemand.push_back(demand);
        std::cout<<"x:"<<x.a<<","<<x.b<<"d:"<<demand<<std::endl;
    }
    for (int i=0;i<stationNum+1;i++) {
        std::vector<double> v;
        for (int j=0;j<stationNum+1;j++) { 
            v.push_back(info[3]->ToObject()->Get(i)->ToObject()->Get(j)->NumberValue());
            std::cout<<" "<<v[j];
        }
        MainBSP->_cost.push_back(v);
        std::cout<<std::endl;
    }
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
