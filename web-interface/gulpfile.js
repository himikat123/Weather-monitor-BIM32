const gulp = require('gulp');
const inline = require('gulp-inline');
const uglify = require('gulp-uglify');
const minifyCss = require('gulp-clean-css');
const autoprefixer = require('gulp-autoprefixer');
const gzip = require('gulp-gzip');
const gulpCopy = require('gulp-copy');
var rename = require("gulp-rename");

gulp.task('inline', function() {
    return gulp.src('build/index.html')
        .pipe(inline({
            base: 'build/',
            js: function() {
                return uglify({
                    mangle: false
                });
            },
            css: [minifyCss({level: {1: {specialComments: 0}}}), autoprefixer],
        }))
        .pipe(gulp.dest('build/')
    );
});

gulp.task('compress', async function() {
    gulp.src('build/index.html')
    .pipe(gzip())
    .pipe(gulp.dest('../BIM32/data'));
});

gulp.task('configCopy', async function() {
    return gulp.src('public/config.json')
        .pipe(gulp.dest('build')
    );
});

gulp.task('defaultConfigCopy', async function() {
    return gulp.src('src/pages/system/defaultConfig.json')
        .pipe(rename('config.json'))
        .pipe(gulp.dest('../BIM32/data')
    );
});

gulp.task('default', gulp.series(
    'inline', 'compress', 'configCopy', 'defaultConfigCopy'
));