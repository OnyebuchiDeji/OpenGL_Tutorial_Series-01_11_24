# OpenGL_Tutorial_Series-01_11_24
OpenGL Quest: Learning Computer Graphics with OpenGL

##	Date: 1-11-2024


##	How to Setup (for Windows Specifically)

+	Dowmload dependencies:
	1.	GLEW:
		-	Download Windows Binaries from: https://glew.sourceforge.net/index.html
		-	Extract Zip and move into Dependencies folder and name "GLEW"
	2.	GLFW:
		-	Download GLFW **32-bit** (for more compatibility) Windows Binaries from: 
			https://www.glfw.org/download
		-	Extact Zip and move into Dependencies folder and name "GLFW"
+	Project Properties Setup:
	1.	Open The Solution in Visual Studio (2022 and higher)
	2.	Click Configuration Properties-> General -> In Output Directory, add "$(SolutionDir)bin\$(Configuration)-$(Platform)\" 
		-	and in Intermediate Output Directory, add "$(SolutionDir)bin-int\$(Configuration)-$(Platform)\"	 
	3.	Right-click on Project in VS solution tree -> Scoll Down -> Open Properties
		-	Change Platform -> Win32
		-	Click C/C++ -> In Additional Includes, Copy and Paste "src;src/vendor;$(SolutionDir)Dependencies\GLFW\include;$(SolutionDir)Dependencies\GLEW\include" **as is** --- the `src;src/vendor` is for other external code like imGUI and stbimages
			+	Under C/C++, add "WIN32;GLEW_STATIC;" in Preprocessor Definitions
		-	Click Linker -> under General, in Additional Library Directories, add "$(SolutionDir)Dependencies\GLFW\lib-vc2022;$(SolutionDir)Dependencies\GLEW\lib\Release\Win32" as is --- notice that the only the Win32 library directories are used
		-	Under Linker, in Additional Dependencies, add "glfw3.lib;glew32s.lib;opengl32.lib;User32.lib;Gdi32.lib;Shell32.lib;msvcrtd.lib;msvcrt.lib;libcmt.lib;libcmtd.lib" as is.
		This removes any other Windows libraries not essential, helping to keep the code lightweight.
	3.	Click Apply and OK --- or just click OK 


##	About Project

###	Overview
	This is not a game engine. It's an Intro to OpenGL and to show you what OpenGl code one needs
	to write to make things happen

###	Brief

Explore very much about the basics of the graphics pipeline and rendering with OpenGL all the way to sending data to the GPU and frendering different shapes both statically and dynamically. Also integrates ImGUI and create a test pipeline for dynamically loading different shader scripts and their data for different
results using the ImGUI library.

###	Reference:
	The Cherno Opengl Series, 2017, Youtube, Available at: https://youtu.be/W3gAzLwfIP0?si=8T2iVkNg7LXJ8wY3


#### Screenshots Sequences

![Image 0](./_screenshots/ots0.png)
![Image 1](./_screenshots/ots1.png)
![Image 2](./_screenshots/ots2.png)
![Image 3](./_screenshots/ots3.png)
![Image 4](./_screenshots/ots4.png)
![Image 5](./_screenshots/ots5.png)
![Image 6](./_screenshots/ots6.png)
![Image 7](./_screenshots/ots7.png)
![Image 8](./_screenshots/ots8.png)
![Image 9](./_screenshots/ots9.png)
![Image 10](./_screenshots/ots10.png)
![Image 11](./_screenshots/ots11.png)
![Image 12](./_screenshots/ots12.png)
![Image 13](./_screenshots/ots13.png)
![Image 14](./_screenshots/ots14.png)
![Image 15](./_screenshots/ots15.png)
![Image 16](./_screenshots/ots16.png)
![Image 17](./_screenshots/ots17.png)
![Image 18](./_screenshots/ots18.png)
![Image 19](./_screenshots/ots19.png)
![Image 20](./_screenshots/ots20.png)