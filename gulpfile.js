var gulp = require('gulp');
var mainBowerFiles = require('main-bower-files');

gulp.task('main-bower-files', function() {
    gulp.src(mainBowerFiles()).pipe(gulp.dest('src/client/js/lib'));
    gulp.src([ 'bower_components/socket.io-client/socket.io.js' ])
            .pipe(gulp.dest('src/client/js/socket.io-clent/lib/'));

});

gulp.task('default', [ 'main-bower-files' ]);
