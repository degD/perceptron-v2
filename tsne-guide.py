import plotly.express as px
from sklearn.datasets import make_classification
from sklearn.manifold import TSNE
from sklearn.decomposition import PCA


X, y = make_classification(
    n_features=6,
    n_classes=3,
    n_samples=1500,
    n_informative=2,
    random_state=5,
    n_clusters_per_class=1,
)
# For comparison with our model, this dataset has
# 6 parameters (features), and 3 classes (categories).

# (1500, 6) (1500, )
# print(X.shape, y.shape)

# Show the dataset in 3D
# Plotting only the first 3 parameters
# fig = px.scatter_3d(x=X[:, 0], y=X[:, 1], z=X[:, 2], color=y, opacity=0.8)
# fig.show()

# This plots data with PCA transformation
pca = PCA(n_components=2)
X_pca = pca.fit_transform(X)
fig = px.scatter(x=X_pca[:, 0], y=X_pca[:, 1], color=y)
fig.update_layout(
    title="PCA visualization of Custom Classification dataset",
    xaxis_title="First Principal Component",
    yaxis_title="Second Principal Component",
)
fig.show()

# Transforming to t-SNE
tsne = TSNE(n_components=2, random_state=42)
X_tsne = tsne.fit_transform(X)
print(tsne.kl_divergence_) # Low KL is a sign of better results

# Plotting t-SNE
fig = px.scatter(x=X_tsne[:, 0], y=X_tsne[:, 1], color=y)
fig.update_layout(
    title="t-SNE visualization of Custom Classification dataset",
    xaxis_title="First t-SNE",
    yaxis_title="Second t-SNE",
)
fig.show()