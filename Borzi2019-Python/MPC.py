import numpy as np


class MPC:

    def __init__(self, init_cond, des_state):
        self.x = init_cond
        self.des = des_state


    def grad(u):
        pass


    def line_search(a0, d):
        


    def conj_direction(g_1, g, d_k):
        y_k = g_1 - g
        sigma_k = y_k - 2 * d_k * np.hstack((y_k, y_k)) / np.hstack((y_k, d_k))
        beta_k = np.hstack((sigma_k, g_1)) / np.hstack((d_k, y_k))

        return beta_k


    def ncg(u, d0, max_k, tol):
        d = d0
        g = -d
        k = 0
        while ( k < max_k and np.linalg.norm(g) > tol ):
            a = self.line_search(a0, d)
            u += a * d
            g_1 = self.grad(u)
            beta = self.conj_direction(g_1, g, d)
            k += 1