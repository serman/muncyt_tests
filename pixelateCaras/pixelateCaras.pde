// Learning Processing
// Daniel Shiffman
// http://www.learningprocessing.com

// Example 16-7: Video pixelation

import gab.opencv.*;
import processing.video.*;
import java.awt.Rectangle;

OpenCV opencv;
Rectangle[] faces;

// Size of each cell in the grid, ratio of window size to video size
int videoScale = 8;
// Number of columns and rows in our system
int cols, rows;
// Variable to hold onto Capture object
Capture video;

void setup() {
  size(640,480);
  
  // Initialize columns and rows
  cols = width/videoScale;
  rows = height/videoScale;
  video = new Capture(this,width,height);
  video.start();
  opencv = new OpenCV(this, 640, 480);
    opencv.loadCascade(OpenCV.CASCADE_FRONTALFACE);  
  
}

void draw() {
  background(0);
  // Read image from the camera
  if (video.available()) {
    video.read();
  }
  
  opencv.loadImage(video); 
  faces = opencv.detect();
  video.loadPixels();
  if(faces.length>0){
    rect(faces[0].x, faces[0].y, faces[0].width, faces[0].height);
 
  // Begin loop for columns
  for (int i = 0; i < cols; i++) {
    // Begin loop for rows
    for (int j = 0; j < rows; j++) {
      
      // Where are we, pixel-wise?
      int x = i*videoScale;
      int y = j*videoScale;
      if(x> faces[0].x && x<faces[0].x+faces[0].width&& y>faces[0].y && y<(faces[0].y+faces[0].height)){ 
      // Looking up the appropriate color in the pixel array
        color c = video.pixels[x + y*video.width];
        fill(c);
        //stroke(0);
        noStroke();
        rect(x,y,videoScale,videoScale);
      }
    }
  }
  filter(POSTERIZE, 15);
  } 
  
}
