# C++ Utilities

This repository contains several miscellaneous utility modules and applications.

## [`args`: A command-line argument parser](args) ![Status: ready for use](https://img.shields.io/badge/Status-Ready_for_use-green?style=flat&labelColor=%232c3e50&color=%2327ae60)

Are you tired of parsing command-line arguments?
This module makes it dead simple.

## [`autotag`: An ID3v2 wrapper](autotag) ![Status: in progress](https://img.shields.io/badge/Status-In_progress-yellow?style=flat&labelColor=%232c3e50&color=%23f39c12)

This makes managing the ID3v2 tags of your media library easy.
Easily set the artist, album art, track title, and much more.

## [`bmp`: A bitmap generator](bmp) ![Status: ready for use](https://img.shields.io/badge/Status-Ready_for_use-green?style=flat&labelColor=%232c3e50&color=%2327ae60)

Create bitmaps with ease!
Have you ever wanted to generate an image from a C++ program?
This library keeps things simple.

## [`cli`: A simple command-line interface](cli) ![Status: in progress](https://img.shields.io/badge/Status-In_progress-yellow?style=flat&labelColor=%232c3e50&color=%23f39c12)

Want to implement a simple shell-like command-line?
Don't want to use GNU's `readline` library?
This module is straightforward and powerful.

## [`fff`: Find my freaking file!](fff) ![Status: in progress](https://img.shields.io/badge/Status-In_progress-yellow?style=flat&labelColor=%232c3e50&color=%23f39c12)

This utility finds files or directories with names that match the given pattern.
Compare it to GNU's `find`.
The search is done in a breadth-first manner.

## [`pcre`: A PCRE2-compatible regular expression engine](pcre) ![Status: in progress](https://img.shields.io/badge/Status-In_progress-yellow?style=flat&labelColor=%232c3e50&color=%23f39c12)

The C++ STL's regular expression engine is the worst.
It's slow. It's cumbersome. It doesn't have all the features I want.
I want to implement a fully-featured, efficient, robust regular expression library.

## [`testing`: A simple unit test suite](testing) ![Status: ready for use](https://img.shields.io/badge/Status-Ready_for_use-green?style=flat&labelColor=%232c3e50&color=%2327ae60)

Existing unit testing suites can be a hassle to set up,
especially when portability is a concern.
This library makes it easy:
include this single module in your project, and it just works.

## [`thread_queue`: A multi-threaded job queue](thread_queue) ![Status: in progress](https://img.shields.io/badge/Status-In_progress-yellow?style=flat&labelColor=%232c3e50&color=%23f39c12)

Do you have a lot of jobs that need to run?
Running jobs in parallel will make them take less time,
but if you have a thousand jobs to run, you can't just start a new thread for each one.
This thread queue is the solution:
the jobs will be started in the order they were added to the queue,
running in parallel, with a healthy cap on the number of concurrent processes.
