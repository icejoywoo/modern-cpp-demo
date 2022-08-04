#!/bin/bash

# Minimal setup for Ubuntu 20.04.
set -eufx -o pipefail
SCRIPTDIR=$(dirname "${BASH_SOURCE[0]}")
source $SCRIPTDIR/setup-helper-functions.sh

# Folly must be built with the same compiler flags so that some low level types
# are the same size.
FB_OS_VERSION=v2022.03.14.00
NPROC=$(getconf _NPROCESSORS_ONLN)
DEPENDENCY_DIR=${DEPENDENCY_DIR:-$(pwd)}

# Install all velox and folly dependencies.
sudo apt install -y \
  g++ \
  cmake \
  ccache \
  ninja-build \
  checkinstall \
  git \
  libssl-dev \
  libboost-all-dev \
  libdouble-conversion-dev \
  libgoogle-glog-dev \
  libbz2-dev \
  libgflags-dev \
  libgtest-dev \
  libgmock-dev \
  libevent-dev \
  libprotobuf-dev \
  liblz4-dev \
  libzstd-dev \
  libre2-dev \
  libsnappy-dev \
  liblzo2-dev \
  protobuf-compiler

function run_and_time {
  time "$@"
  { echo "+ Finished running $*"; } 2> /dev/null
}

function prompt {
  (
    while true; do
      local input="${PROMPT_ALWAYS_RESPOND:-}"
      echo -n "$(tput bold)$* [Y, n]$(tput sgr0) "
      [[ -z "${input}" ]] && read input
      if [[ "${input}" == "Y" || "${input}" == "y" || "${input}" == "" ]]; then
        return 0
      elif [[ "${input}" == "N" || "${input}" == "n" ]]; then
        return 1
      fi
    done
  ) 2> /dev/null
}

function install_fmt {
  github_checkout fmtlib/fmt 8.0.0
  cmake_install -DFMT_TEST=OFF
}

function install_folly {
  github_checkout facebook/folly "${FB_OS_VERSION}"
  cmake_install -DBUILD_TESTS=OFF
}

function install_velox_deps {
  run_and_time install_fmt
  run_and_time install_folly
}

(return 2> /dev/null) && return # If script was sourced, don't run commands.

(
  if [[ $# -ne 0 ]]; then
    for cmd in "$@"; do
      run_and_time "${cmd}"
    done
  else
    install_velox_deps
  fi
)

echo "All deps are installed!"