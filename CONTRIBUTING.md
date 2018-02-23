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


#### Build Docker images
Navigate to the folder containing the Dockerfile and run. This will create a Docker image with name imageName.

`docker build -t imageName -f Dockerfile .` 

A Docker image will be created and tested (assuming tests are included in the Dockerfile) in accordance to the specifications of the Dockerfile. Follow the instructions below to save, load and run images.

#### Saving software bundle

The project is easily distributed by saving docker images as a tar archive. This is done with the following command 

`docker save imageName > bundleName.tar`

#### Load software bundle 
You want to load the software bundle onto the miniature smart vehicle. This can be done either by cloning the project and building, or by importing the software bundle. Load the software bundle by navigating to the where the bundle is located and run

`cat bundleName.tar | docker load`

#### Run software bundle
To run a Docker image in a container you may simply run `docker run imageName`.
If you, however, wants to run our example programs you have to make sure that you are enabling the following flags:
`--rm` Docker to automatically clean up the container and remove the file system when the container exits
`-ti` Opens an interactive terminal
`--net=host` Provide the container, thus the image, with connection to the host's local ports/services
Consequently, the command to run is 

`docker run --rm -ti --net=host imageName`

#### Using Docker with the example program
You may try the aforementioned Docker commands on the example program called Communicator. Simply navigate to src/FirstSub/Communicator and follow the instructions above. Run the image created from the Dockerfile in two separate interactive shells to test if the programs are able to exchange data using the OD4Session.
