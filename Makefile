# ----------------------------
# Makefile Options
# ----------------------------

NAME = Chip8-CE
ICON = icon.png
DESCRIPTION = "Chip8 on Ti-84+ CE"
COMPRESSED = YES
ARCHIVED = YES

CFLAGS = -Wall -Wextra -Oz
CXXFLAGS = -Wall -Wextra -Oz

# ----------------------------

include $(shell cedev-config --makefile)
