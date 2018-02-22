# Project Course
DIT168 Project: Industrial IT and Embedded Systems

## Group 01
Also known as dashFTab

## Introduction
The repository is used for DIT168 project course taught at University of Gothenburg. The developed software is intended to run on a ‘miniature smart car’ equipped with various sensors to allow for Vehicle-to-Vehicle (V2V) communication as well as autonomous driving to some degree. The V2V communication is to be conducted in collaboration with other teams, as part of a subsystem with focus on vehicle coordination.

## V2V Communication Manager
Justinas Stirbys (https://github.com/justasAtGU)

## GitHub Layout
GitHub issues are going to be used continuously throughout the project in order to monitor discovered bugs. As a means for all team members to familiarize with the code the team has decided to incorporate pull requests. The pull requests will further aim to monitor that the established quality requirements are met and improve collective code ownership. 
The team has agreed upon attempting to follow a specific branching model as close as possible to ensure that version control is utilized as the powerful tool it is to improve development. Two branches, master and develop, will be available throughout the project’s lifetime. The master branch will house the latest release. Additionally, release branches will be used for updating documentation, fixing bugs and issues, and small tasks required before a quick release. The hotfixes branches will be used to fix bugs found within the master branch. Once fixed, the branch will be merged back to master and release branch. Feature branches will be used to implement the required microservices.

## Clone, Build and Test
Open your terminal and navigate to where you want to store the folder containing the project. Once there, use the command git clone <url> to download the latest code located on the master branch of the project. To build and test the sender program: 
Go into /src/FirstSub/sender, run the command mkdir build to create a folder housing the build files and executables. Go into the build folder and run the command cmake .. . Now, run the command make and the project will be built according to the CMakeLists.txt file. Lastly, run make test to run potential test suit. To build and test the receiver program, go into /src/FirstSub/receiver and execute exactly the same commands as for sender. 
  
## Licence
The project is licenced under GNU Lesser General Public License version 2.1. This is due to the incorporation of OpenDaVINCI as part of the project. OpenDaVINCI offers their software under LGPLv 2.1 licence and due to the copyleft nature, anyone who distribute its code or derivative works, are required to make the source available under the same terms. LGPLv 2.1 grants permission to relicense the code under any version of the GPL. However, the development team has abstained from this. To view the detailed licence read LICENCE.md in source directory.

## Build Status 
[![Build Status](https://travis-ci.org/justasAtGU/dit168.svg?branch=master)](https://travis-ci.org/justasAtGU/dit168)
