#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue Aug  9

@author: aamalhussain
"""

import pygame
from Environment import Environment


def main():

    env = Environment()

    while env.is_running:
        env.update()

        env.draw()

    pygame.quit()

if __name__ == "__main__":
    main()