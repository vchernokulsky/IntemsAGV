import numpy as np
import math

POINT_RADIUS = 1
POINT_COLOR = [0, 0, 255]
RANGE_SCALE = 100


class Photographer(object):
    def __init__(self, angle_min, angle_max, angle_increment, range_min, range_max, shape):
        self.angle_min = angle_min
        self.angle_max = angle_max
        self.angle_increment = angle_increment
        self.range_min = range_min
        self.range_max = range_max
        self.snapshots = []
        self.snapshot_shape = shape
        self.center = (shape[0]//2, shape[1]//2)

    def is_point_in_frame(self, x, y,):
        if (POINT_RADIUS < x < self.snapshot_shape[1] - POINT_RADIUS and
            POINT_RADIUS < y < self.snapshot_shape[0] - POINT_RADIUS):
            return True
        return False

    def take_photo(self, ranges):
        photo = 200 * np.ones(shape=self.snapshot_shape, dtype=np.uint8)

        i = 1
        for range_ in ranges:
            if range_ != float('inf'):
                x = self.center[1] + int(range_ * RANGE_SCALE * math.cos(self.angle_increment * i))
                y = self.center[0] + int(range_ * RANGE_SCALE * math.sin(self.angle_increment * i))
                if self.is_point_in_frame(x, y):
                    photo[y - POINT_RADIUS: y + POINT_RADIUS, x - POINT_RADIUS: x + POINT_RADIUS] = POINT_COLOR

            i += 1

        return photo
