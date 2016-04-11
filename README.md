# Mandelbox-OpenACC
## Introduction
Mandelboxes are a relatively new discovery akin to a 3D fractal. This project utilizes a GPU and OpenACC to render frames making up a video of a traversal through a mandelbox defined by a parameters file passed as the first argument.

The final rendered video can be found at [https://youtu.be/bwmpGCiiVN4](https://youtu.be/bwmpGCiiVN4), and additional videos with different parameters can be found on our [YouTube channel](https://www.youtube.com/channel/UCsI3aqz-E4BCeXdFZj8n0CQ).

## System Requirements
- `pgCC`, or another C++ compiler supporting OpenACC 2.0
- `ffmpeg`, to convert images to a video
- GPU, the more powerful the GPU the faster the rendering

## Usage
1. Execute `make mandelbox` from the repo directory
There should be no errors, and 1 warning that "Compiler failed to translate accelerator region" in `DE(const vec3 &)`
2. Run `./mandelbox params_mandelbox.dat` to generate and save each frame as a .bmp file in the `images` directory
3. Run `./video.sh` to convert the .bmp files to .jpg's and create a video at 30FPS

## Parameters File Format
```
# CAMERA
# location x,y,z (7,7,7)
14.0 8.0 10.0
# look at x,y,z
000
# up vector x,y,z; (0, 1, 0)
010
# field of view (1)
1.1
# IMAGE
# width height
1080 1080
# detail level, the smaller the more detailed (-3) -4
# MANDELBOX
# scale, rMin, rFixed (2 0.5 1)
2 0.5 1
# max number of iterations, escape time
18 100
# COLORING
# type 0 or 1
1
# brightness
1.2
# IMAGE FILE NAME
image.bmp
```

## License
The original, serial code was developed by T. Gwosdz and N. Nedialkov of McMaster University, and is subject to the following license.
```
This file is part of the Mandelbox program developed for the course
CS/SE  Distributed Computer Systems taught by N. Nedialkov in the
Winter of 2015-2016 at McMaster University.

Copyright (C) 2015-2016 T. Gwosdz and N. Nedialkov

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
```

Parallelization and automatic naviagion were developed by Stuart Douglas and Matthew Pagnan.
