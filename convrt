#!/bin/bash
shopt -s nullglob nocaseglob # Ignore case and errors
for i in *.bmp # Do all images except JPEGs
do 
	echo converting $i
	newname=${i%.*}.jpg      # Strip file extension
	convert "$i" "$newname"  # Convert to JPEG
	rm -rf $i
done