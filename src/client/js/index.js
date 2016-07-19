// gyroの調整
var gyroRevision = 0.00025;
var gyroThreshold = 0.005;

var accelRevision = 0.000025;
var accelThreshold = 0.0005;

// 温度によるColorコードの調整
var tempHigh = 35;
var tempLow = 11;

// 赤→青 FF0000→FFFF00→00FF00→00FFFF→0000FF
var tempMapping = {
    35 : 0xFF0000,
    34 : 0xFF3300,
    33 : 0xFF6600,
    32 : 0xFF9900,
    31 : 0xFFCC00,
    30 : 0xFFFF00,
    29 : 0xCCFF00,
    28 : 0x99FF00,
    27 : 0x66FF00,
    26 : 0x33FF00,
    25 : 0x00FF00,
    24 : 0x00FF33,
    23 : 0x00FF99,
    22 : 0x00FFCC,
    21 : 0x00FFFF,
    20 : 0x00CCFF,
    19 : 0x0099FF,
    18 : 0x0066FF,
    17 : 0x0033FF,
    16 : 0x0000FF,
    15 : 0x3300FF,
    14 : 0x6600FF,
    13 : 0x9900FF,
    12 : 0xCC00FF,
    11 : 0xFF00FF,
};

$(document).ready(function() {

    var scene = new THREE.Scene();
    var width = $('#container').width();
    var height = $('#container').height();
    console.log(width + ' : ' + height)
    var fov = 60;
    var aspect = width / height;
    var near = 1;
    var far = 1000;
    var camera = new THREE.PerspectiveCamera(fov, aspect, near, far);
    camera.position.set(0, 0, 50);

    var renderer = new THREE.WebGLRenderer();
    renderer.setSize(width, height);
    $('#container').append(renderer.domElement);

    var directionalLight = new THREE.DirectionalLight(0xffffff);
    directionalLight.position.set(0, 0.7, 0.7);
    scene.add(directionalLight);

    // 背景色
    renderer.setClearColor(new THREE.Color(0xffffff))

    var geometry = new THREE.BoxGeometry(15, 15, 15);
    var material = new THREE.MeshPhongMaterial({
        color : 0x000000
    });
    var mesh = new THREE.Mesh(geometry, material);
    scene.add(mesh)

    var socket = io.connect('localhost:8888');

    // サーバーからのデータを受信
    socket.on("data", function(data) {

        var temp = data.temp;
        temp = tempLow > temp ? tempLow : temp;
        temp = tempHigh < temp ? tempHigh : temp;

        material.color.setHex(tempMapping[temp]);

        var gyroX = data.gyro.x * gyroRevision;
        gyroX = Math.abs(gyroX) < gyroThreshold ? 0 : gyroX;
        var gyroY = data.gyro.y * gyroRevision;
        gyroY = Math.abs(gyroY) < gyroThreshold ? 0 : gyroY;
        var gyroZ = data.gyro.z * gyroRevision;
        gyroZ = Math.abs(gyroZ) < gyroThreshold ? 0 : gyroZ;

        mesh.rotation.x += gyroX;
        mesh.rotation.y += gyroY;
        mesh.rotation.z += gyroZ;

        // カメラ

        // TODO 微調整
        // if (Math.abs(gyroX) > 0 || Math.abs(gyroY) > 0 || Math.abs(gyroZ) >
        // 0) {
        var accelX = data.accel.x * accelRevision;
        accelX = Math.abs(accelX) < accelThreshold ? 0 : accelX;
        var accelY = data.accel.y * accelRevision;
        accelY = Math.abs(accelY) < accelThreshold ? 0 : accelY;
        var accelZ = data.accel.z * accelRevision;
        accelZ = Math.abs(accelZ) < accelThreshold ? 0 : accelZ;

        camera.position.x += accelX;
        camera.position.y += accelZ;
        camera.position.z += accelY;
        // }

        renderer.render(scene, camera);
    });
});
