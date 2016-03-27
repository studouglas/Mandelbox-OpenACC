#!/bin/bash
rm -rf video.mpg
cat *.jpg | ffmpeg -f image2pipe -c:v mjpeg -i - video.mpg
