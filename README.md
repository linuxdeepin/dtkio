# Development Tool Kit IO

Qt-based development library for I/O on Deepin.

中文说明：[README.zh_CN.md](./README.zh_CN.md)

## Dependencies

### Build dependencies

- cmake,
- doxygen,
- graphviz,
- pkg-config,
- libdtk6core-dev,
- libdtk6widget-dev,
- libkf6codecs-dev,
- libkf6archive-dev,
- qt6-base-dev,
- qt6-tools-dev,
- qt6-documentation-tools | qt6-tools-dev-tools,
- qt6-5compat-dev,
- libudisks2-qt6-dev,
- librsvg2-dev,
- libsecret-1-dev,
- libglib2.0-dev,
- libarchive-dev,
- libsecret-1-dev,
- libpoppler-cpp-dev,
- libzip-dev,
- libminizip-dev,
- libisoburn-dev,
- liblucene++-dev,
- libdocparser-dev,
- libboost-filesystem-dev

## Build and install

### Build from source code

1. Make sure you have installed all dependencies.

   ```bash
   git clone https://github.com/linuxdeepin/dtkio
   cd dtkio
   sudo apt build-dep ./
   ```

2. Build

   ```bash
   cmake -B build -DCMAKE_INSTALL_PREFIX=/usr
   cmake --build build
   ```

### Install

```bash
sudo cmake --build build --target install
```

## Getting help

Any usage issues can ask for help via

* [Telegram group](https://t.me/deepin)
* [Matrix](https://matrix.to/#/#deepin-community:matrix.org)
* [IRC (libera.chat)](https://web.libera.chat/#deepin-community)
* [Forum](https://bbs.deepin.org)
* [WiKi](https://wiki.deepin.org/)

## Getting involved

We encourage you to report issues and contribute changes

* [Contribution guide for developers](https://github.com/linuxdeepin/developer-center/wiki/Contribution-Guidelines-for-Developers-en).

## License

Development Tool Kit is licensed under [LGPL-3.0-or-later](LICENSE).
