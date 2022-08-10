#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue Aug  9

@author: aamalhussain
"""

import pygame
from Agent import Agent
import numpy as np


class Environment:

    def __init__(self):
        self.WIDTH = 900
        self.HEIGHT = 500
        self.FPS = 60
        self.WIN = pygame.display.set_mode([self.WIDTH, self.HEIGHT])
        pygame.display.set_caption("Borzi - 2019")
        self.clock = pygame.time.Clock()
        self.is_running = True
        self.screen_color = (0, 0, 0)  # BLACK

        self.num_agents = 10
        self.num_leaders = 2
        self.agents = [Agent((np.random.randint(0, self.WIDTH),
                              np.random.randint(0, self.HEIGHT)), leader=True) for cA in range(self.num_leaders)]\
                      + [Agent((np.random.randint(0, self.WIDTH),
                              np.random.randint(0, self.HEIGHT))) for cA in range(self.num_agents)]

    def poll_event(self):
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                self.is_running = False

    def draw(self):
        self.WIN.fill(self.screen_color)

        for agent in self.agents:
            agent.draw(self.WIN)

    def update(self):
        self.clock.tick(self.FPS)
        self.poll_event()
        for agent in self.agents:
            agent.update(self.agents, self.WIDTH, self.HEIGHT)

        pygame.display.update()

