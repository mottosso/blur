import os
import sys
import argparse

from .app import show

parser = argparse.ArgumentParser()
parser.add_argument("executable", help="blur executable")
parser.add_argument("image", help="Source image to blur")

args = parser.parse_args()

executable = os.path.abspath(args.executable)
image = os.path.abspath(args.image)


if not os.path.exists(image):
    sys.stderr.write("The file \"%s\" does not exist.\n" % image)
    exit(1)

show(executable, image)
