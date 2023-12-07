import plotly.express as px
from sklearn.manifold import TSNE
from sklearn.decomposition import PCA
import numpy as np
import os



parameters_list = list()
parameter_indices = list()

for n, param_init in enumerate(["0.01", "0.001", "-0.03", "0", "-0.0009"]):
    
    # Compile and run training program with SGD
    os.system(f"gcc 3-train.c -lm -o 3.out; echo 1 n {param_init} | ./3.out;")
    
    # read parameters.log
    parameters_progress_list = list()

    with open("parameters.log") as log_file:
        log_lines_list = log_file.readlines()
        for i, line in enumerate(log_lines_list):
            parameters_progress_list.append(list())
            parameter_indices.append(n)
            for parameter in line.strip().split()[1:]:
                parameters_progress_list[i].append(parameter)
                
    parameters_list += parameters_progress_list


X = np.array(parameters_list)
y = np.array(parameter_indices)


# Apply PCA to reduce dimensions to 50
pca = PCA(n_components=2)
X_pca = pca.fit_transform(X)


# TODO: Find perplexity that gives the minimal KL value


# Apply t-SNE and reduce dimensions to 2
tsne = TSNE(n_components=2, random_state=25)
X_tsne = tsne.fit_transform(X_pca)


# Plot t-SNE scatter plot
fig = px.scatter(x=X_tsne[:, 0], y=X_tsne[:, 1], color=y)
fig.update_layout(
    title="t-SNE Visualization of Model Training Process",
    xaxis_title="First t-SNE",
    yaxis_title="Second t-SNE",
)
fig.show()