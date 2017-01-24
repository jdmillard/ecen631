Build and Run
---
This assignment demonstrates live video feed, implementation of feature functions, and camera calibration. Use `n` and `p` to cycle through functions which are displayed in the second gui window. `esc` will end the video feed portion and move to task 3. Also, if the webcam has errors or is not detected, the video feed portion is automatically skipped and transitions to task 3.


From the build directory, these commands should build and run a given assignment:
```
$ cmake .. && make    # build
$ ./assignment        # run
```

Notes
---
 * The webcam I used worked in the lab, but not at home. I was getting `VIDEOIO ERROR: V4L2: Pixel format of incoming image is unsupported by OpenCV` How I made it work:
    * Install video for linux: `$ sudo apt-get install v4l2ucp`
    * Create environment variable: `export LD_PRELOAD=/usr/lib/x86_64-linux-gnu/libv4l/v4l2convert.so` (always run for new terminals or add to .bashrc)
        * This [thread](https://www.linuxquestions.org/questions/programming-9/opencv-pixel-format-of-incoming-image-is-unsupported-by-opencv-842801/) was helpful, but my *.so path was different
        * To find your proper *.so path, use `locate v4l | grep -v 0 | grep convert`
        * Running `$ v4l2ucp` will open a configuration gui, but I didn't need it.
 * Some additional [good coding ideas](http://answers.opencv.org/question/42508/videoio-error-pixel-format-unsupported/)
