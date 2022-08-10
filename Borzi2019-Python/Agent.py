#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue Aug  9

@author: aamalhussain
"""

import pygame
import numpy as np


class Agent:

    def __init__(self, position, leader=False):
        self.RADIUS = 5
        self.colour = (40, 185, 190) if not leader else (180, 45, 65)  # BLUE if follower otherwise RED
        self.MAX_SPEED = 5
        self.vel = np.random.uniform(-self.MAX_SPEED, self.MAX_SPEED, 2)
        self.pos = np.array(position, dtype='float64')
        self.is_leader = leader

        self.Ca0 = 0.
        self.Cr0 = 75.
        self.lr0 = 1.
        self.la0 = 50
        self.gamma_1 = 10.

        self.Ca = 20.
        self.Cr = 50.
        self.la = 100.
        self.lr = 2.
        self.sigma = .5
        self.gamma = 1
        self.alpha = .07
        self.beta = .05

        self.delta_1 = np.cos(1.047)
        self.delta_2 = np.cos(1.57)

    def draw(self, target):
        pygame.draw.circle(target, self.colour, self.pos, self.RADIUS)

    def handle_window_collisions(self, width, height):
        if self.pos[0] <= self.RADIUS or self.pos[0] >= width - self.RADIUS:
            self.vel[0] = -self.vel[0]
        if self.pos[1] <= self.RADIUS or self.pos[1] >= height - self.RADIUS:
            self.vel[1] = -self.vel[1]

    def update_pos(self):
        self.pos += self.vel

    def update_vel(self, others):
        acc = self.get_acc(others)
        self.vel += acc

    def handle_max_vel(self):
        if self.vel[0] > self.MAX_SPEED:
            self.vel[0] = self.MAX_SPEED
        if self.vel[1] > self.MAX_SPEED:
            self.vel[1] = self.MAX_SPEED

    def update(self, others, width, height):
        self.update_vel(others)
        self.handle_window_collisions(width, height)
        self.handle_max_vel()
        self.update_pos()

    def get_acc(self, others):
        n = len(others)

        ab = np.zeros(2)
        fb = np.zeros(2)
        f0 = np.zeros(2)

        for o in others:
            if self.is_leader and o.is_leader:
                continue
            r = np.linalg.norm(self.pos - o.pos)
            if not r <= 1e-3:
                z = np.dot((o.pos - self.pos) / np.linalg.norm(o.pos - self.pos),
                           self.vel / np.linalg.norm(self.vel))

                if z < self.delta_2:
                    s = 0
                elif z > self.delta_1:
                    s = 1
                else:
                    s = 0.5 - 0.5 * np.tanh(1 / (z - self.delta_2) + 1 / (z - self.delta_1))

                k = self.gamma / (1 + r**2)**self.sigma * s
                ab += (o.vel - self.vel) * k
                if o.is_leader:
                    f0 += ((self.pos - o.pos) / r) * ((self.Ca0 / self.la0) * np.exp(-r / self.la0)
                                                - (self.Cr0 / self.lr0) * np.exp(-r / self.lr0))
                else:
                    fb += ((self.pos - o.pos) / r) * ((self.Ca / self.la) * np.exp(-r / self.la)
                                                - (self.Cr / self.lr) * np.exp(-r / self.lr))

        fb = -fb / n
        f0 = -f0 / n
        ab = ab / n

        a = self.vel * (self.alpha - self.beta * np.linalg.norm(self.vel)**2) + fb + ab + f0

        return a
