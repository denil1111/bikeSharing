"use strict";
var koa = require('koa');
var app = koa();
var route = require('koa-route');
var serve = require('koa-static');
var logger = require('koa-logger');
var parser = require('koa-body');
var render = require("./lib/render");
var addon = require('../../build/Release/bike');

// Set up monk
var monk = require('monk');
var wrap = require('co-monk');
var db = monk('localhost/bikeSharing');

// Wrap monk in generator goodness
var stationDb = wrap(db.get('stations'));
// logger

app.use(logger());

// response

app.use(route.get('/randomrun', function* () {
    var datas = addon.randomData(10);
    var path = addon.runMerge();
    console.log(datas);
    console.log(path);
    var point = [];
    this.body = yield render("index", { point: point, path: path });
}));

app.use(route.post('/run', parser()));

app.use(route.post('/run', function* () {
    // console.log(this);
    // data = yield parser(this);
    // console.log("xx");
    console.log(this.request.body);
    var stationList = this.request.body.stationList;
    var distance = this.request.body.distance;
    var depot = this.request.body.depot;
    yield stationDb.insert({ stationList: stationList, depot: depot });
    addon.input(stationList.length, 20, stationList, distance);
    var path = addon.runAlgorithm(1);
    console.log(path);
    this.type = 'json';
    this.body = path.result;

}));

app.use(route.get('/', function* () {
    var data = yield stationDb.findOne(
        {},
        { sort: { _id: -1 } }
    );
    console.log(data);
    if (!data) {
        var stationList = [];
        var depot = { x: 0, y: 0 };
    } else {
        var stationList = data.stationList;
        var depot = data.depot;
    }
    console.log(stationList);
    this.body = yield render("input", { stationList: stationList, depot: depot });
}));

app.use(route.get('/history/:id', function* (id) {
    var datas = yield stationDb.find(
        {},
        { sort: { _id: -1 } }
    );
    var data = datas[id];
    console.log(data);
    if (!data) {
        var stationList = [];
        var depot = { x: 0, y: 0 };
    } else {
        var stationList = data.stationList;
        var depot = data.depot;
    }
    console.log(stationList);
    this.body = yield render("input", { stationList: stationList, depot: depot });
}));

app.listen(3000);
app.use(serve(__dirname + '/public'));
console.log('listening on port 3000');
