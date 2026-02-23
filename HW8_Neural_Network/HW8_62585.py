import numpy as np

def sigmoid(x):
    return 1 / (1 + np.exp(-x))

def sigmoid_derivative(x):
    return x * (1 - x)

class NeuralNetwork:
    def __init__(self, input_size, hidden_size, output_size):
        self.input_size = input_size
        self.hidden_size = hidden_size
        self.output_size = output_size

        # Инициализация на теглата със случайни стойности
        self.weights_input_hidden = np.random.uniform(-0.05, 0.05, (input_size, hidden_size))
        self.weights_hidden_output = np.random.uniform(-0.05, 0.05, (hidden_size, output_size))

    def train(self, inputs, targets, learning_rate=0.1, epochs=10000):#eproach means how many times we will train the network
        for epr in range(epochs):
            # Forward pass
            hidden_inputs = np.dot(inputs, self.weights_input_hidden)#
            hidden_outputs = sigmoid(hidden_inputs)

            final_inputs = np.dot(hidden_outputs, self.weights_hidden_output)
            final_outputs = sigmoid(final_inputs)

            # Calculate error
            output_errors = targets - final_outputs

            # Backpropagation
            output_delta = output_errors * sigmoid_derivative(final_outputs)
            hidden_errors = output_delta.dot(self.weights_hidden_output.T)
            hidden_delta = hidden_errors * sigmoid_derivative(hidden_outputs)

            # Update weights
            self.weights_hidden_output += learning_rate * hidden_outputs.T.dot(output_delta)
            self.weights_input_hidden += learning_rate * inputs.T.dot(hidden_delta)

    def predict(self, inputs):
        hidden_inputs = np.dot(inputs, self.weights_input_hidden)
        hidden_outputs = sigmoid(hidden_inputs)

        final_inputs = np.dot(hidden_outputs, self.weights_hidden_output)
        final_outputs = sigmoid(final_inputs)

        return final_outputs

# Пример за AND функция
and_inputs = np.array([[0, 0], [0, 1], [1, 0], [1, 1]])
and_targets = np.array([[0], [0], [0], [1]])

and_nn = NeuralNetwork(input_size=2, hidden_size=4, output_size=1)
and_nn.train(and_inputs, and_targets)

print("AND Predictions:")
print(and_nn.predict(and_inputs))

# Пример за OR функция
or_inputs = np.array([[0, 0], [0, 1], [1, 0], [1, 1]])
or_targets = np.array([[0], [1], [1], [1]])

or_nn = NeuralNetwork(input_size=2, hidden_size=4, output_size=1)
or_nn.train(or_inputs, or_targets)

print("\nOR Predictions:")
print(or_nn.predict(or_inputs))

# Пример за XOR функция
xor_inputs = np.array([[0, 0], [0, 1], [1, 0], [1, 1]])
xor_targets = np.array([[0], [1], [1], [0]])

xor_nn = NeuralNetwork(input_size=2, hidden_size=4, output_size=1)
xor_nn.train(xor_inputs, xor_targets)

print("\nXOR Predictions:")
print(xor_nn.predict(xor_inputs))
