Synopsis

This application detects the object through camera and shows the size(length) of an object. 
The object measurement using stereo camera is better than object detection using a single camera that was 
proposed in many previous research works. The proposed measurement system consists of object detection on 
the stereo images and blob extraction and distance and size calculation and object identification.

Code Example

This Project is used to find the height and Width of the object in the image.

Motivation

With the advancement of technology, the stereo vision technology has become more and more mature.
Many related applications have hence emerged, such as 3D movie, face recognition and other.

Installation

All the code is attached written in Visual Studio on C++.
This Project need Libraries OPENCV and OPENCV_CONTRIB.
This Project need MATLAB for Image processing, a phase in the Project.
Add all the libraries of OPENCV to visual studio project for sucessfull execution.

Tests

1. Run the Code in the Matlab Directory to get the Stereo Images Processed.
2. The Output from the Matlab is used as Input to the Visual C++ Project, where we find the height and width of the object.
3. We need to point out the corners pixel points of the both Left and Right images of stereo to get Depth.
4. Then after we get the size of the Object in the image on console.

Contributors

If any contributions or suggestions, please do contact skandagatla1@student.gsu.edu

License

Public Licensed.

