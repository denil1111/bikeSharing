var koa = require('koa');
var app = koa();
var route = require('koa-route');
var render = require("./lib/render")

// logger

app.use(function*(next) {
	var start = new Date;
	yield next;
	var ms = new Date - start;
	console.log('%s %s - %s', this.method, this.url, ms);
});

// response

app.use(route.get('/randomrun', function*() {
	var addon = require('../../build/Release/bike');
	var datas = addon.randomData(10); 
	var path = addon.runMerge(10); 
	console.log(datas);
	console.log(path);
	var point = [];
	var demand = [];
	// datas.forEach(function(data){
 //  		point.push({x:data.x,y:data.y});
 //  		demand.push(data.demand);
	// }); 
	for (data of yield datas) {
		point.push({x:data.x,y:data.y});
  		demand.push(data.demand);
	}
	this.body = yield render("index",{demand:demand,point:point,path:path});
}));

app.listen(3000);
console.log('listening on port 3000');