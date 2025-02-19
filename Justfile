default:
    just --list

build:
    mkdir -p _bin
    clang -g -o _bin/forge -D_CRT_SECURE_NO_WARNINGS src/forge.c src/file_ops.c

run: build
    ./_bin/forge etc/hello.f

clean:
    rm -rf _bin

alias b := build
alias r := run
alias c := clean
