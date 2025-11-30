import random

def generate_needle_graph(n, filename):
    edges = []
    
    # 1. Create the "Needle": Vertex 0 has degree 2
    # It connects to Vertex 1 and Vertex 2
    edges.append((0, 1))
    edges.append((0, 2))
    
    # 2. Create the "Haystack": A dense web for the rest
    # We make sure vertices 1 to n-1 are highly connected
    # so the min-cut is definitely NOT among them.
    for i in range(1, n):
        for j in range(i + 1, n):
            # Add edge with 50% probability (Dense random graph)
            if random.random() < 0.5:
                edges.append((i, j))
                
    # 3. Ensure connectivity (Backbone) to prevent accidental disconnections
    for i in range(1, n - 1):
        edges.append((i, i+1))

    with open(filename, 'w') as f:
        f.write(f"{n}\n")
        f.write(f"{len(edges)}\n")
        for u, v in edges:
            f.write(f"{u} {v}\n")

    print(f"Generated '{filename}' with {n} vertices and {len(edges)} edges.")
    print(f"The 'Needle' (Min Cut) is isolating Vertex 0.")
    print(f"Correct Min Cut is: 2")

# Generate the dataset
generate_needle_graph(50, "n50_needle.txt")