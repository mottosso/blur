import os
import sys
import argparse

from .app import show

parser = argparse.ArgumentParser()
parser.add_argument("executable", help="blur executable")
parser.add_argument("image", help="Source image to blur")
parser.add_argument("--kernel-size", default=9)
parser.add_argument("--radius", default=4)

args = parser.parse_args()

executable = os.path.abspath(args.executable)
image = os.path.abspath(args.image)
kernelSize = args.kernel_size
radius = args.radius


if not os.path.exists(image):
    sys.stderr.write("The file \"%s\" does not exist.\n" % image)
    exit(1)

show(executable, image, kernelSize, radius)
