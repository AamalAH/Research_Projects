from abc import ABC, abstractmethod
import numpy as np
import torch
import torch.nn as nn
import torch.optim as optim
import torch.nn.functional as F
import plotly.express as px
import plotly.graph_objects as go
from tqdm import tqdm


class single_particle_nn(nn.Module):
    def __init__(self, state_space_dims, hidden_units, rotational_invariance=True) -> None:
        super().__init__()

        self.rot_inv = rotational_invariance
        self._input_units = 3
        self._output_units = state_space_dims

        self.layers = [nn.Linear(self._input_units, hidden_units[0]),
        *[nn.Linear(hidden_units[i], hidden_units[i+1]) for i in range(1, len(hidden_units)-1)],
        nn.Linear(hidden_units[-1], self._output_units)]

        self.init_bias()

        params = [(layer.weight, layer.bias) for layer in self.layers]
        self.parameters = nn.ParameterList([p for param in params for p in param])

    def init_bias(self) -> None:
        for layer in self.layers:
            layer.bias = torch.nn.Parameter(torch.zeros(layer.out_features))

    def rot_symm(self, x: torch.Tensor) -> torch.Tensor:
        """
        We will pass in q and \dot{q} as a 2xd tensor.
        We will return q \cdot q, \dot{q} \cdot \dot{q}, q \cdot \dot{q} as a 3x1 tensor
        """

        out = torch.zeros(3)
        out[0] = torch.dot(x[0], x[0])
        out[1] = torch.dot(x[1], x[1])
        out[2] = torch.dot(x[0], x[1])

        return out

    def forward(self, x: torch.Tensor) -> None:
        if self.rot_inv:
            x = self.rot_symm(x)

        for layer in self.layers:
            x = F.softplus(layer(x))
        
        return x


class dynamical_system(ABC):
    def __init__(self, dim: int) -> None:
        super().__init__()
        self.dim = dim

    @abstractmethod
    def call(self, x: torch.Tensor) -> torch.Tensor:
        """
        Return acceleration
        """


class gravitational_system(dynamical_system):
    def __init__(self, mass: float, alpha: float) -> None:
        super().__init__(dim=2)

        self.mass = mass
        self.alpha = alpha
        
        assert self.mass > 0
        assert self.alpha > 0

    def call(self, x: torch.Tensor) -> torch.Tensor:
        return torch.t( - (self.alpha / self.mass) * torch.t(x) / torch.linalg.norm(x, dim=1)**3)


def generate_data(state_space_dims: int, n_data: int) -> torch.Tensor:
    """
    data is composed of n_data samples of q, \dot{q} and corresponding \ddot{q}
    as the target
    """

    q = torch.FloatTensor(n_data, state_space_dims).uniform_(-10, 10)
    q_dot = torch.FloatTensor(n_data, state_space_dims).uniform_(-5, 5)

    system = gravitational_system(mass=1, alpha=0.5)
    acc = system.call(q)

    return torch.cat((q, q_dot, acc), dim=1)


def generate_test_train_split(data:torch.Tensor, train: float, dev: float) -> tuple:
    assert 0 < train < 1
    assert 0 < dev < 1

    n_data = data.size(dim=0)

    train_end = int(n_data * train)
    dev_end = int(n_data * (train + dev))

    return data[:train_end], data[train_end:dev_end], data[dev_end:]

if __name__ == "__main__":
    state_space_dims = 2
    
    hidden_units = [128, 128]
    model = single_particle_nn(state_space_dims, hidden_units)
    optimiser = optim.Adam(model.parameters(), lr=0.01)
    loss_func = nn.MSELoss()
    n_data = int(1e4)
    n_epochs = 2500
    batch_size = 120

    train_losses = np.zeros(n_epochs)
    dev_losses = np.zeros(n_epochs)

    data = generate_data(state_space_dims, n_data)
    data_train, data_dev, data_test = generate_test_train_split(data, 0.6, 0.2)

    for e in tqdm(range(n_epochs)):
        indices = torch.randperm(data_train.size(dim=0))
        x_shuffle, y_shuffle = data_train[indices, 0:state_space_dims*2], data_train[indices, state_space_dims*2:]

        num_iter = 0
        e_train_loss = 0

        for c_batch in range(0, x_shuffle.size(dim=0), batch_size):
            x_batch = x_shuffle[c_batch:c_batch + batch_size]
            y_batch = y_shuffle[c_batch:c_batch + batch_size]

            y_pred = model(x_batch)
            loss = loss_func(y_pred, y_batch)
            optimiser.zero_grad()
            loss.backward()
            optimiser.step()

            e_train_loss += loss.item()

            num_iter += 1

        train_losses[e] = e_train_loss / num_iter

        x_dev, y_dev = data_dev[:, 0:state_space_dims*2], data_dev[:, state_space_dims*2:]

        with torch.no_grad():
            model.eval()
            y_pred = model(x_dev)
            loss = loss_func(y_pred, y_dev)

            e_dev_loss = loss.item()

        dev_losses[e] = e_dev_loss

    fig = px.line(x=list(range(0, n_epochs)), y=train_losses)
    fig.add_trace(go.Scatter(x=list(range(0, n_epochs)), y=dev_losses, mode='lines',))
    fig.show()