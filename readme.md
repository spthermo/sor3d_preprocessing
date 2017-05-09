# SOR3D Data Preparation

## Description
This script (C++) receives depthmaps or RGB as input and performs:
1. hand-object separation
2. depth colorization (HOT_COLORMAP)
3. output:
..* hand (RGB, depthmap)
..* object (RGB, depthmap)
..* scene (RGB, depthmap)

## OpenCV
OpenCV 2.7.1 is required

## Usage
Run ```mapper.o``` with two input arguments:
1. Input data path (e.g. ```/home/username/Documents/sor3d_train_set```)
2. Output data path (e.g. ```/home/username/Desktop/after_preprocessing/train_set```)

For example:

```
./mapper /home/spiros/Documents/sor3d_train_set /home/spiros/Documents/processed/train_set
```

## Publication
This script is used for SOR3D preprocessing. The output data are used in the following paper:

S. Thermos _et_ _al_, "_Deep Affordance-based Sensorimotor Recognition_", to appear CVPR 2017, [pdf](https://arxiv.org/abs/1704.02787)
