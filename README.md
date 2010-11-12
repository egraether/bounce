## Bounce ##

Bounce is an augmented reality installation, allowing the user to interact with the system by throwing balls at a projection on the wall.

It was developed as a student project using [OpenFrameworks](http://www.openframeworks.cc/download) and the included OpenCV-Addon.

[video](http://www.youtube.com/watch?v=-ie6amFUwQU)


### What you need: ###

* experience with [OpenFrameworks](http://www.openframeworks.cc/download)
* camera
* projector
* microphone
* balls


## Instructions ##


### application setup ###

In order to compile Bounce you have to: 

* download [Openframeworks](http://www.openframeworks.cc/download) with addons(FAT), compatible to your IDE
* in the OF directory, go to apps/addonsExamples/ and make a copy of allAddonsExample, rename it `bounce`
* add the source-files from this repository to `bounce/src/`
* get the [media](http://egraether.multimediatechnology.at/bounce/bounce_media.zip) and include it in `bounce/bin/data/`

Notice:
The current version is not able to adapt to size change after being compiled.
All buttons and graphics are measured to `1280 x 800`, and need to be modified in position and size otherwise.
To change the window's size you need to change the `windowWidth` and `windowHeight` at `main.cpp` (line 13 and 14).


### hardware setup ###

* use a projector to throw a screen-projection on a wall. The picture needs to be rectangular, no trapezoidal effect.
* than darker the room and than lighter the projection than better.
* it's not necessary that the camera is directly beside the projector. 
* make sure that the camera gets the full screen, but few from the surrounding.
* fasten the microphone as near as possible at the projection, directly on the wall, a contact microphone is best.
* test if you get a sound when throwing a ball against the wall.
* use black beanbags or tennisballs in black duct tape.


### calibration ###

After the launch of Bounce it is nessesary to calibrate before the balls get recognised.

* Click on the calibrate button in the menu.
* Now you see the picture of the camera, if not, no camera is available.
* Direct the camera to the screen or projection, the full application window has to be inside the picture.
* click again and the calibration will start, make sure that nothing moves in the image of the camera.
* at first the system tries to see 4 circles, one in each corner.
* then every single cornerposition gets measured from the application.
* after that you can see the audio-plot of the microphone, make some noise and you see an effect.
* now the application is able to recognise balls thrown at the projection, if not read **error-handling**.

It is also recommended to calibrate while playing.


### interface ###

**menu**:

* After the start you see the menu
* Mouseclicking has the same effect as a bounce of a ball.
* You can click or bounce on the buttons to start games or calibration.

**games**:

* The games start immediately or after the first bounce.
* after the game is finished you see a scores table
* on the left side are all-time highscores, on the right side the scores of the last ten players.
* if you get inserted in the highscores, you have to type in your name.
* the highscores get saved in `game.txt` in the bounce directory


### keys ###

* SPACE - pauses/proceeds the game
* M - back to menu
* F - fullscreen
* S - open camera settings window
* C - shows/hides the Console (see Console)
* 1 to 5 - shows processed images
* 0 - reset Calibration.


### console ###

used to change specific values (left bottom)
press 'c' to show/hide
use LEFT and RIGHT arrow to navigate between values and UP and DOWN to change the value

**values**:

* `screenStoreSize` - regulates how much pictures from screen get stored, neccessary because cameraImage and screenImage have temporal difference
* `camWaitFrames` - sets how many frames the system waits from sound to picture, because the microphone is faster than the camera
* `threshold` - sets how high the brightness difference of a pixel has to be at least between camera- and screenImage to be recognised
* `bangLevel` - how much energy-change in the audiosignal a bounce has to make to be recognised, symbolised by the red line at the end of calibration mode

---

### error-handling ###

**bang of ball not loud enough**:

* do the calibration as described, till the audioplots appear.
* throw a ball and see if the audioplot goes above the red line.
* if not change the `bangLevel` at the console, till the ballsound is above the red line.

**ball not detected**:

* after calibration, throw a ball against the wall
* press '5' to see a gray picture from the camera with the ball on it
* if there is no ball on the picture, you have problems with the microphone maybe
* press '3' to see a black picture whera all differences between screen and camera are white
* the ball should be a white blob in this picture
* if there is more white than the ball, increase the threshold in the console
* if there is no white, decrease the treshold in the console, until you see the ball after throwing.
* if the white is not controllable, adapt the lighting of your room.

**projector or camera was moved**:

* do the calibration once again
