import PIL
from PIL import Image

img = Image.open('pattern.jpg')
img = img.resize((50, 50), PIL.Image.ANTIALIAS)
img.save('pattern.jpg')