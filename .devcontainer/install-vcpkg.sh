#!/usr/bin/env bash
#-------------------------------------------------------------------------------------------------------------
# Copyright (c) Microsoft Corporation. All rights reserved.
# Licensed under the MIT License. See https://go.microsoft.com/fwlink/?linkid=2090316 for license information.
#-------------------------------------------------------------------------------------------------------------
#
# shellcheck disable=SC1090,SC1091
#
# Docs: https://github.com/msclock/features/blob/main/src/vcpkg
# Maintainer: msclock

set -e

# log with color
_log() {
    local level=$1
    local msg=${*:2}

    _preset="clear"
    case "$level" in
    "red" | "r" | "error")
        _preset='\033[31m'
        ;;
    "green" | "g" | "success")
        _preset='\033[32m'
        ;;
    "yellow" | "y" | "warning" | "warn")
        _preset='\033[33m'
        ;;
    "blue" | "b" | "info")
        _preset='\033[34m'
        ;;
    "clear" | "c")
        _preset='\033[0m'
        ;;
    esac

    echo -e "$_preset["${level^^}"]:\033[0m $msg" 1>&2
}

USERNAME="${USERNAME:-"${_REMOTE_USER:-"automatic"}"}"
VCPKG_ROOT="${VCPKGROOT:-"/usr/local/vcpkg"}"
VCPKG_DOWNLOADS="${VCPKGDOWNLOADS:-"/usr/local/vcpkg-downloads"}"
VCPKG_VERSION="${VCPKGVERSION:-"stable"}"

MARKER_FILE="/usr/local/etc/features/vcpkg"

# Determine the appropriate non-root user
if [ "${USERNAME}" = "auto" ] || [ "${USERNAME}" = "automatic" ]; then
    USERNAME=""
    POSSIBLE_USERS=("vscode" "node" "codespace" "$(awk -v val=1000 -F ":" '$3==val{print $1}' /etc/passwd)")
    for CURRENT_USER in "${POSSIBLE_USERS[@]}"; do
        if id -u "${CURRENT_USER}" >/dev/null 2>&1; then
            USERNAME=${CURRENT_USER}
            break
        fi
    done
    if [ "${USERNAME}" = "" ]; then
        USERNAME=root
    fi
elif [ "${USERNAME}" = "none" ] || ! id -u "${USERNAME}" >/dev/null 2>&1; then
    USERNAME=root
fi

# Run apt-get if needed.
apt_get_update_if_needed() {
    if [ ! -d "/var/lib/apt/lists" ] || [ "$(find /var/lib/apt/lists/* -prune -print | wc -l)" = "0" ]; then
        _log "info" "Running apt-get update..."
        apt-get update
    else
        _log "info" "Skipping apt-get update."
    fi
}

# Install additional packages needed by vcpkg: https://github.com/microsoft/vcpkg/blob/master/README.md#installing-linux-developer-tools
# Check if packages are installed and installs them if not.

# Debian / Ubuntu packages
install_debian_packages() {

    export DEBIAN_FRONTEND=noninteractive

    local package_list=(build-essential
        curl
        ca-certificates
        zip
        unzip
        procps
        pkg-config
        bash-completion
        ninja-build
        git
        libtool)

    if ! dpkg -s "${package_list[@]}" >/dev/null 2>&1; then
        apt_get_update_if_needed
        apt-get -y install --no-install-recommends "${package_list[@]}"
    fi

    unset DEBIAN_FRONTEND
}

# RedHat / RockyLinux / CentOS / Fedora packages
install_redhat_packages() {
    local install_cmd=dnf
    if ! type dnf >/dev/null 2>&1; then
        install_cmd=yum
    fi

    # Get to latest versions of all packages
    if [ "${UPGRADE_PACKAGES}" = "true" ]; then
        ${install_cmd} upgrade -y
    fi

    local package_list=(centos-release-scl
        ca-certificates
        unzip
        zip
        procps
        sudo
        sed
        grep
        which)

    # rockylinux:9 installs 'curl-minimal' which clashes with 'curl'
    # Install 'curl' for every OS except this rockylinux:9
    if [[ ${ID} == "rocky" ]] && [[ ${VERSION} != *"9."* ]]; then
        package_list+=(curl)
    fi

    # Install lsb_release if available
    if ${install_cmd} -q list redhat-lsb-core >/dev/null 2>&1; then
        package_list+=(redhat-lsb-core)
    fi

    # Install git if not already installed (may be more recent than distro version)
    if ! type git >/dev/null 2>&1; then
        package_list+=(git)
    fi
    ${install_cmd} -y install "${package_list[@]}"
}

# Alpine Linux packages
install_alpine_packages() {
    apk update

    apk add --no-cache \
        openssh-client \
        autoconf \
        build-base \
        binutils \
        curl \
        file \
        gcc \
        g++ \
        libgcc \
        libtool \
        linux-headers \
        ninja \
        gnupg \
        procps \
        net-tools \
        curl \
        ca-certificates \
        unzip \
        zip \
        libstdc++ \
        libssl1.1 \
        lttng-ust \
        tzdata \
        userspace-rcu \
        zlib \
        sudo \
        coreutils \
        sed \
        grep \
        which

    # Install git if not already installed (may be more recent than distro version)
    if ! type git >/dev/null 2>&1; then
        apk add --no-cache git
    fi
}

# ******************
# ** Main section **
# ******************

if [ "$(id -u)" -ne 0 ]; then
    _log "error" 'Script must be run as root. Use sudo, su, or add "USER root" to your Dockerfile before running this script.'
    exit 1
fi

# Load markers to see which steps have already run
if [ -f "${MARKER_FILE}" ]; then
    _log "info" "Marker file found:"
    cat "${MARKER_FILE}"
    source "${MARKER_FILE}"
fi

# Bring in ID, ID_LIKE, VERSION_ID, VERSION_CODENAME
. /etc/os-release
# Get an adjusted ID independant of distro variants
if [ "${ID}" = "debian" ] || [ "${ID_LIKE}" = "debian" ]; then
    ADJUSTED_ID="debian"
elif [[ ${ID} == "rhel" || ${ID} == "fedora" || ${ID} == "mariner" || ${ID_LIKE} == *"rhel"* || ${ID_LIKE} == *"fedora"* || ${ID_LIKE} == *"mariner"* ]]; then
    ADJUSTED_ID="rhel"
elif [ "${ID}" = "alpine" ]; then
    ADJUSTED_ID="alpine"
else
    _log "error" "Linux distro ${ID} not supported."
    exit 1
fi

# Install packages for appropriate OS
if [ "${PACKAGES_ALREADY_INSTALLED}" != "true" ]; then
    case "${ADJUSTED_ID}" in
    "debian")
        install_debian_packages
        ;;
    "rhel")
        install_redhat_packages
        ;;
    "alpine")
        install_alpine_packages
        ;;
    esac
    PACKAGES_ALREADY_INSTALLED="true"
fi

# Setup group and add user
umask 0002
if ! grep </etc/group -e "^vcpkg:" >/dev/null 2>&1; then
    groupadd -r "vcpkg"
fi
usermod -a -G "vcpkg" "${USERNAME}"

# Start Installation
# Clone repository with ports and installer
remove_installation() {
    if [ -d "${VCPKG_ROOT}" ]; then
        _log "warning" "Found a vcpkg distribution folder ${VCPKG_ROOT}. Removing it..."
        rm -rf "${VCPKG_ROOT}" "${VCPKG_DOWNLOADS}"
    fi
    mkdir -p "${VCPKG_ROOT}" "${VCPKG_DOWNLOADS}"
}

clone_args=(--depth=1
    -c core.eol=lf
    -c core.autocrlf=false
    -c safe.directory="${VCPKG_ROOT}"
    -c fsck.zeroPaddedFilemode=ignore
    -c fetch.fsck.zeroPaddedFilemode=ignore
    -c receive.fsck.zeroPaddedFilemode=ignore
    https://github.com/microsoft/vcpkg "${VCPKG_ROOT}")

_log "info" "Preset VCPKG_VERSION: $VCPKG_VERSION"
# Setup vcpkg actual version
if [ "${VCPKG_VERSION}" = "stable" ]; then
    api_info="$(curl -sX GET https://api.github.com/repos/microsoft/vcpkg/releases/latest)"
    vcpkg_stable_version=$(echo "$api_info" | awk '/tag_name/{print $4;exit}' FS='[""]' | sed 's|^v||')
    remove_installation
    git clone -b "$vcpkg_stable_version" "${clone_args[@]}"
    _log "info" "Using VCPKG_VERSION: $VCPKG_VERSION=$vcpkg_stable_version"
elif [ "${VCPKG_VERSION}" = "latest" ]; then
    remove_installation
    git clone "${clone_args[@]}"
    _log "info" "Using VCPKG_VRESION: $VCPKG_VERSION"
else
    tags=$(git ls-remote --tags https://github.com/microsoft/vcpkg | awk '{ print $2 }' | sed -e 's|refs/tags/||g')

    if echo "${tags}" | grep "${VCPKG_VERSION}" >/dev/null 2>&1; then
        remove_installation
        _log "info" "Get valid tag" "${VRESION}"
        git clone -b "${VCPKG_VERSION}" "${clone_args[@]}"
        _log "info" "Using VCPKG_VRESION: $VCPKG_VERSION"
    else
        _log "error" 'Need a valid vcpkg tag to install !!! Please see https://github.com/microsoft/vcpkg/tags.'
        exit 1
    fi
fi
## Run installer to get latest stable vcpkg binary
## https://github.com/microsoft/vcpkg/blob/7e7dad5fe20cdc085731343e0e197a7ae655555b/scripts/bootstrap.sh#L126-L144
VCPKG_FORCE_SYSTEM_BINARIES=1 "${VCPKG_ROOT}"/bootstrap-vcpkg.sh

# Give an access to repos vcpkg for all users
git config --system safe.directory "${VCPKG_ROOT}"

if ! command -v cmake &>/dev/null; then
    # Download CMake using vcpkg automatically
    "${VCPKG_ROOT}"/vcpkg install vcpkg-cmake

    # Install CMake and remove archive cache
    cmake_archive=$(find "$VCPKG_ROOT"/downloads/ -name 'cmake*.tar.gz' | head -n 1)
    tar -xzf "$cmake_archive" -C /usr/local/ --strip-components=1
    rm "$cmake_archive"
fi

# Add to bashrc/zshrc files for all users.
updaterc() {
    _log "info" "Updating /etc/bash.bashrc and /etc/zsh/zshrc..."
    if [[ "$(cat /etc/bash.bashrc)" != *"$1"* ]]; then
        echo -e "$1" >>/etc/bash.bashrc
    fi
    if [ -f "/etc/zsh/zshrc" ] && [[ "$(cat /etc/zsh/zshrc)" != *"$1"* ]]; then
        echo -e "$1" >>/etc/zsh/zshrc
    fi
}

# Add vcpkg to PATH
updaterc "$(
    cat <<EOF
export VCPKG_ROOT="${VCPKG_ROOT}"
if [[ "\${PATH}" != *"\${VCPKG_ROOT}"* ]]; then export PATH="\${VCPKG_ROOT}:\${PATH}"; fi
EOF
)"

# Give read/write permissions to the user group.
chown -R ":vcpkg" "${VCPKG_ROOT}" "${VCPKG_DOWNLOADS}"
chmod g+r+w+s "${VCPKG_ROOT}" "${VCPKG_DOWNLOADS}"
chmod -R g+r+w "${VCPKG_ROOT}" "${VCPKG_DOWNLOADS}"

# Enable tab completion for bash
VCPKG_FORCE_SYSTEM_BINARIES=1 su "${USERNAME}" -c "${VCPKG_ROOT}/vcpkg integrate bash"

# Write marker file
if [ ! -d "$MARKER_FILE" ]; then
    mkdir -p "$(dirname "${MARKER_FILE}")"
fi
echo -e "\
    PACKAGES_ALREADY_INSTALLED=${PACKAGES_ALREADY_INSTALLED}\n\
    LOCALE_ALREADY_SET=${LOCALE_ALREADY_SET}" >"${MARKER_FILE}"

_log "success" "Install vcpkg successfully."
