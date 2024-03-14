pushd ./build

    meson setup --native-file conan_meson_native.ini .. meson-src --buildtype=release
    meson compile -C meson-src

popd 
