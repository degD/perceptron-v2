
import os
import numpy as np
import matplotlib.pyplot as plt

fig, axs = plt.subplots(3, 2, layout="constrained", sharey=True)

for i in range(3):
    
    os.system(f"gcc 3-train.c -lm -o ./3.out; echo {i} | ./3.out")
    
    with open("training.log") as tlog:
        
        # Acquiring data
        data = tlog.readlines()
        epoch_range = len(data) - 1
        time_range = data[-1].split()
        loss_data = np.array([float(line.split()[1]) for line in data[:-1]])
    
    # Epoch vs loss
    axs[i][0].plot(np.arange(epoch_range), loss_data)

    if i == 0: 
        axs[i][0].set_title("GD - epoch vs loss")
        axs[i][0].set_ylabel("loss")
        axs[i][0].set_xlabel("epoch")
    if i == 1: 
        axs[i][0].set_title("SGD - epoch vs loss")
        axs[i][0].set_ylabel("loss")
        axs[i][0].set_xlabel("epoch")
    if i == 2: 
        axs[i][0].set_title("ADAM - epoch vs loss")
        axs[i][0].set_ylabel("loss")
        axs[i][0].set_xlabel("epoch")
    
    # Time vs loss
    axs[i][1].plot(np.linspace(0, float(time_range[0]), len(loss_data)), loss_data)

    if i == 0: 
        axs[i][1].set_title("GD - time (ms) vs loss")
        axs[i][1].set_ylabel("loss")
        axs[i][1].set_xlabel("time")
    if i == 1: 
        axs[i][1].set_title("SGD - time (ms) vs loss")
        axs[i][1].set_ylabel("loss")
        axs[i][1].set_xlabel("time")
    if i == 2: 
        axs[i][1].set_title("ADAM - time (ms) vs loss")
        axs[i][1].set_ylabel("loss")
        axs[i][1].set_xlabel("time")

plt.show()