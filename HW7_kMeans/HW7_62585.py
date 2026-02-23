import numpy as np
import matplotlib.pyplot as plt
from sklearn.cluster import KMeans
from sklearn.metrics import silhouette_score

def plot_clusters(X, labels, centers, ax, title):
    ax.scatter(X[:, 0], X[:, 1], c=labels, cmap='viridis', alpha=0.5)
    ax.scatter(centers[:, 0], centers[:, 1], c='red', marker='X', s=200)
    ax.set_title(title)

def kmeans_random_restart(X, n_clusters, n_restarts=10):
    best_labels = None
    best_centers = None
    best_score = -1

    for _ in range(n_restarts):
        kmeans = KMeans(n_clusters=n_clusters, init='random').fit(X)
        labels = kmeans.labels_
        centers = kmeans.cluster_centers_
        score = silhouette_score(X, labels)

        if score > best_score:
            best_score = score
            best_labels = labels
            best_centers = centers

    return best_labels, best_centers, best_score

def main(filename, n_clusters):
    data = np.loadtxt(filename)  # Use np.loadtxt instead of loadtxt
    X = data[:, :2]

    best_labels, best_centers, best_score = kmeans_random_restart(X, n_clusters)

    fig, ax = plt.subplots(1, 1, figsize=(8, 6))
    plot_clusters(X, best_labels, best_centers, ax, f'Best Clusters (Silhouette Score: {best_score:.2f})')
    plt.show()

if __name__ == "__main__":
    # Example for "normal.txt" with 3 clusters
    main("C:\FMI Sofia UNI programming\Inteligent Systems\HOMEWORKS DEIVID\homewrok7_deivid\\normal\\normal.txt", 3)

    # # Example for "unbalance.txt" with 5 clusters
    # main("C:\FMI Sofia UNI programming\Inteligent Systems\HOMEWORKS DEIVID\homewrok7_deivid\unbalance\unbalance.txt", 5)
