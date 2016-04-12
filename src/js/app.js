"use strict";
var koa = require('koa');
var app = koa();
var route = require('koa-route');
var serve = require('koa-static');
var logger = require('koa-logger');
var parser = require('koa-body');
var render = require("./lib/render");
var addon = require('../../build/Release/bike');
// logger

app.use(logger());

// response

app.use(route.get('/randomrun', function*() {
	var datas = addon.randomData(10);
	var path = addon.runMerge();
	console.log(datas);
	console.log(path);
	var point = [];
	this.body = yield render("index",{point:point,path:path});
}));

app.use(route.post('/run', parser()));

app.use(route.post('/run', function*() {
    // console.log(this);
    // data = yield parser(this);
    // console.log("xx");
    console.log(this.request.body);
    var stationList = this.request.body.stationList;
    var distance = this.request.body.distance;
    var depot = this.request.body.depot; 
    addon.input(stationList.length, stationList, distance);
    var path = addon.runMerge();
    console.log(path);
    this.type = 'json';
    this.body = path;
    
}));

app.use(route.get('/', function*() {
	this.body = yield render("input");
}));

app.listen(3000);
app.use(serve(__dirname + '/public'));
console.log('listening on port 3000');
