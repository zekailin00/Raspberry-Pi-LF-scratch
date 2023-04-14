#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char* argv[])
{
 //Open the default video camera
 VideoCapture cap(0);

 // if not success, exit program
 if (cap.isOpened() == false)  
 {
  cout << "Cannot open the video camera" << endl;
  cin.get(); //wait for any key press
  return -1;
 } 

 double dWidth = cap.get(CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
 double dHeight = cap.get(CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video

 cout << "Resolution of the video : " << dWidth << " x " << dHeight << endl;

 string window_name = "My Camera Feed";
 namedWindow(window_name); //create a window called "My Camera Feed"
 
 string window_name2 = "Processed";
 namedWindow(window_name2); //create a window called "Processed Feed"

 string window_name3 = "Direction";
 namedWindow(window_name3); //create a window called "Direction"

 while (true)
 {
  Mat frame;
  bool bSuccess = cap.read(frame); // read a new frame from video 

  //Breaking the while loop if the frames cannot be captured
  if (bSuccess == false) 
  {
   cout << "Video camera is disconnected" << endl;
   cin.get(); //Wait for any key press
   break;
  }

  //show the frame in the created window
  imshow(window_name, frame);

  Mat processed_img;
  resize(frame, processed_img, Size(256, 256), INTER_LINEAR);
  cvtColor(processed_img, processed_img, COLOR_BGR2GRAY);
  GaussianBlur(processed_img, processed_img, Size(7, 7), 0);
  imshow(window_name2, processed_img);

  Mat direction_img;
  threshold(processed_img, direction_img, 120, 255, THRESH_BINARY);
  rectangle(direction_img, Rect(Point2i(0,0), Point2i(256, 128)), Scalar(0,0,0), -1);
  Moments m = moments(direction_img);
  Point center(m.m10/m.m00, m.m01/m.m00);
  circle(direction_img, center, 5, Scalar(128,0,0), -1);
  line(direction_img, Point(128, 0), Point(128, 256), Scalar(128,0,0),1);
  line(direction_img, Point(center.x, 0), Point(center.x, 256), Scalar(255,0,255),1);

  float offset = (center.x - 128) / 128.0;
  string text = "offset: " + to_string(offset);
  putText(direction_img, text, Point(5, 20), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255,255,255));

  imshow(window_name3, direction_img);
  

  //wait for for 10 ms until any key is pressed.  
  //If the 'Esc' key is pressed, break the while loop.
  //If the any other key is pressed, continue the loop 
  //If any key is not pressed withing 10 ms, continue the loop 
  if (waitKey(10) == 27)
  {
   cout << "Esc key is pressed by user. Stoppig the video" << endl;
   break;
  }
 }

 return 0;

}