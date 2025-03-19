#!/bin/sh

ADEVS_SITE=https://web.ornl.gov/~nutarojj/adevs
ADEVS_SRC=adevs-3.2

HIBL_SITE=https://github.com/paulftw
HIBL_SRC=hiberlite

FNSS_SITE=https://github.com/fnss
FNSS_SRC=fnss

FNSS_CPP_SITE=https://github.com/ballanty
FNSS_CPP_SRC=fnss-cpp

SQLITE_SITE=https://sqlite.org/2024
SQLITE_SRC=sqlite-amalgamation-3470000

SRC=src
LIB=lib
BIN=bin
INC=include

download()
{

    pushd ${SRC}

    if [ ! -d ${ADEVS_SRC} ]
    then
    wget ${ADEVS_SITE}/${ADEVS_SRC}.zip
    unzip ${ADEVS_SRC}.zip
    rm ${ADEVS_SRC}.zip
    fi

    if [ ! -d ${HIBL_SRC} ]
    then
    git clone ${HIBL_SITE}/${HIBL_SRC}.git
    fi

    if [ ! -d ${FNSS_SRC} ]
    then
    git clone ${FNSS_SITE}/${FNSS_SRC}.git
    fi


    if [ ! -d ${FNSS_CPP_SRC} ]
    then
    git clone ${FNSS_CPP_SITE}/${FNSS_CPP_SRC}.git
    fi

    if [ ! -d ${SQLITE_SRC} ]
    then
    wget ${SQLITE_SITE}/${SQLITE_SRC}.zip
    unzip ${SQLITE_SRC}.zip
    ln -s ${SQLITE_SRC} sqlite
    rm ${SQLITE_SRC}.zip
    fi


    popd

}

mkdir -p ${SRC} ${LIB} ${BIN} ${INC}


download



# Build ADEVS

pushd include
ln -s ../${SRC}/${ADEVS_SRC}/include adevs
popd


# Build FNSS-CPP

pushd ${SRC}/${FNSS_CPP_SRC}
make
make INSTALL_LIB=../../lib INSTALL_BIN=../../bin INSTALL_HDR=../../include install
cp lib/libfnss.a ../../lib
popd
pushd include/include
mkdir ../fnss-cpp
mv * ../fnss-cpp
popd
rmdir include/include


# Build Hiberlite

pushd ${SRC}/${HIBL_SRC}
cp ../sqlite/sqlite3.c sqlite-amalgamation
git apply ../../../../cmake/patches/hiberlite.local.patch
make
make INSTALL_PREFIX=../.. install
popd


# Make link to Brite in include

pushd ${INC}
ln -s ../BRITE BRITE
popd


# Make a copy of rapidxml to use

cp src/fnss-cpp/deps/rapidxml.hpp include
