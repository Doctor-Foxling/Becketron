# Becketron

<img src="https://github.com/Doctor-Foxling/Doctor-Foxling.github.io/blob/main/images/thumbs/Projects/Engine2.png">

This is a game engine written mostly in C++ that currently supports OpenGL based 2D and 3D rendering, PhysX-SDK based physics system, GUI via ImGui library (docking), 
Entity Component System through the Entt library and other features that can be explored in the engine <a href="https://doctor-foxling.github.io/Becketron-1.html">Documentation</a>.

The Current most upto-date branch for the repository is '3D-Experiment' where the 3D rendering, PhysX implementation, Skyboxes, Texture selection and other minor features are implemented.

Current project solution is composed of three projects, Becketron (static library), which is the game engine iteself and contains the implementation for most of the modules
(with the exception of GUI) that the engine is made of. The GUI setup (i.e., ImGui integration) is also done within the Becketron project. The implementation of the of GUI 
components (e.g., Scene Hierarchy Panel, Properties Panel, Viewport etc.) are implemented with the 'Tron' (Editor) Project.

<div align="center">
<img src="https://github.com/Doctor-Foxling/Doctor-Foxling.github.io/blob/main/images/thumbs/Projects/Game%20Engine/Engine_Architecture.PNG" width="75%">
</div>
<br>
The Sandbox project is a testing ground for the new features that are added to the Engine and isn't necessarily essentailly for using the engine as a game/simulation developer. Both,
Sandbox and Tron Editor Project have access to assets (e.g., Textures, Skyboxes etc.) and GLSL shader files, specific to a given scene.

## Building and Running

To run and test the enginge, the following steps can be taken:
<ul>
  <li><b>Clone the repository:</b> <code>git clone --recursive https://github.com/Doctor-Foxling/Becketron</code> </li>
  <li><b>Check out the latest branch:</b> <code>git checkout 3DExperiment</code></li>
  <li><b>Build the solution:</b>Use the batch file, <code>Win-GenProjects.bat</code>, in the <a href="">Scripts</a> folder to build the solution</li>
  <li><b>Open the solution in Visual Studio</b> (or any other editor)</li>
  <li><b>Set the startup project to Tron </b> (If not set already)</li> 
  <li><b>Ready to start debugging</b></li>
</ul>