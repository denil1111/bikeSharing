var koa = require('koa');
var app = koa();
var route = require('koa-route');
var serve = require('koa-static');
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
	this.body = yield render("index",{point:point,path:path});
}));

app.listen(3000);
app.use(serve(__dirname + '/public'));
console.log('listening on port 3000');
