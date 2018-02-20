# Contributing to the project
### How to contribute
Before you start your contribution, create a gitHub issue in order to discuss  with the development team the changes or additions you would like to make. Same goes for reporting bugs and requesting bug fixes. 

Please note that GNU Lesser General Public Licence is used in this project. This means that any changes or contributions made by you must also follow the standards of this license.

#### Coding guidelines
- Use tabs, not spaces. 
- Treat warnings as errors when coding.

To be able to start making changes, you must first create a feature branch. All feature branches and merges should be done with the develop branch as origin. This is to ensure that the master branch is always in a deployable state.
This project makes use of pull request in order to ensure code quality. Your contribution to the project will have to be approved by the development team before a merge.

Update documentation accordingly, explaining what you did and how. Remember to update version number when applicable.  
Make sure that every contributed feature is accompanied by appropriate tests.  

### How to package and release the binaries of your software using Docker to upload binaries to the car.

Prerequisite to building and running a docker image is to have docker installed. [See documentation for instructions how to install docker](https://docs.docker.com/install/linux/docker-ce/ubuntu/#install-using-the-repository)

#### Saving software bundle

The project is easily distributed by saving docker images as a tar archive. This is done with the following command 

`docker save imagename > bundleName.tar`

#### Load software bundle 
You want to load the software bundle onto the miniature smart vehicle. This can be done either by cloning the project and building, or by importing the software bundle. Load the software bundle by running

`cat bundleName.tar | docker load`

navigate to folder, then run

`docker run --rm -ti --net=host receiver`

in order to run and test. 