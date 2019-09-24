import os

path_to_neg_imgs = 'neg'
path_to_pos_imgs = 'pos'
path_to_neg_file = 'bg.txt'
path_to_pos_file = 'info.dat'

with open(path_to_neg_file, 'w') as f:
    for path_to_img in os.listdir(path_to_neg_imgs):
        f.write(os.path.join(path_to_neg_imgs, path_to_img) + '\n')

"""with open(path_to_pos_file, 'w') as f:
    for path_to_img in os.listdir(path_to_pos_imgs):
        f.write(os.path.join(path_to_neg_imgs, path_to_img) + ' 1 0 0 50 50\n')"""