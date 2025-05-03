#!/bin/bash

# Compile the application
g++ -o sHASH sHASH2.cpp -lcrypto

# Create the directory structure
mkdir -p sHASH/DEBIAN
mkdir -p sHASH/usr/local/bin

# Create the control file
cat <<EOF > sHASH/DEBIAN/control
Package: shash.2
Version: 0.4
Section: utils
Priority: optional
Architecture: all
Maintainer: Bodi -abodi1@live.maryville.edu
Description: A secure file hashing tool
 This tool calculates the SHA-256 hash of a given file and outputs the result.
EOF

# Move the binary
mv sHASH sHASH/usr/local/bin/

# Build the .deb package
dpkg-deb --build sHASH

# Clean up
rm -r sHASH
