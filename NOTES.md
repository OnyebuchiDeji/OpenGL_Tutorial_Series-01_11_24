#	The Cherno OpenGL Series (2017)

###	Reference:
	The Cherno Opengl Series, 2017, Youtube, Available at: https://youtu.be/W3gAzLwfIP0?si=8T2iVkNg7LXJ8wY3

##	Date Started: Fri-1-Nov-2024

##	Overview
	This is not a game engine. It's an Intro to OpenGL and to show you what OpenGl code one needs
	to write to make things happen

##	Episode 1: Intro to OpenGL, "Welcom to OpenGL"

##	Episode 2: Render A Triangle with Legacy OpenGL code, "Setting up OpenGL and Creating a Window in C++"
	Downloaded GLFW binaries and included GLFW to project
	linked glf23.lib,opengl32.lib to project
	Also, the header file used for the opengl render were the ones that glfw3 provided which are the Windows ones.
	This was basically the DirectX3D.

##	Episode 3: "Using Modern OpenGL in C++""
	Here, the latest OpenGL functions are gotten so they can be called.
	So a library is needed that has the function declarations to access
	the corresponding function definitions in the driver dll files.
	This is essentially getting function pointers to those functions in the dll files.

	One can do this manually, there are a few problems:
		1.	it takes too much time as there are 1000s of functions.
		2.	it won't be cross-platform because to access the drivers and pull out functions from that
			will require the win32 api called 'cformwindow' to looad the libraries/function pointers.

	Hence another library is used to do this.
	This library provides the OpenGL API specification symbol declarations and constants in a header file
	while it's .cpp file, it's actuall implementation, does these:
		1.	identifies your graphics drivers.
		2.	Finds the appropriate DLL file.
		3.	Loads all the function pointers

	In this tutorial, the GLEW library is used, downloaded from `glew.sourceforge.net`
	The Windows binaries are downloaded and its header files and static library file are linked
	to this project.

	Now, glew gives to .lib files.
		The first: 'glew32.lib' is a static library but is used as a dynamic library
		The second: 'glew32s.lib' is the actual static library to include as a link dependency.
		Also note that 'glew32s.lib' is bigger than the other.


##	Episode 4: Drawing With Modern OpenGL; "Vertex Buffers and Drawing a Triangle in OpenGL"
	Vertex Buffer is a buffer of data that one can specify and send to the GPU
	Shaders tell the GPU what data to get and how to use it to draw something and color it.
	The CPU is what does the draw call, the GPU is what does the processing.

	OpenGL operates like a state machine:
		You set a series of states...
		E.g. You say, 'Select this buffer and this shader and draw me a triangle.'
		So based on which buffer and shader you select determines what is drawn.

	Steps:
		1.	Define buffer
		2.	Send to OpenGL's VRAM
		3.	Call OpenGL to render it.

##	Episode 5: "Vertex Attributes and Layouts in OpenGL"
	Vertex Attributes and Layouts are Covered Here.

	These tell the OpenGL shader what data is there and what is the layout of the memory.
	This is so that the shader can interprete the data in a specific way.

	On the shader side too, accessing the data must match the layout specified in the CPU side.
	For example,
		Here is my buffer, BU
		4 bytes into my buffer are vector coordinates
		8 bytes into my buffer are texture coordinates.

	Documentation for these:
		glVertexAttribPointer:
			https://docs.gl/gl4/glVertexAttribPointer
		
		glEnableVertexAttribArray:
			https://docs.gl/gl4/glEnableVertexAttribArray


##	Episode 6: "How Shader's Work in OpenGL"
	No code was written; just an explanation of how shaders work, which things I've written.

##	Episode 7: "Writing a Shader in OpenGL"


##	Episode 8: "How I Deal with Shaders in OpenGL", Thurs-7-Nov-2024
	This continues on how to deal with shaders.

	The Cherno does not always write his shaders in two different files.
	Instead, he writes them in the same file, separated by special syntax.

	He organizes his OpneGL code like he would using DirectX3D.

##	Episode 9: "Index Buffers in OpenGL"
	Consider drawing a Square...

	Everything the GPU does, it does in Triangles, because Triangles have the least number of vertices
	needed to represent one flat plane with a surface normal pointing to a single direction.

	Hence a Square can be made up of two triangles.

	This episode focuses on using index buffers to draw a square.

##	Episode 10: "Dealing with Errors in OpenGL"
	This explores how to handle Errors in OpenGL by setting up debugging tools that detect errors properly

	No external library is used.
	There are two main ways to check for errors in  OpenGL:
		2.	glDebugMessageCallback: Added in OpenGL 4.3, it allows one to specify a function pointer to OpenGL and OpenGL will call
			that function when an error occurs.
			This is far better than glGetError as it does not require one to constantly keep prompting OpenGL.
			It also gives far more information.
			But it's only for OpenGL 4.3.

			Link: https://docs.gl/gl4/glGetError

			It returns Gl_NO_ERROR when there's no error; it's value us 0
			It states that dues to distributed implementations (for its various versions), <b>there may be several error flags</b>.
			Now, if any single error flag has recorded an error, the value of that flag is returnes and that flag is reset
			to GL_NO_ERROR <b>when glGetError</b> is called.<br>
			if more than one flag has recorded an error, glGetError returns and clears an arbitrary (non-specific) flag value.<br>
			So to ensure that the error flag for the current error that occurs is sent immediately, the glGetError should always be
			called in a loop until it returns 'GL_NO_ERROR' so that all error flags are reset so that the next error raised will yield
			a flag that identifies a specific error.

		1.	glGetError Function: It's been with OpenGL from long.
			if an error occurs when an OpenGL function is called, a Flag is set internally in OpenGL's memory
			that specifies what error occured.
			glGetError gives back this Flag (an Integer) code that representst the error.
			The more glGetError is called, the more Error Flags it gives; so it needs to be called until there are no more
			error flags to be retrieved.
			The workflow is this:
			First glGetError is called until all prior pending errors are cleared
	        Then the Draw Call or any function is called, which can cause an error
	        Then glGetError is called to retrieve these errors.
			It's compatible with every version

			Although note that using glGetError only returns the error code.
			The meaning of the code then needs to be investigated.
			The enums in OpenGL are defined in Hexadecimal notation.
			
			So, by the example displayed in the ep10, the error was decumal 1280
			In Hex this is 500. It was converted using Visual Studio, by putting a Break points, etc...

			Then in the OpenGL header file, just search '0x500' and the meaning of it can be seen.
			The 500 means <em>"GL_INVALID_ENUM"</em>
			So now, a function that can translate the decimal as Hex to the definition of the error can be made.

			This makes sense since the cause of the error was passing in GL_INT instead of GL_UNSIGNED_INT

	This solution, though, has these inconveniences:
		*	It does not show which line or function call causes the error unless using a breakpoint to investigate, which can become more tedious
			the more there is an issue.
			Better Solution:
				Use `assert` which would trigger a break and pause execution on the line where the error occurcs.


		*	It could cause a clutter since these three lines of code must be called:
			        GLClearError();
					<code to investigate for errors>
					GLCheckError();	


##	Episode 11: "Uniforms in OpenGL"
	There a cool way to get data from the CPU to the Shader
	One can update that data live, not just at compile time but even dynamically, while the program is running.
	
	Some Differences Between Attrributes and Uniforms:
		Uniforms are set per draw; that is, one can set the uniforms up before calling glDrawArrays
		or glDrawElements.
		Whereas Attributes are set per vertex, for each vertices passed to the shader.

	A major thing to keep in mind is that Uniforms are Per Draw...
	Meaning that for the Square (the two triangles) rendered, the code that draws the two triangles...
		        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
	the uniform variable cannot be changed in between or within the Draw Call above.
	So it's not possible to draw one triangle as one color and the other triangle with its own.
	This can only be done if the two triangles are created as separate geometry with their own VBO and then between their draw calls
	the uniform's value is changed and registered using, for example:
				GLCall(glUniform4f(location, r, 0.05f, 0.58f, 1.0f));
			before the:
				GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
	OR
		It can be done via a Vertex Attribute without needing separate VBOs for them.
				
#	Episode 12: "Vertex Arrays in OpenGL"
	They are similar to Vertex Buffers.
	This is intrinsic to OpenGL. They do not exist in other Rendering Specification/APIs like DirectX
	They are a way to bind Vertex Bufffers with a certain kind of specification for the layout of that veryex buffer.

	Normally, see the layout of the Vertex Buffer being specified:
		Each vertex is 2 floats, the type, stride, etc are specified
		GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0));

	The Vertex Array object allows one to bind that Vertex layout specification using the glVertexAttribPointer
	to an actual Vertex Buffer or possibly a series of Vertex Buffers.<br>
	So instead of having to explicitely specify the Vertex Layout anytime one draws, using glVertexAttribPointer,
	a Vertex Array Object stores or specifies that specific kind of layout for those vertices that define the Vertex Buffer.<br>
	Hence all that needs to be done is that for that set of vertices all that's now needed is to simply *bind* the Vertex Buffer
	to the Vertex Array object.<br>
	Hence, one can have predefined Vertex Array objects, and for several Vertex Buffers, one just binds it to the corresponding chosen
	Vertex Array Objects, rather than always calling 'glVertexAttribPointer' and needing to specify the layout explicitely for every Vertex Buffer made.<br>

	So without a Vertex Array Object being specified, for a specific Vertex Buffer, anytime its layout needs to be changed one will
	have to always call 'glVertexAttribPointer'.
	Now this 'glVertexAttribPointer' implicitely creates a Vertex Array object, and hence for every different layout, the state of the prior is not
	saved.
	So another advantage of Vertex Array Objects is that they can be saved and used later even during runtime by simply binding the chosen VBO to it.<br>
	Likewise, if there are several Vertex Buffer Objects, thehy will need to be bound to an implicit VBO using 'glVertexAttribPointer' everytime during execution.
	So there will be binding everything needed for the current draw everytime.

	Now, the OpenGL compatibility program creates a VertexArrayObject implicitely using glVertexAttribPointer when 0 is passed to it
	BUt the core does not

#	Episode 13: "Abstracting OpenGL into Classes" -- date: Sun-17-Nov-2024
	All that is done is abstracting the code to make it more readable, less verbose.


#	Epsiode 14: "Buffer Layout Abstraction in OpenGL" -- date: Sun-17-Nov-2024

	The Buffer Layout is specified by the Vertex Array Object.
	Hence this is abstracting the Vertex Array Object.

	*But Why, What's the Purpose of Abstracting a Vertex Array Object?*
		A Vertex Array's purpose is to tie a Vertex Buffer to a specified layout.
		Vertex Buffers are just bytes, plain data.
		The Vertex Array ties the buffer to a layout, telling OpenGL how big each vertex is, what strides make up one vertex, etc.
		Then an Index Buffer is also attached to it.

	Hence the goal is this:
		Create Vertex Array Object, vao1, and give it a layout.
		Then for every Vertex Buffer that should use that layout, bind that Vertex Buffer to vao1 so OpenGL reads that Vertex Buffer
		using the layout specified by vao1.

		OR

		Create a Vertex Array Object factory class that consists of several layout specifications.
		Then for your several Vertex Buffer objects, depending on which layout is preferred for each corresponding Vertex Buffer Object...
		prompt the VAO Factory for a vao of that layout and bind the chosen vbo to that layout.
		Then bind that vao to the program so it is used.

	The important thing is to define a layout. It enables one to read the actual Vertex Buffer data, even on the CPU side without needing tools
	that can read the data from the GPU, for debuffing
	Another importance is a Picking Cache so that when a user clicks on the screen with the mouse, that rendered object can be selected by firing a
	ray and checking whether it intersects with a triangle in the model.
	To do this, the Vertex Buffer with a layout will need to be stored on the CPU side.


#	Episode 15: "Shader Abstraction in OpenGL" -- date: Tue-19-Nov-2024

In games and game engines it's normal to have a custom shading language that compiles to shader code for each type of Graphics API.
Hence games generate new shaders and uses them while the game is still running.
Shader Generation is quite common.

Objectives:
	1.	Pass in a file and have it compiled as a shader.
	2.	Bind the shader and Unbind the shader
	3.	Set Uniforms for the shader

#	Episode 16: "Writing a Basic Renderer in OpenGL" -- Date: Fri-22-Nov-2024

The Goal:	Create the Renderer, a central pipeline that acts as the API between the arranging of data to render and actually rendering
that data. The Renderer is where rendering should go through basically.
It makes rendering of things abstracted in methods so it can be done as many times as possible.
Moreover, the renderer keeps the code clean and enables debugging easier.

E.g. The code, `GLCall(glClear(GL_COLOR_BUFFER_BIT));` and 
	`GLCall(glDrawElements(GL_TRIANGLES, ibo.GetCount(), GL_UNSIGNED_INT, nullptr));` and
	`GLCall(glfwSwapBuffers(window));`
	are things that the Renderer should handle


#	Episode 17: "Textures in OpenGL" -- Date: Fri-22-Nov-2024

Texture: an image that can be used when rendering something.
It can be from to apply the texture to a Rectangle face in the program, to using pre-calculated
math values baked into the texture and then sampling them in the shaders to do some lighting effects.

But here, we just get the image from the computer unto a surface in the OpenGL app.

Steps:
	1.	Load image into CPU memory. Now, game engines don't usually use PNGs; most game engines have their own
		texture format for various reasons like optimizations, etc.
		There are different kinds of PNG formats, e.g. there is the compressed PNG format that requires some decoding.
		Hence, we use a library called STB_Image.
	2.	It will load the PNG: It reads the image contents via its file path and then returns a pointer to a buffer
		of RGBA pixels.
	3.	Then take the pixel array and upload it to the GPU as a 'Texture' using OpenGL, the graphics specification.
	4.	Then the shader is modified so that the Fragment/Pixel shader will read from the Texture memory and work out
		the colour of each pixel referencing the Texture:
			Bind the texture when its time to render, sending it to the Fragment shader.
			Then modify the shader to use that texture, sampling it when drawing the Rectangle.

STB Image is a single file. It is gotten from here: https://github.com/nothings/stb/blob/master/stb_image.h
Then the raw text is taken

Morealso, normally, the path to the stb__image folder would be inclided in the Project's include paths/directories
so no need for #include "vendor/stb_image/stb_image.h", just #include "stb_image.h"
       
        *   Texture Coordinates
        * 
        *   Now these tell the gemoetry being rendered which area of the Texture to render from.
        *   The pixel shader goes through and rasterizes eveery pixel of the rectangle 9in this case);
        *   it is responsible for determine the colour of each pixel at each position within the Rectangle's area
        *   By specifying the texture coordinates, one specifies the area that the Dragment shader should read from
        *   to sample the pixel color and use that to determine which color and where it should be rendered on the Rectangle.
        *   
        *   It starts from bottom left (s=0, t=0) to top right (s=1, t=1), where s represents horizontal position
        *   and t represents vertical position.
        *   so s=0.5, t=0.5 is halfway.

		Now, in the shader, something called a 'Varying' is used to send data from the Vertex Shader to the Fragment shader
		This is needed because the Texture Coordinates are in the Vertex Shader and need to be moved to the Fragment shader.


#	Episode 18: "Blending in OpenGL"

OpenGL does not perform blending by default; it takes whatever is being rendered and renders it in an opaque form,
disregarding every Transparent pixels.


#	Episode 19: "Maths in OpenGL"

Just talks about how maths is used in general and starts writing the Maths library.

Matrices: An array of numbers used to transform vectors and planes

Vectors: They are of two types:
	1.	Directional
	2.	Positional

*Used the free math library, glm* at **https://github.com/g-truc/glm/releases**

Normally, if writing your own engine, you should write your own math library.

glm is openGL specific; the mat't
glm's matrices are column-major ordering, which is the way the matrice is laid out in memory in OpenGL
so OpenGL does not need to do transposing to convert it to a suitable way for it.


#	Epsidoe 20: "Projection Matrices in OpenGL"

Projection is part of the Transformation pipeline.

Projection in OpenGL: How to transform an arbitrary coordinate system to map them to the 2D screen.
It's how a massive 3D world is rendered on your 2D monitor/screen.
Projection is the maths that converts the 3D points into points that fit the 2D monitor.

In OpenGl, all the vertex points sent to the GPU are converted to the normalized device coordinate space.
This space is then mapped to the window.
This space is a coordinate system between -1 and 1 in every axis
Resolution does not matter; the left side is -1, right +1, bottom -1 ,top +1

what the Projection matrix does is to convert a 3D world's coordinates to that normalized space
    elaboration:
        Changed the normalized rendering coordinates using a Projection matrix.
        that is, changed the normalized world coordinates from left -1, right +1, top +1, bottom -1
        to different coordinates, e.g.
                 1. left -2. right 2, top 1.5, bottom -1.5  -- maintained 4:3 aspect ratio  
                 2. left -4, right 4, top 3, bottom -3      -- maintained 4:3 aspect ratio
 
  
        In the original, only points between +1 and -1 could be displayed
        In the second, points between -2 and 2 for both axis
        In the third, between -4 and 4
        
        Later, after changing screen width x height -> 960, 540...
        then having changed the normalized coordinates to:
                left 0, right 960, top 540, bottom 0
        Only points between 0 and 960 in the x-axis could be visible
        while only those between 0 and 540 in y-axis could be visible.
  
    The concept is not difficult; By default normalized coordinates in OpenGL is left -1, right 1, top 1, bottom -1
    Usually, in GLSL, this is represented by the `fragCoord` variable in the GLSL language.
    Then iResolution is the screen's actual resolution.
  
    So people would normalize this fragment shader render coordinates to fit what they want, which is what
    is being done in this episode.
  
    Examples of what people do:
    
    normalized_coord = (fragCoord - 0.5*iResolution.xy) / iResolution.y
    if iResolution = (x{800}, y{450})...
    new_ires = 0.5 * (800, 450) = 400, 225
    new_fragCoord = fragCoord - new_ires ->> left -400, right 400, top 225, bottom -225
    
    new_fragCoord / iResolution.y ->> left -0.89, right 0.89, top 0.5, bottom -0.5
  
    So in the end, the goal is to make a normalized_coord that can accommodate a larger range of values or points
    so that it is mapped to the original fragCoord by converting that value to be between the OG:
        left -1, right +1, top +1, bottom -1, as explained above and demonstrated below.
  
    Because if you see what the values here:
            glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
	The above code effectively meams, map values within range 0.0f - 960.0f and between 0.0f and 540.0f to between -1.0f and 1.0f
	although know that it isn't the arguments -1.0f, 1.0f that specify this. Those last arguments are for the z axis: zFar and zNear.

	Hence, when this vertex is defined:
            glm::vec4 vp(100.0f, 100.0f, 0.0f, 1.0f);

	...and multiplied with the proj matrix
            glm::vec4 result = proj * vp
  
    It is seen that `result`'s x and y values of vp were converted and mapped between -1.0f and 1.0f:
  
            vp.x 100 -> vp.x = -0.791666627
            vp.y 100 -> vp.y = -0.629629612
	This kind of Projection matrix is a Pixel-Space Projection because every unit is one pixel.
    So, this whole process simply specifies a Projection so that it can accommodate points withing that projection's range.
    Then, the Projection Matrix converts each point it multiplies to fit the permanent normalized render coordinate system
    that the GLSL language uses, left -1, right +1, top +1, bottom -1.


#	Episode 21: "Model View Projection Matrices in OpenGL"

MVP matrices is the transformation matrix sequence for rendering

Model, View, Projection matrix -- this is the order that they are multiplied:
	M * V * P * vertex
But in OpenGL, because its matrices are column-major, so it's multiplied this way:
	P * V * M * vertex

These matrices are different; they define different things for the vertex position.
After all the matrices are multiplied together, the final matrix is multiplied with the vertex position
to perform some transformation -- from 3D point to 2D point on monitor/screen.

that is, from whatever space (orthographic or perspective space) to the normalized render coordinate:
	left -1, right +1, top +1, bottom -1

**The View Matrix**:
It represents the view of the camera, transforming the vertex, mapping it according to the view of the camera.
It is used to simulate a camera.
This is the transforms (position, rotation, scale or shear) of the camera.

**The Model Matrix**:
The model is the vertex being drawn, while the matrix is a transformation of the model.
This is the transforms (position, rotation, scale or shear) of the object.

The result of multiplying to the Projection Matrix is to transform the points (vertices) to the normalized
rneder coordinate system.

A camera matrix and A model matrix is added.

Camera:
	the camera matrix transforms the points such that when the camera moves to the left, the vertices
	in the world are moved to the right.
	Here, a translation matrix is created.

the projection * view * model are multiplied to represent the transformation on the object.
These are all that's needed to be known to position objects in a 2D or 3D world.

They are used to translate the image horizontally and vertically by 200px

The next episodes would look at adding perspective/using a Perspective Projection matrix that maps the 3d points
to the 2D normalized coordinates for rendering on the screen.
But because of this, a way to update the code is needed using imGUI, to prevent needing to open and close the program continually.


#	Episode 22: "ImGui in OpenGL"

Objective: Integrating ImGui. 

Dear ImGui: A GUI library usable in OpenGL, DirectX -- it's API independent. 
With it, one can display GUI items and hence it's useful for debugging and making apps quickly in a
realtime rendering setting, like a graphics. 

Downloaded imgui source from: https://github.com/ocornut/imgui/releases. 

Then included it into project, that is, the header and .cpp files. 
Then also took some example files.

WHat was done:
	1.	ImGui was setup.
	2.	Windows were made, and how to make and interact with GUI components was done.
	3.	Use of the Slider to change the Model's Matrix made it move across the screen and update in real time.

Only Issue:
	Something to do with the vertical view port in the display.
	To fix it, needed to modify the view matrix:
		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100.0f, -100.0f, 0));

	Then it worked. Perhaps the reason has to do with the Point vertices of the model itself.
	perhaps with the centering, hence why subtracting 100.0f from both the vertical and horizontal worked.

	The reason was because the Point vertices where not centered at the centre of the screen when defining its vertices.

#	Episode 23: "Rendering Multiple Objects in OpenGL"

As it says, it's drawing several objects unto the screen.

In the render method, from the Renderer class, these are done:
	shader is bound
	vao is bound
	ibo is bound
	Then the Draw call on the vao is made using the ibo and shader

The shader is a program that just specifies is telling the GPU how, where and when to draw something. 
The Vertex Array contains all the data and specifies its layouts and variables; it incudes the vertex buffer and index buffer. 
The Vertex Buffer has the positions of points to render, texture coordinates, and can have normals, tangents, etc. 
The Index Buffer has the indices into the Vertex Buffer so one can choose which vertices are to be rendered and how they should be assembvled

Then glDrawelements says use that index buffer to access the vertex array buffer and call the shader program, passing each of those
vertices individually and draw and rasterize the object on the screen.


To render multiple objects, there are two main methods:

	1.	Create a second Vertex positions, so another Vertex Buffer with all the positions of the second object.

	2.	One could change the Model-View-Projection matrix uniform; one could give the same object's set of positions
		a different model matrix which would position the object somewhere else in the screen. 
		The result is two of the same object at diffferent parts of the screen, the same texture. 

		This solution is preferred as there's no need to make another set of Positions and create another Vertex Object as
		this will take memory and is redundant.
		Though binding new Vertex Buffer objects is still faster than transfering data from CPU to GPU via uniforms.

		This techniqiue of providing different set of uniforms everytime one draws is common

		All the manipulation to accomplish this is taken care of by the shader using the MVP uniform.

In the episode, the second way is done: two different model-view-projection matrices will be used.


However, this is not what you do to render several things. E.g. Using a for loop to render a thousand tiles on the screen, but it
will be slow since there will be thousands of rendering calls, which is performance draining.

Rendering multiple objects can be done using a single draw call; it's called Batch Rendering. 
This is like the Method 1, but here when all the vertex positions for all the objects to render are added to a single Vertex Buffer
and everything is rendered at once since it contains all the positions of every tile that one has.

It's thousands of times faster than binding a shader, changing the mvp uniform using a for loop for a thousand tiles.

Still, this way would be how to render very complex 3D objects that should not be rendered severally. 

**However, Batch rendering will be done later.**

Now, with this Method 2 of rendering multiple objects, the thing different between them is the Uniforms. 
This introduces Materials, which are a wrapper for a Shader and a bunch of Uniforms.


#	Episode 24: "Setting Up a Test Framework in OpenGL" -- Sun-08-Dec-2024

This is a pre-requisite to continuing further; the aim is to make a test framework.
This is an "encapsulation" of different scenes; it shows the working of each test scene, which is
the point of this framework. 

This framework could now have debugging capabilities added to it as an extension of its functionality
and since it is to **test** how well the example scene works. 

It provides a way to view every scene, like a library of scenes, using the GUI (Imgui) and then to select which
you want. This is a menu and when one selects the scene, it loads everything in the scene and when the 'Back button'
is pressed, it goes back to the previous menu.


#	Homework:

	Create a Test Menu with Imgui that displays all the tests.
	Implement it so that one can select a test to run it. Then when the Back button is
	clicked, it goes back.

	Did it in "ep24_quest-test_menu"


#	Episode 25: "Creating Tests in OpenGL" -- Sun-08-Dec-2024

This is a continuation from the Episode 24

Here, The Cherno shows one of the ways one can implement the Test Menu app; his own way.


#	Episode 26: "Creating a Texture Test in OpenGL" -- Mon-09-12-2024

Also a continuation from Episode 25.

But here demonstrates the creation of a Sandbox application that displays a menu
where one can display all the different tests, which can be chosen to view what the pieces of code
do -- though it was not created as a separate project, but in a seperate folder like the previous episode.

This episode shows the creation of a test; the test is the quad that displays a texture.

**There was and issue that resulted from the Vertex Buffer being deleted**
This issue was fixed by making the Vertex Buffer Object (VBO) a member of the class to extend its lifetime.

Because there is a one-to-one mapping of the Cpu obejcts to the GPU objects.
So when the VBO on the CPU was deleted, it was also deleted from the GPU memory.

Though there is a way one could design the app such that the vertex buffer class *just* sends data to the GPU.
This would not require a live VBO object on the CPU side, thereby mitigating the issue.

But in this episode, it wasn't done in that way. 


#	Epsiode 27: "How to make your UNIFORMS FASTER in OpenGL" -- Mon-09-12-2024

This uses Caching to make setting Uniforms faster!

Instead of retrieving uniforms from the GPU everytime one needs to set a uniform, there are some
techniques to not needing to continously query the GPU about where the Uniforms are because the GPU
already has this information.

The subject is Caching.

The concept is that when the Shader program is compiled into source code, it can be read with C++
and the Uniforms' locations and values can be gotten to validate its existence.

So it can read the source, convert it to the appropriate data. Now, once can get this data and use it in runtime.

**BUT** the Cherno has already shown the implementation of this previously: it involves the use of an unordered map
to catch the uniform locations, using their names as the key that maps to a GLint value that represents that uniform's
location.

So the source code of the compiled shader program is not really read. All that prior explanation is something one
would have done if it was a bigger program.

However, the Shader class is modified.

**Summary** 
This is pretty neat! Instead of prompting Uniform locations every time, just use a Shader Cache


#	Episode 28: "Batch Rendering - An Introduction" -- Mon-9-12-2024

Definition: How to render > 1 piece of geometry with one draw call.

##	Elaboration: 
	Normal Rendering: Create VAO, bind IBO and VBO, and render with the GLDrawElements call/
	hence to draw another object, repeat above process.
	Now, if the geometry does not change, one can bypass the above normal rendering by just using Uniform matrices
	sent to the Shader to render the same object in a different position -- simply by changing the model matrix
	or the view or projection matrix of that second object.
	The latter works because one is sending to the shader different Vertex data, since it has been modified by the matrices...
	and calling the GLDrawElements call the second time to draw the object with modified positions.

	But the latter does not work for renders of different gemoetry, hence the need for Batch rendering.


	Batch Rendering improves performance drawing thousands of geometry with a single draw call by drawing each quad at the
	same time, not individually per draw call

	Also, batch rendering allows one to also make particles. The mnore the particles the less efficient the normal rendering.
	However, one can use different transforms for each particles to render multiple different-sized ones.
	But even better is to use batch rendering.

	Another is rendering a lot of text characters on the screen; batch rendering improves this.

Batching: batch all the geometry into a single vertex buffer and index buffer and simply drawing that once.
This causes a huge performance improvement, so much that the Cherno wants to benchmark this.

This video explains how to do this.

Basically, the normal draw call is slow because of the afore-explained reasons and these:

	the vertex buffer contains the vertices for a single quad; likewise the index buffer contains
	the indices for a single call.
	Then to draw 1000 of these quads, 1000 draw calls with different transforms applied to them will have to be made.
	Say a quad has 4 vertices and 6 indices

	This is **SLOW/BAD**

For Batch Rendering:

	For example, to draw 1000 quads, 1000 * 4 vertices are needed for the vertex buffer...
	and 1000 * 6 indices for the index buffer.

	Then a single draw call is all that's needed. This is **FAST/GOOD**

	Now, to move a single quad, a transform to that quad's vertices needs to be done.
	This is because the buffer contains the positions(vertices) of that quad and all of them.

	Now, one can also apply a transform to all the quads' positions, like in the case of a camera.

	Also to set the color of each quad, their color data would be put in the Vertex Buffer.
	So each vertex (the data for a single quad -- includes its positions[vertices] and color map) will have a color
	associated with it.

	Now, this means that for quads that are constantly moving, since their positions are constantly
	changing, the Vertex Buffer has to become **Dynamic**

	This is essential because unless the Vertex Buffer is specified as dynamic, it will be static.


This video also demonstrates how to do this but using The Cherno's OpenGL Sandbox.

Likewise, all the batch rendering episodes are done in the Sandbox...

However, I do it here regardless. I may definitely make something like The Cherno's Sandbox


#	Episode 29: "Batch Rendering - Colors"

Normally, when rendering a quad, the Uniforms for colors would
be set before calling the Draw call.
So for the different quads, before calling each of their draw calls, the uniform
with the appropriate color is set. THis will give them different colors.

However, because there is only one draw call for batch rendering, the above cannot
be done.

The solution is to add the colors into the vertices variable and send that to the GPU
through the VAO rather than using a Uniform.

So the color would passed from the Vertex SHader to the Fragment shader.

So now since each vertex has its own color, each quad can be given its own color.
Moreover, with this, one can do tricks like color gradients.

#	Episode 30: "Batch Rendering - Textures"

This episode demonstrates the way to add textures unique to each object using Batch Rendering.

If it was normal rendering, the appropriate Texture uniform would be set before calling
the Draw Call for that quad or object.

But using Batch rendering, Unifomrs cannot be used, so the Texture coordinates are added to
the Vertex data (or vertices).

One can use Sprite sheets to do this, for example. 
the main thing is specifying the Texture coordinates both in the source (from image) and in the Vertex data.

However, in this episode, separate textures are used to make it easier.

Hence the texture coordinate data is put in the vertex buffer and then sent to the Vertex data.
This way, each vertex and so each quad has its oen texture.

Now, if you look at the code from Colors and this Textures code in the appropriate tests: "TestBatchRenderingColors" and "TestBatchRenderingTextures"
you will see that the VertexAttribPointer function, controlled by the VAO, give an ID to each vertex data added

It is controlled in a very cool way by the VertexBufferLayout and VertexArray classes, such that the number of layouts corresponds to a certain index.

Consider this:

        float vertices[] = {

             -50.0f, -50.0f, 0.18f, 0.95f, 0.55f, 1.0f,
             50.0f,  -50.0f, 0.18f, 0.95f, 0.55f, 1.0f,
             50.0f,   50.0f, 0.18f, 0.95f, 0.55f, 1.0f,
             -50.0f,  50.0f, 0.18f, 0.95f, 0.55f, 1.0f,

             70.0f,  -50.0f, 1.0f, 1.0f, 1.0f, 1.0f,
             170.0f, -50.0f, 1.0f, 1.0f, 1.0f, 1.0f,
             170.0f,  50.0f, 1.0f, 1.0f, 1.0f, 1.0f,
             70.0f,   50.0f, 1.0f, 1.0f, 1.0f, 1.0f,
            
             190.0f,  -50.0f, 0.18f, 0.95f, 0.55f, 1.0f,
             290.0f, -50.0f, 0.18f, 0.95f, 0.55f, 1.0f,
             290.0f,  50.0f, 0.18f, 0.95f, 0.55f, 1.0f,
             190.0f,   50.0f, 0.18f, 0.95f, 0.55f, 1.0f,
        };

		 layout.Push<float>(2u);		id 0
        layout.Push<float>(4u);			id 1

But for textures, they can be given an index that identifies each. In this one, each texture's index is put in the Vertex Data and
sent to the Vertex SHader.
Then this index is passed from the Vertex Shader to the Fragment Shader. This index identifies the texture that has been bound. So
each vertex being processed by the Vertex Shader has it's own texture index, and so the texture for theat specific vertex can be gotten.

As a result, one can know the texture that belongs to which quad.

Now, there is a limitation to how many different textures can be sent to the GPU at once, depending on the platform.
Hence, if one needs to render different textures past this limitation, you will need to break up the batch.

to give different draw calls to accommodate the surplus texture slots.
E.g. you have 32 available.
When you'vre rendered the first 32, then flush, and then render the remaining 32 textures for as many batches as possible.
So if you have 32 * 4 textures, only 4 draw calls are made; saving a lot of speed.


#	Episode 31: "Batch Rendering - Dynamic Geometry" -- Mon-09-12-2024

Demonstrates the creation of the Dynamic Buffer Object: **Dynamic Batching** or **Dynamic Batch Rendering**

There are two ways to produce the effect:
	
	Using a Dynamic Buffer  and/or Using a Dynamic Index Buffer

This episode focuses on the Vertex Buffer.

It involves not predefining the vertices that define an object like a quad, but modifying the vertex values
of the Vertex Buffer after making it dynamic, so the values can be modified during runtime, every frame.

This involves copying bytes of memory into the Vertex Buffer at a specific location.

Steps:
	1.	Use the initialization function (or constructor) to only allocate memory large
	enough to accommodate the data to be stored.
	2.	In the Render function that's run every loop, all the Vertex Data is copied to the GPU where it
	can be accessed and rendered normally.


Likewise, the Index Buffer can be made dynamic; I did this one myself.


With this, literaly done with The Cherno OpenGL series!


##	**What I Learnt From Trying to Implement Dynamic Indices **
	
	See, modfying the indices buffer that is added dynamically via glBufferSubData, it works exactly as the one for dynamiuc
	Vertex Buffers.
	For Vertex Buffers use GL_ARRAY_BUFFER:
		 GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices));
	For Index Buffers use GL_ELEMENT_BUFFER:
		 GLCall(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(indices) * (unsigned int)m_QuadCount * numOfIndicesPerQuad , indices));

	Now, it worked. But the *issue* was that, when I increase the indices, adding more Quads, it worked. But when I decreased
	it, it didn't work.

	SOL:
		The reason behind the issue was that the GPU stores the completed information. Even though the indices were removed
		dynamically, the Quad was still rendered because the associated Vertex Buffer still existed.
		Hence the solution was to use the m_QuadCount variable to control the creation of the Vertex Buffer data as well
		as the number of indices.
		It's solution is demonstrated in "TestBatchRenderingDynamicGeometry.cpp and .h"