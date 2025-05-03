#!/bin/bash

# Compile the application
g++ -o sHASH sHASH2.cpp -lcrypto

# Create the directory structure
mkdir -p sHASH/DEBIAN
mkdir -p sHASH/usr/local/bin

# Create the control file
cat <<EOF > sHASH/DEBIAN/control
Package: shash
Version: 1.0
Section: utils
Priority: optional
Architecture: amd64
Maintainer: Alexander Bodi <your-email@example.com>
Description: A secure file hashing tool
 This tool calculates the SHA-256 hash of a given file and outputs the result.
EOF

# Move the binary
mv sHASH sHASH/usr/local/bin/

# Build the .deb package
dpkg-deb --build sHASH

# Clean up
rm -r sHASH
