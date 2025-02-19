default:
    just --list

build:
    mkdir -p _bin
    clang -g -o _bin/forge src/forge.c

run: build
    ./_bin/forge

clean:
    rm -rf _bin

alias b := build
alias r := run
alias c := clean
