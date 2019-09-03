import csv
import matplotlib.pyplot as plt
import numpy as np

def get_k(xs, trendpoly):
    k = (trendpoly(xs)[10] - trendpoly(xs)[0]) / (xs[10] - xs[0])
    return k

def create_wheel_plot(path_to_dataset, path_to_saving_plot, line):
    xs = []
    ys = []

    with open(path_to_dataset, 'r') as f:
        reader = csv.reader(f)

        for row in reader:
            xs += [float(row[0])]
            ys += [float(row[1])]
    plt.xticks([i for i in range(0, 270, 15)])
    plt.yticks([i for i in range(0, 50, 2)])
    plt.plot(xs, ys, line)
    trend = np.polyfit(xs, ys, 1)
    trendpoly = np.poly1d(trend)
    plt.plot(xs, trendpoly(xs))
    k = get_k(xs, trendpoly)
    plt.grid()
    plt.savefig(path_to_saving_plot)
    plt.cla()

    return k

path_to_dataset_l = 'datasets/dataset_l.csv'
path_to_dataset_r = 'datasets/dataset_r.csv'
path_to_saving_left_plot = 'datasets/left_wheel.png'
path_to_saving_right_plot = 'datasets/right_wheel.png'

k_l = create_wheel_plot(path_to_dataset_l, path_to_saving_left_plot, 'r--')
k_r = create_wheel_plot(path_to_dataset_r, path_to_saving_right_plot, 'g.')

print('k_l:', k_l, 'k_r:', k_r)