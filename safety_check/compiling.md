#Cross compile and run: 
##Ubuntu:
1. docker build -f Dockerfile.armhf -t safety_check .
2. docker save safety_check > safety_check
3. scp image to BB

##BB:
1. cat safety_check | docker load
2. docker-compose up
3. docker run --rm -ti --net=host safety_check .

#Compile natively and run:
##BB
1. docker build -f Dockerfile -t native_safety_check .
2. docker-compose up
3. docker run --rm -ti --net=host native_safety_check .
