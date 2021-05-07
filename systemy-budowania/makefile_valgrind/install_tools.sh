#!/bin/bash

# Skrypt instaluje potrzebne 3rd part toole do naszego projektu
# Obecnie skrypt wspiera tylko debiana i podobne mu systemu jak ubuntu, mint, kali

function is_debian_based()
{
    # Local oznacza to co kiedys auto w C, czyli ze zmienna ma scope tylko w funkcji
    local debian_distro=("debian" "ubuntu" "mint" "kali") # te distro wspieramy
    local distro=`echo "$1" | tr '[:upper:]' '[:lower:]'` # Ubuntu -> ubuntu
    local is_debian=0

    local len=${#debian_distro[@]}
    # Wyszukiwanie liniowe
    for ((i = 0; i < ${len}; ++i)); do
        if [ "${distro}" == "${debian_distro[i]}" ]; then
            is_debian=1
        fi
    done

    # To taki return, bo bedziemy wolac funkcje z ``
    echo $is_debian
}

# jesli jest plik /etc/os-release to source pliku i czytaj zmienna
if [ -f /etc/os-release ]; then
    . /etc/os-release
    distr=$ID_LIKE
    if [ -z "$distro" ]; then
        distro=$NAME
    fi
fi
echo "ELOSSS"
is_debian=`is_debian_based $distro`
if [ $is_debian -eq 1 ]; then
    if ! dpkg -l | grep -qw "valgrind"; then
        sudo apt --yes install valgrind || exit 1
    else
        echo "Masz juz valgrinda"
    fi
else
    echo "Nie wspieramy Twojego systemu, zainstaluj pakiety recznie :("
fi