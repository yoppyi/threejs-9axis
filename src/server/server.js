var http = require('http');
var url = require('url');
var path = require('path');
var fs = require('fs');
var serialport = require('serialport');

var socketServer = require('http').createServer();
var io = require('socket.io').listen(socketServer);

var port = 3000;
var websocketPort = 8888;
var clientFilePath = '/src/client';
var serialPortName = 'COM3';

// httpサーバー画面
http.createServer(function(request, response) {

    var uri = url.parse(request.url).pathname;
    var filename = path.join(process.cwd() + clientFilePath, uri);
    var extname = path.extname(filename);
    var header = {
        'Access-Control-Allow-Origin' : '*',
        'Pragma' : 'no-cache',
        'Cache-Control' : 'no-cache'
    }

    fs.exists(filename, function(exists) {

        if (!exists) {
            if (uri.lastIndexOf('/api', 0) === 0) {
                response.writeHead(200, header);
                response.write('{"x": ' + x + ', "y": ' + y + '}');
                response.end();
            }
            response.writeHead(404, header);
            response.end('Not Found');
            return;
        }

        if (fs.statSync(filename).isDirectory()) {
            filename += '/index.html';
        }

        fs.readFile(filename, 'binary', function(err, file) {
            response.writeHead(200, header);
            response.write(file, 'binary');
            response.end();
        });

    });

}).listen(port);

console.log('Server running at http://localhost:' + port);

// arduinoからのシリアル通信待ち受け
var sp = new serialport.SerialPort(serialPortName, {
    baudRate : 9600,
    dataBits : 8,
    parity : 'none',
    stopBits : 1,
    flowControl : false,
    parser : serialport.parsers.readline("\n")
});

// websocket
socketServer.listen(websocketPort);
io.sockets.on('connection', function(socket) {

    // シリアルポートからデータを取得、clientに送信
    sp.on('data', function(input) {
        var buffer = new Buffer(input, 'utf8');
        try {
            var json = JSON.parse(buffer);
            socket.emit('data', json);
            console.log(json);
        } catch (e) {
            return;
        }
    });

});
