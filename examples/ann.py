import numpy as np
import json

## VARS ##
INPUT_LAYER_SIZE = 21
TRAINING_SET_COUNT = 100
ENABLE_BIAS = True
BATCHES = 1000
TESTING_SET_COUNT = 10000
LEARNING_RATE = 0.3

class NeuralNetwork:
    def __init__(self, input_size, training_set_size, testing_set_size, enable_bias=True, batches=5000, learning_rate=0.3):
        self.input_size = input_size
        self.output_size = 1
        self.training_set_size = training_set_size
        self.testing_set_size = testing_set_size
        self.enable_bias = enable_bias
        self.batches = batches
        self.learning_rate = learning_rate

        self.weights = self.init_random_weights()
        self.training_set_inputs, self.training_set_outputs = self.init_set(self.training_set_size)
        self.testing_set_inputs, self.testing_set_outputs = self.init_set(self.testing_set_size)


    def init_random_weights(self):
        'Initialize weights randomly between -1 and 1'
        size = self.input_size+1 if self.enable_bias else self.input_size
        weights = 2*np.random.random((size, 1)) - 1
        return weights

    def init_set(self, size):
        '''
            creates size inputs, each one is a self.input_size digits long number of zeros and ones.
            Also creates size outputs, using solutions from most_frequent_element
        '''
        # input layer
        inputs = np.array([np.random.choice([0, 1], size=self.input_size) for _ in range(size)])

        if self.enable_bias:  # adding the bias input (with value 1) to each example.
            inputs = np.insert(inputs, self.input_size, 1, axis=1)

        # output layer
        outputs = np.array([[self.most_frequent_element(x) for x in inputs]]).T

        return inputs, outputs

    def train(self):
        'Training the neural network.'
        for i in range(self.batches):
            # forward propogation
            l0 = self.training_set_inputs
            l1 = self.forward(l0)

            # error calc
            l1_error = self.training_set_outputs - l1
            if (i % 100) == 0:
                print("[Batch {}]\tError (training set): {:.4f}\tError (testing set): {:.4f}".format(i, np.mean(np.abs(l1_error)), self.test()))

            # delta between error and correct answers
            l1_delta = l1_error * self.sigmoid_deriv(l1)

            # updating weights
            self.weights += self.learning_rate * np.dot(l0.T, l1_delta)

    def test(self):
        'Testing the network. Returning the mean error value.'
        l0 = self.testing_set_inputs
        l1 = self.sigmoid(np.dot(l0, self.weights))
        l1_error = self.testing_set_outputs - l1
        return np.mean(np.abs(l1_error))
    
    def forward(self, input_):
        'Running the input through the network and returning the result'
        return self.sigmoid(np.dot(input_, self.weights))

    @staticmethod
    def sigmoid(x):
        return 1/(1+np.exp(-x))

    @staticmethod
    def sigmoid_deriv(x):
        return x*(1-x)

    @staticmethod
    def most_frequent_element(arr):
        values, counts = np.unique(arr, return_counts=True)
        return values[np.argmax(counts)]

def main():
    print("Initializing Neural Network...")
    NN = NeuralNetwork(INPUT_LAYER_SIZE, TRAINING_SET_COUNT, TESTING_SET_COUNT, ENABLE_BIAS, BATCHES, LEARNING_RATE)

    print("Network is starting to train (against {} examples).".format(TRAINING_SET_COUNT))
    NN.train()
    print("Network has finished training.")

    print("Testing against a testing set (of {} examples)...".format(TESTING_SET_COUNT))
    error_rate = NN.test()
    print("Error (against testing set): {:.4f}".format(error_rate))

    # UI for forwarding queries through the neutral network
    print("\r\nDone.")
	
	
    with open('weights.json', 'w') as f:
        json.dump(NN.weights.tolist(), f)
    

if __name__ == "__main__":
    main()
	
