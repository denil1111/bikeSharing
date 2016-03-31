 /*[{'x':10, 'y':40, 'd':2},{'x':45, 'y':20, 'd':1},{'x':36, 'y':10, 'd':-1},
                {'x':66, 'y':30, 'd':3},{'x':55, 'y':50, 'd':-4},{'x':80, 'y':20, 'd':-1}];*/
 /* [0, 3, 2, 1, 5, 4, 0];*/
var colors = ["yellow", "gold", "CornflowerBlue", "Chartreuse", "cyan"];


var HEIGHT = 400;
var WIDTH = 500;
var D_SCALE = 1.5

function get_x(d) { return d.x; }
function get_y(d) { return d.y; }

xmin = d3.min(stations, get_x);
xmax = d3.max(stations, get_x);
ymin = d3.min(stations, get_y);
ymax = d3.max(stations, get_y);

var xScale = d3.scale.linear()
                     .domain([xmin, xmax])
                     .range([WIDTH*0.1, WIDTH*0.9]);
var yScale = d3.scale.linear()
                     .domain([ymin, ymax])
                     .range([HEIGHT*0.1, HEIGHT*0.9]);


function randomInt(m, n)
{
  return Math.round(m + (n-m)*Math.random());
}


var svg = d3.select("body").append("svg")
                           .attr("width", WIDTH)
                           .attr("height", HEIGHT);
var circles = svg.selectAll("circle")
                 .data(stations)
                 .enter()
                 .append("circle");
circles.attr("cx", function(d) {
          return xScale(d.x);
        })
        .attr("cy", function(d) {
          return yScale(d.y);
        })
        .attr("r", function(d){
          return Math.abs(d.d)*D_SCALE;
        })
        .attr("fill", function(){
          return colors[randomInt(0, 4)];
        });

var d3line = d3.svg.line()
               .x(function(d) { return xScale(stations[d].x); })
               .y(function(d) { return yScale(stations[d].y); })
               .interpolate("linear");

svg.append("path")
   .attr("d", d3line(tour))
   .style("stroke-width", 1)
   .style("stroke", "steelblue")
   .style("fill", "none");
