# Building for Developers

## Requirements

Debian-based Linux and the following packages are required:

```sh
sudo apt update
sudo apt install build-essential debhelper devscripts
```

## Manual Compilation

Run the following command from the project root:

```sh
cc -std=c99 -Wall -Wextra -O2 -o src/penguin src/penguin.c
```

The compiled binary is created at `src/penguin`.

## Building the Debian Package

```sh
dpkg-buildpackage -us -uc
```

The generated `.deb` file is created in the directory above the project directory.

## Verification

```sh
./src/penguin --version
./src/penguin --help
./src/penguin
./src/penguin --run
./src/penguin --dance
./src/penguin --fly
```

Interrupt animations with `Ctrl-C` and verify that the cursor and normal terminal screen are restored.