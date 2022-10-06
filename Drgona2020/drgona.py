# %% 
import numpy as np
import torch
import torch.nn as nn
import torch.optim as optim
import torch.nn.functional as F
import plotly.express as px
import plotly.graph_objects as go


class dissipative_nn(nn.Module):
    def __init__(self, input_units, hidden_units, output_units) -> None:
        super().__init__()

        self.layers = [nn.Linear(input_units, hidden_units[0]), 
        *[nn.Linear(hidden_units[i], hidden_units[i+1]) for i in range(len(hidden_units) - 1)],
        nn.Linear(hidden_units[-1], output_units)]

        self.set_weights()

    def set_weights(self) -> None:
        l_min = 0.01
        l_max = 0.02

        for layer in self.layers:
            m = layer.out_features
            n = layer.in_features
            s = 0.8 / (layer.in_features * layer.out_features)

            W = generate_weight_matrix(m, n, l_max, l_min, s)
            layer.weight = nn.Parameter(torch.Tensor(W), requires_grad=False)
            layer.bias = nn.Parameter(torch.zeros(m), requires_grad=False)

        # self.layers[-1].bias = nn.Parameter(torch.randn(2), requires_grad=False)

    def forward(self, x):
        for layer in self.layers[:-1]:
            x = F.relu(layer(x))
        x = self.layers[-1](x)
        return x


def generate_weight_matrix(m, n, l_max, l_min, s=0.8):

    if m == n:
        S = np.eye(n) * (l_max - (l_max - l_min) * s)
    if m > n:
        S = np.vstack((np.eye(n) * (l_max - (l_max - l_min) * s), np.zeros((m-n, n))))
    if n > m:
        S = np.hstack((np.eye(m) * (l_max - (l_max - l_min) * s), np.zeros((m, n-m))))


    us = [np.random.randn(m, 1) for i in range(m)]
    for i in range(1, m):
        for u in us[:i]:
            us[i] = us[i] - u @ u.T @ us[i] / np.linalg.norm(u)**2
    
    U = np.hstack(us)

    vs = [np.random.randn(n, 1) for i in range(n)]
    for i in range(1, n):
        for v in vs[:i]:
            vs[i] = vs[i] - v @ v.T @ vs[i] / np.linalg.norm(v)**2

    V = np.hstack(vs)

    return U.T @ S @ V


def create_matrix_gershgorin(n, centre, rad):
    M = np.random.uniform(0, 1, size=(n, n))
    M -= np.diag(np.diag(M))
    S = np.sum(M, axis=1)

    A = np.diag(rad / S) @ M + np.eye(n) * centre

    return A


if __name__ == '__main__':
    state_space_dims = 2
    hidden_units = [5, 10, 15, 20, 15, 10, 5]
    dnn = dissipative_nn(state_space_dims, hidden_units, state_space_dims)
    n_iter = 100
    n_init = 10

    A = torch.Tensor(create_matrix_gershgorin(state_space_dims, 0.75, 0.25))
    print(np.linalg.eig(A)[0])

    g = lambda x: A @ x

    all_traj = np.zeros((n_init, n_iter, state_space_dims))
    for c_init in range(n_init):
        x = torch.Tensor(np.random.randn(state_space_dims))
        for c_iter in range(n_iter):
            all_traj[c_init, c_iter] = x
            x = g(x) + dnn(x)

    fig = px.line(x=all_traj[0, :, 0], y=all_traj[0, :, 1])
    fig.add_trace(go.Scatter(x=[all_traj[0, -1, 0]], y=[all_traj[0, -1, 1]]))

    for c_init in range(1, n_init):
        fig.add_trace(go.Scatter(x=all_traj[c_init, :, 0], y=all_traj[c_init, :, 1], mode="lines"))
        fig.add_trace(go.Scatter(x=[all_traj[c_init, -1, 0]], y=[all_traj[c_init, -1, 1]], fillcolor='black'))

    fig.show()

    print(all_traj[:, -1, 0])
# %%
