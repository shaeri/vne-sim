# -*- mode: cmake -*-
#
# This file is part of the Feel++ library
#
# Author(s): Christophe Prud'homme <christophe.prudhomme@ujf-grenoble.fr> Date:
# 2010-02-10
#
# Copyright (C) 2010 Université Joseph Fourier
#
# This library is free software; you can redistribute it and/or modify it under
# the terms of the GNU Lesser General Public License as published by the Free
# Software Foundation; either version 3.0 of the License, or (at your option)
# any later version.
#
# This library is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
# details.
#
# You should have received a copy of the GNU Lesser General Public License along
# with this library; if not, write to the Free Software Foundation, Inc., 51
# Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
#
# this files defines - GLPK_INCLUDE_DIR - GLPK_LIBRARIES - GLPK_FOUND

include(CheckIncludeFileCXX)
check_include_file_cxx(glpk.h FEELPP_HAS_GLPK_H)

find_library(GLPK_LIB glpk PATHS /usr/lib $ENV{GLPK_DIR}/lib)
set(GLPK_LIBRARIES ${GLPK_LIB})

find_path(
  GLPK_INCLUDE_DIR glpk.h
  PATHS /usr/include/ /usr/include/glpk $ENV{GLPK_DIR}/include
  DOC "Directory where GLPK header files are stored")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GLPK "Could not find GLPK " GLPK_INCLUDE_DIR
                                  GLPK_LIBRARIES)
# show the BERKELEY_DB_INCLUDE_DIR and BERKELEY_DB_LIBRARIES variables only in
# the advanced view
mark_as_advanced(GLPK_INCLUDE_DIR GLPK_LIBRARIES)
