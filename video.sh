#!/bin/bash

# convert bmp files to jpg
shopt -s nullglob nocaseglob # Ignore case and errors
for i in images/*.bmp # Do all images except JPEGs
do 
	echo converting $i
	newname=${i%.*}.jpg      # Strip file extension
	convert "$i" "$newname"  # Convert to JPEG
	rm -rf $i
done

# create movie
ffmpeg -r 30 -i images/image_%d.jpg mandelbox_video.mp4
