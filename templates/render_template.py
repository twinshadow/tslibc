#!/usr/bin/env python
from sys import argv, stdin

def get_args(args):
    out = dict()
    for arg in args:
        k, v = arg.split("=", 1)
        out[k] = v
    return out


def render(args, input, render_begin="${", render_end="}"):
    for k,v in args.items():
        k = render_begin + k + render_end
        input = input.replace(k, v)
    return input


def main(args):
    input = stdin.read()
    args = get_args(args)
    out = render(args, input)
    print out


if __name__ == "__main__":
    main(argv[1:])
