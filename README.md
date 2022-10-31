## DeadSpace Trainer
A trainer for the game Dead Space(2008). This is a basic internal DLL trainer for the game Dead Space which has the following functionality: infinite health, infinite ammo, infinite air, infinite stasis, infinite entity damage, and user-specified credit and node setter. This project was written in C++ using Visual Studio 2017 IDE.

<p align="center">
  <img src="https://user-images.githubusercontent.com/52585921/117048270-cbfded00-ace0-11eb-810f-f67574363109.jpg?raw=true" alt="DeadSpace Trainer Image"/>
</p>


## Instructions
To use this application the user must either compile the project themselves or download the DLL provided. The user must also use a DLL Injection application to inject the DLL mentioned into the Dead Space process. Below I have provided step by step instructions on one of the methods 

### Step By Step
  * Download Cheat Engine from https://www.cheatengine.org/downloads.php
  * Download the *"DeadSpaceCheatDLL.dll"* file from this repository
  * Launch the Dead Space video game and attach Cheat Engine to the *"Dead Space.exe"* process
  * Once Cheat Engine has attached navigate to the DLL injection tab by going to *Memory View -> Tools -> Inject DLL*
  * Pick the downloaded DLL and confirm the injection by clicking *"Yes"* on the *"Confirmation"* window
  * Enjoy playing the game with the trainer :)
  

## Project Goal
My goal for this project was to apply my pre-requisite knowledge of operating systems in a fun and challenging way. I also wanted to learn more about the Windows API(Win32), Reverse Engineering, x86 Assembly, and GDI. I also used this as an opportunity to expand my knowledge of C++ programming and OOP.
 
## Help
This project could not have been built without the excellent tutorials provided by GuidedHacking, as well as ChiliTomatoNoodle. The process of finding offsets, reverse engineering, and writing the code required for memory reading/writing was learned from GuidedHacking. While the process of creating the User-Interface with Win32 was learned from ChiliTomatoNoodle.
