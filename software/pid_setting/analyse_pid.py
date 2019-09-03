import csv

path_to_dataset_l = 'datasets/dataset_l_test.csv'
path_to_dataset_r = 'datasets/dataset_r_test.csv'
max_linear_speed = 3.82

def per_cent_of_max_speed(speed):
    return speed*0.1/max_linear_speed*100

def get_errs_from_dataset(path_to_dataset):
    max_err = 0
    errs = []

    with open(path_to_dataset, 'r') as f:
        reader = csv.reader(f)

        flag = True
        for row in reader:
            if not flag:
                cur_err = abs(float(row[0]) - float(row[1]))
                errs += [cur_err]

                if cur_err > max_err:
                    max_err = cur_err
            flag = False

    mean_err = sum(errs) / len(errs)
    return max_err, mean_err

max_err_l, mean_err_l = get_errs_from_dataset(path_to_dataset_l)
max_err_r, mean_err_r = get_errs_from_dataset(path_to_dataset_r)

print('max_err_r:', max_err_r, ',%:', per_cent_of_max_speed(max_err_r), '|mean_err_r:', mean_err_r, ',%:', per_cent_of_max_speed(mean_err_r))
print('max_err_l:', max_err_l, ',%:', per_cent_of_max_speed(max_err_l), '|mean_err_l:', mean_err_l, ',%:', per_cent_of_max_speed(mean_err_l))
