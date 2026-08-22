# Building for Developers

## Requirements

Debian-based Linux and the following packages are required:

```sh
sudo apt update
sudo apt install build-essential debhelper devscripts libcmocka-dev
```

## Manual Compilation

Run the following command from the project root:

```sh
cc -std=c99 -Wall -Wextra -O2 -o src/penguin src/penguin.c src/options.c
```

The compiled binary is created at `src/penguin`.

## Building the Debian Package

```sh
dpkg-buildpackage -us -uc
```

The generated `.deb` file is created in the directory above the project directory.

## Verification

Run the cmocka unit tests with:

```sh
cc -std=c99 -Wall -Wextra -Werror -Isrc \
	tests/test_options.c src/options.c \
	$(pkg-config --cflags --libs cmocka) \
	-o tests/test_options
./tests/test_options
```

```sh
./src/penguin --version
./src/penguin --help
./src/penguin
./src/penguin --run
./src/penguin --dance
./src/penguin --fly
```

Interrupt animations with `Ctrl-C` and verify that the cursor and normal terminal screen are restored.