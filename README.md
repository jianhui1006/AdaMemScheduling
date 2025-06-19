# AdaMemScheduling
Generalizable and Scalable Memory-Aware Scheduling: the Power of Adaptivity Methods
## Code

### Code List
Note that some parameters should be modified befor using. Input/output path is always needed.

**scheduler.cpp**

Main scheduler methods in the paper, each method corresponds to a namespace. The cost of the topological order can be evaluated by namespace Topsort. Related to all Experiments.

**dm-tree.cpp**

Randomly generate a tree-based dag, related to tree-based datasets.

**dm-scav.cpp**

Generator node-deleted graphs for scalability test, related to Exp-9.

**dm-scae.cpp**

Generator edge-deleted graphs for scalability test, related to Exp-9.

**dp.cpp**

State compression dynamic programming. Use to calculate the optimal solution on small graphs. Related to Exp-1.

### Usage
Step1: Modify the parameters in code, including input/output path, threshold and so on.

Step2: Compile and run.

```
g++ code.cpp -o code -std=c++17 -O2
./code
```

## Data

Because some datasets used in the paper are too large to be uploaded to GitHub, we have prepared a cloud storage for storing most of the data. However, for Exp-8 and Exp-9, the data is still too large and the cloud storage space is insufficient, and we are unable to upload all the data. A more reasonable approach is to use the generator in the code to generate it. The download links are as follows:

[Google Drive](https://drive.google.com/drive/folders/1kJrWa_BLNu74S1LfSr7D56zn5i0lLAbB?usp=sharing)

[OneDrive](https://1drv.ms/f/c/0c6141973d086fdc/EkzTBNuH6PBIqUS76wRjP8sB-jS0J131Jymcx4Ht-_SRbQ?e=fHvdLq)
