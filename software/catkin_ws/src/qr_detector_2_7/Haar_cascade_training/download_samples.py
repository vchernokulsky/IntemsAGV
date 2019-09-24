from google_images_download import google_images_download
import os

response = google_images_download.googleimagesdownload()

queries_samples = ['images', 'anything']

for query_sample in queries_samples:
    arguments = {'keywords': query_sample,
                 'format': 'jpg',
                 'limit': 2000,
                 'print_urls': True,
                 'chromedriver': '/usr/bin/chromedriver'}

    response.download(arguments)
