// 百度地图API功能
var map = new BMap.Map("allmap", {
  enableMapClick: false
}); // 创建Map实例
// map.centerAndZoom(new BMap.Point(116.404, 39.915), 11);  // 初始化地图,设置中心点坐标和地图级别
// map.addControl(new BMap.MapTypeControl()); //添加地图类型控件
map.centerAndZoom("杭州", 15); // 设置地图显示的城市 此项是必须设置的
var top_left_control = new BMap.ScaleControl({
  anchor: BMAP_ANCHOR_TOP_LEFT
}); // 左上角，添加比例尺
var top_left_navigation = new BMap.NavigationControl(); //左上角，添加默认缩放平移控件
var top_right_navigation = new BMap.NavigationControl({
  anchor: BMAP_ANCHOR_TOP_RIGHT,
  type: BMAP_NAVIGATION_CONTROL_SMALL
}); //右上角，仅包含平移和缩放按钮
/*缩放控件type有四种类型:
BMAP_NAVIGATION_CONTROL_SMALL：仅包含平移和缩放按钮；BMAP_NAVIGATION_CONTROL_PAN:仅包含平移按钮；BMAP_NAVIGATION_CONTROL_ZOOM：仅包含缩放按钮*/

//添加控件和比例尺
function add_control() {
  map.addControl(top_left_control);
  map.addControl(top_left_navigation);
  map.addControl(top_right_navigation);
}
//移除控件和比例尺
function delete_control() {
  map.removeControl(top_left_control);
  map.removeControl(top_left_navigation);
  map.removeControl(top_right_navigation);
}
add_control();
var createTag = false;
var deleteTag = false;
var depotTag = false;
var stationMList = [];
var stationWithDepot = [];
var bikeIcon = new BMap.Icon("/image/bike.png", new BMap.Size(32, 32));
var depotIcon = new BMap.Icon("/image/depot.png", new BMap.Size(64, 51));
var vehicleIcon = new BMap.Icon("/image/vehicle.png", new BMap.Size(40, 27));
var depotMarker = new BMap.Marker(new BMap.Point(depot.x, depot.y), {
  icon: depotIcon
});
function addStation(x, y, demand) {
  stationList.push({
    d: demand,
    x: x,
    y: y
  });
  var index = stationList.length - 1;
  var stationMarker = new BMap.Marker(new BMap.Point(x, y), {
    icon: bikeIcon
  });
  stationMarker.setLabel(new BMap.Label(demand, {
    offset: new BMap.Size(20, -10)
  }));
  stationMList.push(stationMarker);
  stationMarker.addEventListener("click", function() {
    if (deleteTag == true) {
      map.removeOverlay(this);
    }
    $("#demandChange").dialog({
      autoOpen: false,
      open: function() {
        $("#demandChangeData").attr("placeholder", stationList[index].d);
        $(this).off('submit').on('submit', function() {
          var demand = $("#demandChangeData").val();
          $("#demandChangeData").val("");
          $(this).dialog('close');
          stationList[index].d = demand;
          stationMarker.getLabel().setContent(stationList[index].d);
          return false;
        });
      },
      buttons: {
        "Ok": function() {
          $(this).find('form').submit();
          $(this).dialog("close");
        },
        "Cancel": function() {
          $(this).dialog("close");
        }
      }
    });
    $("#demandChange").dialog("open");
  });
  map.addOverlay(stationMarker);
}
stationList.forEach(function(station) {
  addStation(station.x, station.y, station.d);
})
map.addOverlay(depotMarker);
var distance = [
  []
];
$("#demandInput").dialog({
  autoOpen: false,
});
$("#demandChange").dialog({
  autoOpen: false,
});
map.addEventListener("click", function(e) {
  $("#demandInput").dialog({
    autoOpen: false,
    open: function() {
      $(this).off('submit').on('submit', function() {
        var demand = $("#demandInputData").val();
        $("#demandInputData").val("");
        $(this).dialog('close');
        addStation(e.point.lng, e.point.lat, demand);
        return false;
      });
    },
    buttons: {
      "Ok": function() {
        $(this).find('form').submit();
        $(this).dialog("close");
      },
      "Cancel": function() {
        $(this).dialog("close");
      }
    }
  });
  if (createTag == true) {
    // var demand = prompt("demand");
    $("#demandInput").dialog("open");
    // alert(x + "," + y);
  } else if (depotTag == true) {
    depot.x = e.point.lng;
    depot.y = e.point.lat;
    depotMarker.setPosition(new BMap.Point(e.point.lng, e.point.lat));
  }
});
function reset() {
  // map.removeOverlay(carMk);
  map.clearOverlays();
  stationMList.forEach(function(stationM, i) {
    stationM.setLabel(new BMap.Label(stationList[i].d, {
      offset: new BMap.Size(20, -10)
    }));
    map.addOverlay(stationM);
  });
  map.addOverlay(depotMarker);
}
function CancelTag() {
  map.setDefaultCursor("");
  createTag = false;
  deleteTag = false;
  depotTag = false;
}

function createStation() {
  CancelTag();
  map.setDefaultCursor("crosshair");
  createTag = true;
}

function deleteStation() {
  CancelTag();
  map.setDefaultCursor("crosshair");
  deleteTag = true;
}

function setDepot() {
  CancelTag();
  map.setDefaultCursor("crosshair");
  depotTag = true;
}

document.onkeydown = function(event) {
  var e = event || window.event || arguments.callee.caller.arguments[0];
  if (e && e.keyCode == 27) { // 按 Esc 
    CancelTag();
  }
};

function postData() {
  console.log(JSON.stringify({
    stationList: stationList,
    distance: distance
  }))
  $.ajax({
    url: "/run",
    type: 'POST',
    data: JSON.stringify({
      stationList: stationList,
      distance: distance,
      depot: depot
    }),
    dataType: 'json',
    contentType: 'application/json',
    success: function(data, status, xhr) {
      console.log(data);
      //         Do Anything After get Return data  
      //          $.each(data.payload, function(index){  
      //              $("#result").append("</br>" + data.payload[index].beanStr);  
      //          });  
      path(data);
    },
    Error: function(xhr, error, exception) {
      // handle the error.    
      alert(exception.toString());
    }
  });
}

function run() {
  distance = new Array(stationList.length + 1);
  var i = 0;
  for (i = 0; i < stationList.length + 1; i++) {
    distance[i] = new Array(stationList.length + 1);
  }
  var k = 0;
  stationWithDepot = [];
  for (i = 0; i < stationList.length; i++) {
    stationWithDepot.push(stationList[i]);
  }
  stationWithDepot.push(depot);
  stationWithDepot.forEach(function(station_i, i) {
    stationWithDepot.forEach(function(station_j, j) {
      if (i != j) {
        var driving = new BMap.DrivingRoute(map, {
          onSearchComplete: function(results) {
            if (driving.getStatus() != BMAP_STATUS_SUCCESS) {
              return;
            }
            var plan = results.getPlan(0);
            distance[i][j] = plan.getDistance(false);
            k++;
            if (k == stationWithDepot.length * (stationWithDepot.length - 1)) {
              postData();
            }
          }
        });
        var p1 = new BMap.Point(station_i.x, station_i.y);
        var p2 = new BMap.Point(station_j.x, station_j.y);
        driving.search(p1, p2);
      } else {
        distance[i][j] = 0;
      }
    });
  });
  console.log(distance);
  // $.post("/run",
  // {
  //   x:"xx"
  // }
  // );
}
var carMk;
function path(routePath) {
  var p1 = new BMap.Point(depot.x, depot.y);
  var options = {
    renderOptions: {
      panel: "r-result"
    },
    onSearchComplete: function(results) {
      if (driving.getStatus() == BMAP_STATUS_SUCCESS) {
        var plan = results.getPlan(0);
        var s = [];
        var pts = [];
        var keyPts = [];
        console.log(plan.getNumRoutes());
        for (var j = 0; j < plan.getNumRoutes(); j++) {
          var route = plan.getRoute(j);
          var ppts = route.getPath();
          var polyline = new BMap.Polyline(ppts);
          map.addOverlay(polyline);
          pts = pts.concat(ppts);
          keyPts.push(pts.length)
        }
        var paths = pts.length;
        var bikeOnV = 0;
        carMk = new BMap.Marker(pts[0], {
          icon: vehicleIcon
        });
        carMk.setLabel(new BMap.Label(bikeOnV, {
          offset: new BMap.Size(30, 0)
        }));
        map.addOverlay(carMk);
        i = 0;
        j = 0;
        console.log(keyPts);
        var currentDemand = [];
        for (var index = 0; index < stationList.length; index++) {
          currentDemand[index] = stationList[index].d;
        }

        function resetMkPoint(i) {
          carMk.setPosition(pts[i]);
          if (i < paths) {
            if (keyPts[j] == i) {
              console.log(routePath);
              console.log(j);
              console.log(routePath[j]);
              (function(j) {
                setTimeout(function() {
                  stationLB = stationMList[routePath[j].id].getLabel();
                  currentDemand[routePath[j].id] -= routePath[j].d;
                  stationLB.setContent(currentDemand[routePath[j].id]);
                  bikeOnV += routePath[j].d;
                  carMk.getLabel().setContent(bikeOnV);
                }, 1000);
              } (j));
              j++;
              i++;
              setTimeout(function() {
                resetMkPoint(i);
              }, 2000);
            } else {
              i++;
              setTimeout(function() {
                resetMkPoint(i);
              }, 100);
            }
          } else {
            i = 0;
            carMk.getLabel().setContent("finish!");
            // resetMkPoint(i);
          }
        }
        setTimeout(function() {
          resetMkPoint(0);
          console.log(i);
        }, 100);
      }
    }
  };
  var driving = new BMap.DrivingRoute(map, options);
  var waypoints = [];
  routePath.forEach(function(routeX, index) {
    waypoints.push(new BMap.Point(stationList[routeX.id].x, stationList[routeX.id].y));
  });
  driving.search(p1, p1, {
    waypoints: waypoints
  }); //waypoints表示途经点
}
// map.enableScrollWheelZoom(true); //开启鼠标滚轮缩放