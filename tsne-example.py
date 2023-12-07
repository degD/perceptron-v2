import plotly.express as px
from sklearn.datasets import make_classification
from sklearn.manifold import TSNE
from sklearn.decomposition import PCA
import numpy as np


X, y = make_classification(
    n_features=1000,
    n_classes=2,
    n_samples=1500,
    n_informative=2,
    random_state=5,
    n_clusters_per_class=1,
)


pca = PCA(n_components=50)
X_pca = pca.fit_transform(X)

# perplexity = np.arange(200, 1000, 50)
# divergence = []

# for i in perplexity:
#     model = TSNE(n_components=2, init="pca", perplexity=i)
#     reduced = model.fit_transform(X_pca)
#     divergence.append(model.kl_divergence_)
# fig = px.line(x=perplexity, y=divergence, markers=True)
# fig.update_layout(xaxis_title="Perplexity Values", yaxis_title="Divergence")
# fig.update_traces(line_color="red", line_width=1)
# fig.show()


tsne = TSNE(n_components=2, random_state=42, perplexity=1300)
X_tsne = tsne.fit_transform(X_pca)


fig = px.scatter(x=X_tsne[:, 0], y=X_tsne[:, 1], color=y)
fig.update_layout(
    title="t-SNE visualization of Custom Classification dataset",
    xaxis_title="First t-SNE",
    yaxis_title="Second t-SNE",
)
fig.show()

print(tsne.kl_divergence_)