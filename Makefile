# Targets here are just a convenience for using
# docker to manage the development environment.

image_develop:
	cppdock build linux_x64 \
	&& docker build -f ./docker/Dockerfile-develop -t full_duplex:develop .

develop:
	docker run --rm -it -v ${shell pwd}:/opt/src:ro full_duplex:develop
