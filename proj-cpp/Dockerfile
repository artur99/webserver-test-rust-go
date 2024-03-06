# from alpine
FROM alpine:3.19

# Install the necessary packages
RUN apk add --no-cache g++ make cmake perl
RUN apk add --no-cache python3 py3-pip
RUN pip3 install --break-system-packages conan
RUN apk add --no-cache meson ninja
RUN conan profile detect

# Add dependencies
ADD ./conanfile.txt /proj-cpp/conanfile.txt
WORKDIR /proj-cpp
RUN conan install . --output-folder=build --build=missing

RUN apk add --no-cache pkgconfig

# Add the project files
ADD . /proj-cpp

# Build the project
WORKDIR /proj-cpp/build
RUN meson setup --native-file conan_meson_native.ini .. meson-src
RUN meson compile -C meson-src

# Run
CMD ["/proj-cpp/build/meson-src/server"]
