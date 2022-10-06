import numpy as np
import matplotlib.pyplot as plt
from scipy.integrate import odeint


def plotOnSimplex(traj):

    f = plt.figure()
    ax = f.add_subplot(1, 1, 1)
    plt.axis('off')
    proj = np.array(
        [[-1 * np.cos(30 / 360 * 2 * np.pi), np.cos(30 / 360 * 2 * np.pi), 0],
         [-1 * np.sin(30 / 360 * 2 * np.pi), -1 * np.sin(30 / 360 * 2 * np.pi), 1]
         ])

    ts = np.linspace(0, 1, 10000)

    e1 = proj @ np.array([ts, 1 - ts, 0 * ts])
    e2 = proj @ np.array([0 * ts, ts, 1 - ts])
    e3 = proj @ np.array([ts, 0 * ts, 1 - ts])

    ax.plot(e1[0], e1[1], 'k-', alpha = 0.3)
    ax.plot(e2[0], e2[1], 'k-', alpha = 0.3)
    ax.plot(e3[0], e3[1], 'k-', alpha = 0.3)

    for cI in range(nInit):
        x = proj @ traj[cI]
        ax.plot(x[0], x[1], '--', alpha=0.7)


def replicator(x, t, A):
    dx = np.zeros(3)
    
    # dx = x * (A@x - np.dot(x, A@x))
    c1 = lambda p: 0.5 * (1 + p)**2
    c2 = lambda p: (1 + p)**2
    c3 = lambda p: 2 * (1 + p)**2

    c = np.array([c1(x[0] + x[2]), c2(x[0] + x[1]), c3(x[1] + x[2])])
    loss = np.array([c[0] + c[1], c[1] + c[2], c[0] + c[2]])

    dx = x * (loss - np.dot(loss, x))

    return dx


def make_gif():
    with imageio.get_writer('satoGif.gif', mode='I') as writer:
        for t in range(gifLength):
            image = imageio.imread('Sato/Sato_{0}.png'.format(t))
            writer.append_data(image)


if __name__ == "__main__":
    ex = 0
    nInit = 20

    A = np.array([[ex, 1, -1], [-1, ex, 1], [1, -1, ex]])

    t = np.linspace(0, int(1e3), int(1e4) + 1)

    allSol = np.zeros((nInit, 3, int(1e4) + 1))
    
    for cI in range(nInit):
        x0 = np.random.dirichlet(np.ones(3))
        sol = odeint(replicator, x0, t, args=tuple([A]))
        allSol[cI] = sol.T

    plotOnSimplex(allSol)
    plt.show()