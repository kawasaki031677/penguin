# penguin <img src="./image/tux.png" width="30" alt="Tux"> 

A terminal penguin command inspired by Tux, Linux's official mascot.
Distributed as a `.deb` package for Debian-based Linux systems.

<img src="./image/demo.gif" width="600" alt="penguin demo">

## Requirements

- Debian-based Linux (Debian, Ubuntu, etc.)
- A terminal that supports ANSI/VT100 escape sequences

## Installation

Download the `.deb` file and install it with the following command:

```sh
sudo apt install ./penguin_*.deb
```

To uninstall:

```sh
sudo apt remove penguin
```

## Usage

### Normal Display and Animations

```sh
penguin
penguin --run
penguin --dance
penguin --fly
```

### Help and Version

```sh
penguin --help
penguin --version
```

Press `Ctrl-C` to stop an animation.

## For Developers

See [docs/BUILDING.md](docs/BUILDING.md) for compilation, package creation, and testing instructions.

## Credits

Tux image created by Larry Ewing and The GIMP.
